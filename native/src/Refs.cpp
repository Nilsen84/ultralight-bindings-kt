#include "Refs.h"
#include "util/JniUtil.h"

static Refs refs{};
Refs &Refs::Get() { return refs; }

void Refs::Init(JNIEnv *env) {
    using namespace utils::jni;

    RuntimeException.clazz = FindClassGlobal(env, "java/lang/RuntimeException");
    String.clazz = FindClassGlobal(env, "java/lang/String");

    NativeUtils.clazz = FindClassGlobal(env, "io/github/nilsen84/ultralight/internal/NativeUtils");
    NativeUtils.throwableToString = FindStaticMethod(env, NativeUtils.clazz, "throwableToString",
                                                     "(Ljava/lang/Throwable;)Ljava/lang/String;");

    UltralightNative.clazz = FindClassGlobal(
        env, "io/github/nilsen84/ultralight/internal/UltralightNative");

    JsCallback.clazz =
            FindClassGlobal(env, "io/github/nilsen84/ultralight/JsCallback");
    JsCallback.invoke = FindMethod(env, JsCallback.clazz, "invoke",
                                   "([Ljava/lang/String;)Ljava/lang/String;");

    UltralightRendererImpl.clazz = FindClassGlobal(
        env, "io/github/nilsen84/ultralight/internal/UltralightRendererImpl");
    UltralightRendererImpl.ctor =
            FindMethod(env, UltralightRendererImpl.clazz, "<init>", "()V");

    UltralightViewImpl.clazz = FindClassGlobal(
        env, "io/github/nilsen84/ultralight/internal/UltralightViewImpl");
    UltralightViewImpl.ctor =
            FindMethod(env, UltralightViewImpl.clazz, "<init>", "(J)V");
    UltralightViewImpl.handle =
            FindField(env, UltralightViewImpl.clazz, "handle", "J");

    UltralightFilesystem.clazz = FindClassGlobal(
        env, "io/github/nilsen84/ultralight/UltralightFilesystem");
    UltralightFilesystem.exists = FindMethod(env, UltralightFilesystem.clazz,
                                             "exists", "(Ljava/lang/String;)Z");
    UltralightFilesystem.read = FindMethod(env, UltralightFilesystem.clazz,
                                           "read", "(Ljava/lang/String;)[B");
    UltralightFilesystem.getMimeType =
            FindMethod(env, UltralightFilesystem.clazz, "getMimeType",
                       "(Ljava/lang/String;)Ljava/lang/String;");
    UltralightFilesystem.getCharset =
            FindMethod(env, UltralightFilesystem.clazz, "getCharset",
                       "(Ljava/lang/String;)Ljava/lang/String;");

    UltralightLogger.Level.clazz = FindClassGlobal(
        env, "io/github/nilsen84/ultralight/UltralightLogger$Level");
    UltralightLogger.Level.error =
            FindStaticField(env, UltralightLogger.Level.clazz, "Error",
                            "Lio/github/nilsen84/ultralight/UltralightLogger$Level;");
    UltralightLogger.Level.warning =
            FindStaticField(env, UltralightLogger.Level.clazz, "Warning",
                            "Lio/github/nilsen84/ultralight/UltralightLogger$Level;");
    UltralightLogger.Level.info =
            FindStaticField(env, UltralightLogger.Level.clazz, "Info",
                            "Lio/github/nilsen84/ultralight/UltralightLogger$Level;");
    UltralightLogger.clazz =
            FindClassGlobal(env, "io/github/nilsen84/ultralight/UltralightLogger");
    UltralightLogger.log =
            FindMethod(env, UltralightLogger.clazz, "log",
                       "(Lio/github/nilsen84/ultralight/UltralightLogger$Level;Ljava/"
                       "lang/String;)V");

    UltralightClipboard.clazz =
            FindClassGlobal(env, "io/github/nilsen84/ultralight/UltralightClipboard");
    UltralightClipboard.clear =
            FindMethod(env, UltralightClipboard.clazz, "clear", "()V");
    UltralightClipboard.read = FindMethod(env, UltralightClipboard.clazz, "read",
                                          "()Ljava/lang/String;");
    UltralightClipboard.write = FindMethod(env, UltralightClipboard.clazz,
                                           "write", "(Ljava/lang/String;)V");

    IntRect.clazz = FindClassGlobal(env, "io/github/nilsen84/ultralight/IntRect");
    IntRect.ctor = FindMethod(env, IntRect.clazz, "<init>", "(IIII)V");

    UltralightBufferImpl.clazz = FindClassGlobal(
        env, "io/github/nilsen84/ultralight/internal/UltralightBufferImpl");
    UltralightBufferImpl.ctor =
            FindMethod(env, UltralightBufferImpl.clazz, "<init>", "(JJ)V");
    UltralightBufferImpl.surface =
            FindField(env, UltralightBufferImpl.clazz, "surface", "J");
    UltralightBufferImpl.data =
            FindField(env, UltralightBufferImpl.clazz, "data", "J");

    UltralightSurfaceImpl.clazz = FindClassGlobal(
        env, "io/github/nilsen84/ultralight/internal/UltralightSurfaceImpl");
    UltralightSurfaceImpl.ctor =
            FindMethod(env, UltralightSurfaceImpl.clazz, "<init>", "(J)V");
    UltralightSurfaceImpl.handle =
            FindField(env, UltralightSurfaceImpl.clazz, "handle", "J");
}
