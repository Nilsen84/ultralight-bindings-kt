package io.github.nilsen84.ultralight.internal

import io.github.nilsen84.ultralight.IntRect
import io.github.nilsen84.ultralight.UltralightBuffer
import io.github.nilsen84.ultralight.UltralightClipboard
import io.github.nilsen84.ultralight.UltralightFilesystem
import io.github.nilsen84.ultralight.UltralightLogger
import io.github.nilsen84.ultralight.UltralightRenderer
import io.github.nilsen84.ultralight.UltralightSurface
import io.github.nilsen84.ultralight.UltralightView
import java.nio.ByteBuffer

internal object UltralightNative {
    external fun setConfig()
    external fun setClipboard(clipboard: UltralightClipboard)
    external fun setLogger(logger: UltralightLogger)
    external fun setFilesystem(fs: UltralightFilesystem)
    external fun setPlatformFontLoader()
    external fun createRenderer(): UltralightRendererImpl
}

internal class UltralightRendererImpl : UltralightRenderer {
    external override fun createView(width: Int, height: Int): UltralightViewImpl
    external override fun update()
    external override fun refreshDisplay()
    external override fun render()
}

internal class UltralightViewImpl(private val handle: Long) : UltralightView {
    external override fun loadUrl(url: String)
    external override fun resize(width: Int, height: Int)
    external override fun isLoading(): Boolean
    external override fun surface(): UltralightSurfaceImpl
    external override fun width(): Int
    external override fun height(): Int
    external override fun close()
    external override fun focus()
    external override fun fireMouseMoveEvent(button: Int, x: Int, y: Int)
    external override fun fireMouseButtonEvent(button: Int, down: Boolean, x: Int, y: Int)
    external override fun fireScrollEvent(deltaY: Int)
}

internal class UltralightSurfaceImpl(private val handle: Long) : UltralightSurface {
    external override fun dirtyBounds(): IntRect
    external override fun clearDirtyBounds()
    external override fun lockPixels(): UltralightBufferImpl
    external override fun rowBytes(): Int
}

internal class UltralightBufferImpl(private val surface: Long, private val data: Long) : UltralightBuffer {
    external override fun asByteBuffer(): ByteBuffer
    external override fun close()
}