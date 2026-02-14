#include "Clipboard.h"

#include "Refs.h"
#include "jni/JniRef.h"
#include "jni/JniUtils.h"
#include "ultralight/Jni.h"

Clipboard::Clipboard(JNIEnv *env, jobject clipboard)
    : clipboard_(JniGlobalRef<>::FromLocal(env, clipboard)) {
}

void Clipboard::Clear() {
    auto env = utils::env::EnsureAttached();
    env->CallVoidMethod(
        clipboard_,
        Refs::get().UltralightClipboard.clear
    );
    utils::jni::JniException::throwIfPending(env);
}

ultralight::String Clipboard::ReadPlainText() {
    auto env = utils::env::EnsureAttached();
    auto jstr = (jstring) env->CallObjectMethod(
        clipboard_,
        Refs::get().UltralightClipboard.read
    );
    utils::jni::JniException::throwIfPending(env);
    return utils::ultralight::FromJavaString(env, JniLocalRef<jstring>::WrapLocal(env, jstr));
}

void Clipboard::WritePlainText(const ultralight::String &text) {
    auto env = utils::env::EnsureAttached();
    auto jstr = utils::ultralight::ToJavaString(env, text);
    env->CallVoidMethod(
        clipboard_,
        Refs::get().UltralightClipboard.write,
        jstr.get()
    );
    utils::jni::JniException::throwIfPending(env);
}
