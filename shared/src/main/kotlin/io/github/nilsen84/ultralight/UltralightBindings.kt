package io.github.nilsen84.ultralight

import io.github.nilsen84.ultralight.internal.BuiltinResourceFilesystem
import io.github.nilsen84.ultralight.internal.UltralightNative
import java.nio.ByteBuffer

fun interface JsCallback {
    fun invoke(args: Array<String>): String?
}

fun interface UltralightLogger {
    enum class Level {
        Error,
        Warning,
        Info
    }
    fun log(level: Level, message: String)
}

interface UltralightFilesystem {
    fun exists(path: String): Boolean
    fun getMimeType(path: String): String
    fun getCharset(path: String): String
    fun read(path: String): ByteArray
}

interface UltralightClipboard {
    fun clear()
    fun read(): String
    fun write(text: String)
}

interface UltralightRenderer {
    fun createView(width: Int, height: Int): UltralightView
    fun update()
    fun refreshDisplay()
    fun render()
}

interface UltralightView : AutoCloseable {
    fun loadUrl(url: String)
    fun resize(width: Int, height: Int)
    fun isLoading(): Boolean
    fun surface(): UltralightSurface
    fun width(): Int
    fun height(): Int
    override fun close()
    fun focus()

    fun fireMouseMoveEvent(button: Int, x: Int, y: Int)
    fun fireMouseButtonEvent(button: Int, down: Boolean, x: Int, y: Int)
    fun fireScrollEvent(deltaY: Int)

    fun evaluateScript(script: String): String
    fun bindFunction(name: String, callback: JsCallback)
}

interface UltralightSurface {
    fun dirtyBounds(): IntRect
    fun clearDirtyBounds()
    fun lockPixels(): UltralightBuffer
    fun rowBytes(): Int
}

interface UltralightBuffer : AutoCloseable {
    fun asByteBuffer(): ByteBuffer
    override fun close()
}

data class IntRect(val left: Int, val top: Int, val right: Int, val buttom: Int) {
    val width get() = right - left
    val height get() = buttom - top
    val isValid get() = width > 0 && height > 0
}

object Ultralight {
    fun createRenderer(
        filesystem: UltralightFilesystem? = null,
        logger: UltralightLogger? = null,
        clipboard: UltralightClipboard? = null,
    ): UltralightRenderer {
        UltralightNative.setConfig()
        UltralightNative.setPlatformFontLoader()
        UltralightNative.setFilesystem(BuiltinResourceFilesystem(delegate = filesystem))
        if (logger != null) UltralightNative.setLogger(logger)
        if (clipboard != null) UltralightNative.setClipboard(clipboard)
        return UltralightNative.createRenderer()
    }
}