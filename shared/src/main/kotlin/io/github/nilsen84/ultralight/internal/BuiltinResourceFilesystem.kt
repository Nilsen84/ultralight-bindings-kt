package io.github.nilsen84.ultralight.internal

import io.github.nilsen84.ultralight.UltralightFilesystem
import java.net.URL
import java.net.URLConnection

internal class BuiltinResourceFilesystem(
    private val delegate: UltralightFilesystem? = null
) : UltralightFilesystem {

    companion object {
        const val BUILTIN_PREFIX = $$"$builtin/"
        private const val CLASSPATH_RESOURCE_DIR = "io/github/nilsen84/ultralight/resources/"
    }

    private fun getBuiltinUrl(path: String): URL? {
        if (!path.startsWith(BUILTIN_PREFIX)) return null
        val resourceName = path.removePrefix(BUILTIN_PREFIX)
        return javaClass.classLoader.getResource(CLASSPATH_RESOURCE_DIR + resourceName)
    }

    override fun exists(path: String): Boolean {
        if (getBuiltinUrl(path) != null) return true
        return delegate?.exists(path) ?: false
    }

    override fun getMimeType(path: String): String {
        if (path.startsWith(BUILTIN_PREFIX)) {
            return URLConnection.guessContentTypeFromName(path) ?: "application/octet-stream"
        }
        return delegate?.getMimeType(path) ?: "application/unknown"
    }

    override fun getCharset(path: String): String {
        return delegate?.getCharset(path) ?: "utf-8"
    }

    override fun read(path: String): ByteArray {
        getBuiltinUrl(path)?.let { return it.readBytes() }
        return delegate?.read(path) ?: error("File not found: $path")
    }
}