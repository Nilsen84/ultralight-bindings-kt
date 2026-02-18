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
    JNI_CHECK_EX(env, CallVoidMethod, clipboard_, Refs::Get().UltralightClipboard.clear);
}

ultralight::String Clipboard::ReadPlainText() {
    auto env = utils::env::EnsureAttached();
    auto str = (jstring) JNI_CHECK_EX(env, CallObjectMethod, clipboard_, Refs::Get().UltralightClipboard.read);
    return utils::ul::JStringToUlString(env, JniLocalRef<jstring>::WrapLocal(env, str));
}

void Clipboard::WritePlainText(const ultralight::String &text) {
    auto env = utils::env::EnsureAttached();
    auto jstr = utils::ul::UlStringToJString(env, text);
    JNI_CHECK_EX(env, CallVoidMethod, clipboard_, Refs::Get().UltralightClipboard.write, jstr.Get());
}
