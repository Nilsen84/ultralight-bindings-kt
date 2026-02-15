#include "Clipboard.h"

#include "Refs.h"
#include "jni/JniRef.h"
#include "../util/JniUtil.h"
#include "../util/UlUtil.h"

Clipboard::Clipboard(JNIEnv *env, jobject clipboard)
    : clipboard_(JniGlobalRef<>::FromLocal(env, clipboard)) {
}

void Clipboard::Clear() {
    auto env = utils::env::EnsureAttached();
    env->CallVoidMethod(
        clipboard_,
        Refs::Get().UltralightClipboard.clear
    );
    utils::jni::JniException::ThrowIfPending(env);
}

ultralight::String Clipboard::ReadPlainText() {
    auto env = utils::env::EnsureAttached();
    auto jstr = (jstring) env->CallObjectMethod(
        clipboard_,
        Refs::Get().UltralightClipboard.read
    );
    utils::jni::JniException::ThrowIfPending(env);
    return utils::ul::JStringToUlString(env, JniLocalRef<jstring>::WrapLocal(env, jstr));
}

void Clipboard::WritePlainText(const ultralight::String &text) {
    auto env = utils::env::EnsureAttached();
    auto jstr = utils::ul::UlStringToJString(env, text);
    env->CallVoidMethod(
        clipboard_,
        Refs::Get().UltralightClipboard.write,
        jstr.Get()
    );
    utils::jni::JniException::ThrowIfPending(env);
}
