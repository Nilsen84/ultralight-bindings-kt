package io.github.nilsen84.ultralight

import java.util.Locale
import kotlin.io.path.absolutePathString
import kotlin.io.path.createTempDirectory
import kotlin.io.path.outputStream

object UltralightLoader {
    fun load() {
        val bindings = getBindings()
        val natives = NATIVES.map {
            val name = bindings.prefix + it + "." + bindings.extension
            val path = "/$NATIVES_DIR/${bindings.folder}/$name"
            val url = javaClass.getResource(path) ?: error("Missing native library: $path")
            name to url
        }
        val output = createTempDirectory("ultralight-internal-natives-")

        for ((name, url) in natives) {
            val output = output.resolve(name)
            url.openStream()!!.use { input ->
                output.outputStream().use { output ->
                    input.copyTo(output)
                }
            }
            System.load(output.absolutePathString())
        }
    }
}

private enum class Bindings(val folder: String, val extension: String, val prefix: String) {
    MAC_X64("mac-x64", "dylib", "lib"),
    MAC_ARM64("mac-arm64", "dylib", "lib"),
    LINUX_X64("linux-x64", "so", "lib"),
    WINDOWS_X64("win-x64", "dll", "")
}

private val NATIVES = listOf(
    "Sph0",
    "Sph1",
    "Sph2",
    "Sph3",
    "ultralight-jni"
)

private const val NATIVES_DIR = "io/github/nilsen84/ultralight/natives"

private fun getBindings(): Bindings {
    val name = System.getProperty("os.name").lowercase(Locale.US)
    val arch = System.getProperty("os.arch")

    val x64 = arch == "amd64" || arch == "x86_64"
    val arm64 = arch == "aarch64" || arch == "arm64"

    val mac = name.contains("mac")
    val windows = name.contains("windows")
    val linux = name.contains("linux")

    return when {
        mac && x64 -> Bindings.MAC_X64
        mac && arm64 -> Bindings.MAC_ARM64
        linux && x64 -> Bindings.LINUX_X64
        windows && x64 -> Bindings.WINDOWS_X64
        else -> error("Unsupported platform: $name $arch")
    }
}