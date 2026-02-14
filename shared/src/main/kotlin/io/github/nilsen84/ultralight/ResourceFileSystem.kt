package io.github.nilsen84.ultralight

import java.net.URL
import java.net.URLConnection

class ResourceFileSystem(
    val prefix: String = "",
    val loader: ClassLoader = ResourceFileSystem::class.java.classLoader
): UltralightFilesystem {
    private fun getUrl(path: String): URL? {
        return loader.getResource(prefix + path)
    }

    override fun exists(path: String): Boolean {
        return getUrl(path) != null
    }

    override fun getMimeType(path: String): String {
        return URLConnection.guessContentTypeFromName(path) ?: "application/unknown"
    }

    override fun getCharset(path: String): String {
        return "utf-8"
    }

    override fun read(path: String): ByteArray {
        return getUrl(path)?.readBytes() ?: error("File not found: $path")
    }
}