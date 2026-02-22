package io.github.nilsen84.ultralight.internal

import io.github.nilsen84.ultralight.IntRect
import io.github.nilsen84.ultralight.JsCallback
import io.github.nilsen84.ultralight.KeyEvent
import io.github.nilsen84.ultralight.KeyEvent.Type
import io.github.nilsen84.ultralight.Ultralight
import io.github.nilsen84.ultralight.UltralightBuffer
import io.github.nilsen84.ultralight.UltralightClipboard
import io.github.nilsen84.ultralight.UltralightFilesystem
import io.github.nilsen84.ultralight.UltralightLogger
import io.github.nilsen84.ultralight.UltralightRenderer
import io.github.nilsen84.ultralight.UltralightSurface
import io.github.nilsen84.ultralight.UltralightView
import io.github.nilsen84.ultralight.UltralightLoadListener
import io.github.nilsen84.ultralight.UltralightViewListener
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
    external override fun setLoadListener(listener: UltralightLoadListener?)
    external override fun setViewListener(listener: UltralightViewListener?)

    external override fun fireMouseMoveEvent(button: Int, x: Int, y: Int)
    external override fun fireMouseButtonEvent(button: Int, down: Boolean, x: Int, y: Int)
    external override fun fireScrollEvent(deltaY: Int)
    external fun fireKeyEvent(type: Int, modifiers: Int, virtualKeyCode: Int, nativeKeyCode: Int, text: String, unmodifiedText: String, isKeyPad: Boolean, isAutoRepeat: Boolean, isSystemKey: Boolean)

    override fun fireKeyEvent(ev: KeyEvent) {
        fireKeyEvent(
            when (ev.type) {
                Type.KeyDown -> 0
                Type.KeyUp -> 1
                Type.Char -> 2
            },
            ev.modifiers.fold(0) { acc, key -> when (key) {
                KeyEvent.Modifier.Shift -> 1 or acc
                KeyEvent.Modifier.Ctrl -> 2 or acc
                KeyEvent.Modifier.Alt -> 4 or acc
                KeyEvent.Modifier.Meta -> 8 or acc
            } },
            ev.virtualKeyCode, ev.nativeKeyCode, ev.text, ev.unmodifiedText,
            ev.isKeyPad,
            ev.isAutoRepeat,
            ev.isSystemKey
        )
    }

    external override fun evaluateScript(script: String): String
    external override fun bindFunction(name: String, callback: JsCallback)
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