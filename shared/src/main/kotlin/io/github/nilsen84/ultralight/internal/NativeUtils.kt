package io.github.nilsen84.ultralight.internal

internal object NativeUtils {
    @JvmStatic
    fun throwableToString(e: Throwable): String {
        return e.stackTraceToString()
    }
}