#include <AppCore/Platform.h>
#include <memory>

#include "Refs.h"
#include "Ultralight/Renderer.h"
#include "Ultralight/platform/Config.h"
#include "Ultralight/platform/Platform.h"
#include "jni/JniUtils.h"
#include "ultralight/Jni.h"
#include "wrapper/Clipboard.h"
#include "wrapper/Filesystem.h"
#include "wrapper/Logger.h"

static ultralight::RefPtr<ultralight::Renderer> s_renderer;
static Filesystem *s_filesystem{};
static Logger *s_logger{};
static Clipboard *s_clipboard{};

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env;
  if (vm->GetEnv((void **)&env, JNI_VERSION_1_8) != JNI_OK) {
    return JNI_ERR;
  }

  Refs::jvm = vm;

  auto success = utils::jni::WrapCppException(env, [&] {
    Refs::get().init(env);
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
    auto renderer = env->NewObject(Refs::get().UltralightRendererImpl.clazz,
                                   Refs::get().UltralightRendererImpl.ctor);
    utils::jni::JniException::throwIfPending(env);
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
            ->CreateView((uint32_t)width, (uint32_t)height, config, nullptr)
            .LeakRef();
    auto viewObj =
        env->NewObject(Refs::get().UltralightViewImpl.clazz,
                       Refs::get().UltralightViewImpl.ctor, (jlong)view);
    utils::jni::JniException::throwIfPending(env);
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
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
    auto len = env->GetStringLength(url);
    auto data = env->GetStringCritical(url, nullptr);
    ultralight::String16 str(data, len);
    env->ReleaseStringCritical(url, data);
    view->LoadURL(str);

    view->surface();
  });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_resize(
    JNIEnv *env, jobject obj, jint width, jint height) {
  return utils::jni::WrapCppException(env, [&] {
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
    view->Resize((uint32_t)width, (uint32_t)height);
  });
}

extern "C" JNIEXPORT jboolean
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_isLoading(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
    return view->is_loading();
  });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_surface(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
    auto res = env->NewObject(Refs::get().UltralightSurfaceImpl.clazz,
                              Refs::get().UltralightSurfaceImpl.ctor,
                              (jlong)view->surface());
    utils::jni::JniException::throwIfPending(env);
    return res;
  });
}

extern "C" JNIEXPORT jint
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_width(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
    return view->width();
  });
}

extern "C" JNIEXPORT jint
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_height(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
    return view->height();
  });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_close(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
    view->Release();
  });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_focus(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
    view->Focus();
  });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightViewImpl_fireScrollEvent(
    JNIEnv *env, jobject obj, jint deltaY) {
  return utils::jni::WrapCppException(env, [&] {
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
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
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
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
    auto view = GET_HANDLE(env, obj, ultralight::View, UltralightViewImpl);
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

    event.type = down ? ultralight::MouseEvent::kType_MouseDown
                      : ultralight::MouseEvent::kType_MouseUp;
    event.x = x;
    event.y = y;
    view->FireMouseEvent(event);
  });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightSurfaceImpl_dirtyBounds(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto surface =
        GET_HANDLE(env, obj, ultralight::Surface, UltralightSurfaceImpl);
    auto res = env->NewObject(
        Refs::get().IntRect.clazz, Refs::get().IntRect.ctor,
        surface->dirty_bounds().left, surface->dirty_bounds().top,
        surface->dirty_bounds().right, surface->dirty_bounds().bottom);
    utils::jni::JniException::throwIfPending(env);
    return res;
  });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightSurfaceImpl_clearDirtyBounds(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto surface =
        GET_HANDLE(env, obj, ultralight::Surface, UltralightSurfaceImpl);
    surface->ClearDirtyBounds();
  });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightSurfaceImpl_lockPixels(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto surface =
        GET_HANDLE(env, obj, ultralight::Surface, UltralightSurfaceImpl);
    void *data = surface->LockPixels();
    auto buffer = env->NewObject(Refs::get().UltralightBufferImpl.clazz,
                                 Refs::get().UltralightBufferImpl.ctor,
                                 (jlong)surface, (jlong)data);
    utils::jni::JniException::throwIfPending(env);
    return buffer;
  });
}

extern "C" JNIEXPORT jint
Java_io_github_nilsen84_ultralight_internal_UltralightSurfaceImpl_rowBytes(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto surface =
        GET_HANDLE(env, obj, ultralight::Surface, UltralightSurfaceImpl);
    return surface->row_bytes();
  });
}

extern "C" JNIEXPORT jobject
Java_io_github_nilsen84_ultralight_internal_UltralightBufferImpl_asByteBuffer(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto surface = (ultralight::Surface *)GET_FIELD_AS_PTR(
        env, obj, UltralightBufferImpl, surface);
    auto data = GET_FIELD_AS_PTR(env, obj, UltralightBufferImpl, data);
    jobject buffer = env->NewDirectByteBuffer(data, surface->size());
    utils::jni::JniException::throwIfPending(env);
    return buffer;
  });
}

extern "C" JNIEXPORT void
Java_io_github_nilsen84_ultralight_internal_UltralightBufferImpl_close(
    JNIEnv *env, jobject obj) {
  return utils::jni::WrapCppException(env, [&] {
    auto surface = (ultralight::Surface *)GET_FIELD_AS_PTR(
        env, obj, UltralightBufferImpl, surface);
    surface->UnlockPixels();
  });
}
