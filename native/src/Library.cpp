#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Refs.h"
#include "jni/JniEnv.h"
#include "util/JniUtil.h"
#include "util/UlUtil.h"
#include "wrapper/Clipboard.h"
#include "wrapper/Filesystem.h"
#include "wrapper/Logger.h"

#include <AppCore/Platform.h>
#include <JavaScriptCore/JavaScript.h>
#include <Ultralight/Renderer.h>
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/JavaScript.h>
#include <Ultralight/View.h>
#include <Ultralight/StringSTL.h>

#include "util/JsUtil.h"

static ultralight::RefPtr<ultralight::Renderer> s_renderer{};
static Filesystem *s_filesystem{};
static Logger *s_logger{};
static Clipboard *s_clipboard{};

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_8) != JNI_OK) {
        return JNI_ERR;
    }

    Refs::jvm = vm;

    auto success = utils::jni::WrapCppException(env, [&] {
        Refs::Get().Init(env);
        return true;
    });

    return success ? JNI_VERSION_1_8 : JNI_ERR;
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightNative_setPlatformFontLoader(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        ultralight::Platform::instance().set_font_loader(
            ultralight::GetPlatformFontLoader());
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightNative_setConfig(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        ultralight::Config config;
        config.animation_timer_delay = 1 / 120.0;
        config.scroll_timer_delay = 1 / 120.0;
        config.resource_path_prefix = "$builtin/";
        ultralight::Platform::instance().set_config(config);
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightNative_setClipboard(
    JNIEnv *env, jobject obj, jobject clipboard) {
    return utils::jni::WrapCppException(env, [&] {
        if (s_clipboard)
            delete s_clipboard;
        s_clipboard = new Clipboard(env, clipboard);
        ultralight::Platform::instance().set_clipboard(s_clipboard);
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightNative_setLogger(
    JNIEnv *env, jobject obj, jobject logger) {
    return utils::jni::WrapCppException(env, [&] {
        if (s_logger)
            delete s_logger;
        s_logger = new Logger(env, logger);
        ultralight::Platform::instance().set_logger(s_logger);
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightNative_setFilesystem(
    JNIEnv *env, jobject obj, jobject fs) {
    return utils::jni::WrapCppException(env, [&] {
        if (s_filesystem)
            delete s_filesystem;
        s_filesystem = new Filesystem(env, fs);
        ultralight::Platform::instance().set_file_system(s_filesystem);
    });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightNative_createRenderer(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        s_renderer = ultralight::Renderer::Create();
        auto renderer = env->NewObject(Refs::Get().UltralightRendererImpl.clazz,
                                       Refs::Get().UltralightRendererImpl.ctor);
        utils::jni::ThrowIfPending(env);
        return renderer;
    });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightRendererImpl_createView(
    JNIEnv *env, jobject obj, jint width, jint height) {
    return utils::jni::WrapCppException(env, [&] {
        auto config = ultralight::ViewConfig();
        config.is_transparent = true;
        auto view =
                s_renderer
                ->CreateView((uint32_t) width, (uint32_t) height, config, nullptr)
                .LeakRef();
        auto viewObj =
                env->NewObject(Refs::Get().UltralightViewImpl.clazz,
                               Refs::Get().UltralightViewImpl.ctor, (jlong) view);
        utils::jni::ThrowIfPending(env);
        return viewObj;
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightRendererImpl_update(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] { s_renderer->Update(); });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightRendererImpl_refreshDisplay(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env,
                                        [&] { s_renderer->RefreshDisplay(0); });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightRendererImpl_render(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] { s_renderer->Render(); });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_loadUrl(
    JNIEnv *env, jobject obj, jstring url) {
    return utils::jni::WrapCppException(env, [=] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        view->LoadURL(utils::ul::JStringToUlString(env, url));
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_resize(
    JNIEnv *env, jobject obj, jint width, jint height) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        view->Resize((uint32_t) width, (uint32_t) height);
    });
}

extern "C" JNIEXPORT jboolean
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_isLoading(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        return view->is_loading();
    });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_surface(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        return JNI_CHECK_EX(env, NewObject,
                            Refs::Get().UltralightBufferImpl.clazz,
                            Refs::Get().UltralightBufferImpl.ctor,
                            utils::PtrToInteger<jlong>(view->surface())
        );
    });
}

extern "C" JNIEXPORT jint
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_width(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        return view->width();
    });
}

extern "C" JNIEXPORT jint
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_height(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        return view->height();
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_close(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        view->Release();
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_focus(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        view->Focus();
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_fireScrollEvent(
    JNIEnv *env, jobject obj, jint deltaY) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        ultralight::ScrollEvent event;
        event.type = ultralight::ScrollEvent::kType_ScrollByPixel;
        event.delta_x = 0;
        event.delta_y = deltaY;
        view->FireScrollEvent(event);
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_fireMouseMoveEvent(
    JNIEnv *env, jobject obj, jint button, jint x, jint y) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        ultralight::MouseEvent event;
        event.type = ultralight::MouseEvent::kType_MouseMoved;
        event.x = x;
        event.y = y;
        event.button = ultralight::MouseEvent::kButton_None;
        switch (button) {
            case 0:
                event.button = ultralight::MouseEvent::kButton_Left;
                break;
            case 1:
                event.button = ultralight::MouseEvent::kButton_Right;
                break;
            case 2:
                event.button = ultralight::MouseEvent::kButton_Middle;
                break;
            default:
                event.button = ultralight::MouseEvent::kButton_None;
                break;
        }
        view->FireMouseEvent(event);
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_fireMouseButtonEvent(
    JNIEnv *env, jobject obj, jint button, jboolean down, jint x, jint y) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        ultralight::MouseEvent event;
        switch (button) {
            case 0:
                event.button = ultralight::MouseEvent::kButton_Left;
                break;
            case 1:
                event.button = ultralight::MouseEvent::kButton_Right;
                break;
            case 2:
                event.button = ultralight::MouseEvent::kButton_Middle;
                break;
            default:
                event.button = ultralight::MouseEvent::kButton_None;
                break;
        }

        event.type = down
                         ? ultralight::MouseEvent::kType_MouseDown
                         : ultralight::MouseEvent::kType_MouseUp;
        event.x = x;
        event.y = y;
        view->FireMouseEvent(event);
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_fireKeyEvent(
    JNIEnv *env, jobject obj, jint type, jint modifiers, jint virtualKey, jint nativeKey, jstring text,
    jstring unmodifiedText, jboolean isKeyPad, jboolean isAutoRepeat, jboolean isSystemKey) {
    using enum ultralight::KeyEvent::Type;
    using enum ultralight::KeyEvent::Modifiers;

    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);

        ultralight::KeyEvent event;
        if (type == 0) event.type = kType_RawKeyDown;
        else if (type == 1) event.type = kType_KeyUp;
        else event.type = kType_Char;

        if (modifiers & 1) event.modifiers |= kMod_ShiftKey;
        if (modifiers & 2) event.modifiers |= kMod_CtrlKey;
        if (modifiers & 4) event.modifiers |= kMod_AltKey;
        if (modifiers & 8) event.modifiers |= kMod_MetaKey;

        event.virtual_key_code = virtualKey;
        event.native_key_code = nativeKey;
        event.text = utils::ul::JStringToUlString(env, text);
        event.unmodified_text = utils::ul::JStringToUlString(env, unmodifiedText);
        event.is_keypad = isKeyPad;
        event.is_auto_repeat = isAutoRepeat;
        event.is_system_key = isSystemKey;

        view->FireKeyEvent(event);
    });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightSurfaceImpl_dirtyBounds(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto surface =
                GET_HANDLE(ultralight::Surface, env, obj, UltralightSurfaceImpl);
        auto res = env->NewObject(
            Refs::Get().IntRect.clazz, Refs::Get().IntRect.ctor,
            surface->dirty_bounds().left, surface->dirty_bounds().top,
            surface->dirty_bounds().right, surface->dirty_bounds().bottom);
        utils::jni::ThrowIfPending(env);
        return res;
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightSurfaceImpl_clearDirtyBounds(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto surface =
                GET_HANDLE(ultralight::Surface, env, obj, UltralightSurfaceImpl);
        surface->ClearDirtyBounds();
    });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightSurfaceImpl_lockPixels(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto surface =
                GET_HANDLE(ultralight::Surface, env, obj, UltralightSurfaceImpl);
        void *data = surface->LockPixels();
        auto buffer = env->NewObject(Refs::Get().UltralightBufferImpl.clazz,
                                     Refs::Get().UltralightBufferImpl.ctor,
                                     (jlong) surface, (jlong) data);
        utils::jni::ThrowIfPending(env);
        return buffer;
    });
}

extern "C" JNIEXPORT jint
Java_io_github_nilsen84_ultralight_internal_UltralightSurfaceImpl_rowBytes(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto surface = GET_HANDLE(ultralight::Surface, env, obj, UltralightSurfaceImpl);
        return surface->row_bytes();
    });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightBufferImpl_asByteBuffer(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto surface = GET_FIELD_AS_PTR(ultralight::Surface, env, obj, UltralightBufferImpl, surface);
        auto data = GET_FIELD_AS_PTR(void, env, obj, UltralightBufferImpl, data);
        jobject buffer = env->NewDirectByteBuffer(data, surface->size());
        utils::jni::ThrowIfPending(env);
        return buffer;
    });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightBufferImpl_close(
    JNIEnv *env, jobject obj) {
    return utils::jni::WrapCppException(env, [&] {
        auto surface = GET_FIELD_AS_PTR(ultralight::Surface, env, obj, UltralightBufferImpl, surface);
        surface->UnlockPixels();
    });
}

extern "C" JNIEXPORT jstring
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_evaluateScript(
    JNIEnv *env, jobject obj, jstring script) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);

        auto str = utils::ul::JStringToUlString(env, script);

        ultralight::String exception;
        auto result = view->EvaluateScript(str, &exception);

        if (!exception.empty()) {
            std::string s = ultralight::Convert(exception);
            throw std::runtime_error("EvaluateScript: " + s);
        }

        return utils::ul::UlStringToJString(env, result).Leak();
    });
}

struct BoundCallback {
    JniGlobalRef<> callback;
};

static JSValueRef BoundCallbackInvoke(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject,
                                      size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception) {
    return utils::js::WrapCppException(ctx, exception, [&] {
        JNIEnv *env = utils::env::EnsureAttached();
        auto data = static_cast<BoundCallback *>(JSObjectGetPrivate(function));

        auto jArgs = JniLocalRef<jobjectArray>::WrapLocal(
            env, env->NewObjectArray(argumentCount, Refs::Get().String.clazz, nullptr));
        utils::jni::ThrowIfPending(env);

        for (size_t i = 0; i < argumentCount; i++) {
            JSStringRef argStr = JSValueToStringCopy(ctx, arguments[i], nullptr);
            utils::ScopeGuard releaseArgStr([&] { JSStringRelease(argStr); });
            auto jstr = utils::js::JSStringToJString(env, argStr);
            env->SetObjectArrayElement(jArgs.Get(), (jsize) i, jstr.Get());
        }

        auto rawResult = (jstring) env->CallObjectMethod(data->callback, Refs::Get().JsCallback.invoke, jArgs.Get());
        utils::jni::ThrowIfPending(env);
        JniLocalRef result(env, rawResult);
        if (!result) return JSValueMakeNull(ctx);

        auto jsStr = utils::js::JStringToJSString(env, result);
        JSValueRef jsVal = JSValueMakeString(ctx, jsStr);
        JSStringRelease(jsStr);
        return jsVal;
    });
}

static JSClassRef GetBoundCallbackClass() {
    static JSClassRef sClass = [] {
        JSClassDefinition def = kJSClassDefinitionEmpty;
        def.className = "JavaCallback";
        def.finalize = [](JSObjectRef ref) { delete static_cast<BoundCallback *>(JSObjectGetPrivate(ref)); };
        def.callAsFunction = BoundCallbackInvoke;
        return JSClassCreate(&def);
    }();
    return sClass;
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_bindFunction(
    JNIEnv *env, jobject obj, jstring name, jobject callback) {
    return utils::jni::WrapCppException(env, [&] {
        auto view = GET_HANDLE(ultralight::View, env, obj, UltralightViewImpl);
        auto jsContext = view->LockJSContext();
        JSContextRef ctx = *jsContext;

        auto jsName = utils::js::JStringToJSString(env, name);
        utils::ScopeGuard releaseJsName([&] { JSStringRelease(jsName); });

        auto data = std::make_unique<BoundCallback>(JniGlobalRef<>::FromLocal(env, callback));
        JSObjectRef funcObj = JSObjectMake(ctx, GetBoundCallbackClass(), data.get());
        data.release();

        JSObjectRef globalObj = JSContextGetGlobalObject(ctx);
        JSValueRef exception = nullptr;
        JSObjectSetProperty(ctx, globalObj, jsName, funcObj, kJSPropertyAttributeNone, &exception);
        if (exception) utils::js::ThrowJSException(ctx, exception);
    });
}
