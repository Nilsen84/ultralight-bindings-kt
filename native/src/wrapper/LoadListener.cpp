#include "LoadListener.h"
#include "../Refs.h"
#include "../jni/JniEnv.h"
#include "../util/UlUtil.h"
#include "../util/JniUtil.h"

void LoadListenerWrapper::OnBeginLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) {
    if (!is_main_frame) return;

    JNIEnv* env = utils::env::EnsureAttached();
    auto jUrl = utils::ul::UlStringToJString(env, url);

    env->CallVoidMethod(listener_, Refs::Get().UltralightLoadListener.onBeginLoading, jUrl.Get());
    utils::jni::ThrowIfPending(env);
}

void LoadListenerWrapper::OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) {
    if (!is_main_frame) return;

    JNIEnv* env = utils::env::EnsureAttached();
    auto jUrl = utils::ul::UlStringToJString(env, url);

    env->CallVoidMethod(listener_, Refs::Get().UltralightLoadListener.onFinishLoading, jUrl.Get());
    utils::jni::ThrowIfPending(env);
}

void LoadListenerWrapper::OnFailLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url, const ultralight::String& description, const ultralight::String& error_domain, int error_code) {
    if (!is_main_frame) return;

    JNIEnv* env = utils::env::EnsureAttached();
    auto jUrl = utils::ul::UlStringToJString(env, url);

    env->CallVoidMethod(listener_, Refs::Get().UltralightLoadListener.onFailLoading, jUrl.Get());
    utils::jni::ThrowIfPending(env);
}

void LoadListenerWrapper::OnWindowObjectReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) {
    if (!is_main_frame) return;

    JNIEnv* env = utils::env::EnsureAttached();
    auto jUrl = utils::ul::UlStringToJString(env, url);

    env->CallVoidMethod(listener_, Refs::Get().UltralightLoadListener.onWindowObjectReady, jUrl.Get());
    utils::jni::ThrowIfPending(env);
}

void LoadListenerWrapper::OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) {
    if (!is_main_frame) return;

    JNIEnv* env = utils::env::EnsureAttached();
    auto jUrl = utils::ul::UlStringToJString(env, url);

    env->CallVoidMethod(listener_, Refs::Get().UltralightLoadListener.onDOMReady, jUrl.Get());
    utils::jni::ThrowIfPending(env);
}

void LoadListenerWrapper::OnUpdateHistory(ultralight::View* caller) {
    JNIEnv* env = utils::env::EnsureAttached();
    env->CallVoidMethod(listener_, Refs::Get().UltralightLoadListener.onUpdateHistory);
    utils::jni::ThrowIfPending(env);
}
