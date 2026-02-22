#include <iostream>
#include "ViewListener.h"
#include "../Refs.h"
#include "../jni/JniEnv.h"
#include "../util/UlUtil.h"
#include "../util/JniUtil.h"

void ViewListenerWrapper::OnChangeTitle(ultralight::View* caller, const ultralight::String& title) {
    JNIEnv* env = utils::env::EnsureAttached();
    auto jTitle = utils::ul::UlStringToJString(env, title);

    env->CallVoidMethod(listener_, Refs::Get().UltralightViewListener.onChangeTitle, jTitle.Get());
    utils::jni::ThrowIfPending(env);
}

void ViewListenerWrapper::OnChangeURL(ultralight::View* caller, const ultralight::String& url) {
    JNIEnv* env = utils::env::EnsureAttached();
    auto jUrl = utils::ul::UlStringToJString(env, url);

    env->CallVoidMethod(listener_, Refs::Get().UltralightViewListener.onChangeURL, jUrl.Get());
    utils::jni::ThrowIfPending(env);
}

void ViewListenerWrapper::OnChangeTooltip(ultralight::View* caller, const ultralight::String& tooltip) {
    JNIEnv* env = utils::env::EnsureAttached();
    auto jTooltip = utils::ul::UlStringToJString(env, tooltip);

    env->CallVoidMethod(listener_, Refs::Get().UltralightViewListener.onChangeTooltip, jTooltip.Get());
    utils::jni::ThrowIfPending(env);
}

void ViewListenerWrapper::OnAddConsoleMessage(ultralight::View* caller, const ultralight::ConsoleMessage& message) {
    JNIEnv* env = utils::env::EnsureAttached();

    auto& refs = Refs::Get().UltralightViewListener;

    jobject jLevel;
    switch (message.level()) {
        case ultralight::kMessageLevel_Log:     jLevel = env->GetStaticObjectField(refs.MessageLevel.clazz, refs.MessageLevel.log); break;
        case ultralight::kMessageLevel_Warning: jLevel = env->GetStaticObjectField(refs.MessageLevel.clazz, refs.MessageLevel.warning); break;
        case ultralight::kMessageLevel_Error:   jLevel = env->GetStaticObjectField(refs.MessageLevel.clazz, refs.MessageLevel.error); break;
        case ultralight::kMessageLevel_Debug:   jLevel = env->GetStaticObjectField(refs.MessageLevel.clazz, refs.MessageLevel.debug); break;
        case ultralight::kMessageLevel_Info:    jLevel = env->GetStaticObjectField(refs.MessageLevel.clazz, refs.MessageLevel.info); break;
        default:                                jLevel = env->GetStaticObjectField(refs.MessageLevel.clazz, refs.MessageLevel.log); break;
    }
    JniLocalRef jLevelRef(env, jLevel);

    auto jMessage = utils::ul::UlStringToJString(env, message.message());
    auto jSourceId = utils::ul::UlStringToJString(env, message.source_id());

    env->CallVoidMethod(listener_, refs.onAddConsoleMessage, *jLevelRef, jMessage.Get(), (jint)message.line_number(), (jint)message.column_number(), *jSourceId);
    utils::jni::ThrowIfPending(env);
}
