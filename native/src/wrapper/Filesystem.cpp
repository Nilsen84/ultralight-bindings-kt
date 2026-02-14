#include "Filesystem.h"

#include "Refs.h"
#include "jni/JniEnv.h"
#include "jni/JniUtils.h"
#include "ultralight/Jni.h"

Filesystem::Filesystem(JNIEnv *env, jobject obj)
    : filesystem_(JniGlobalRef<>::FromLocal(env, obj)) {}

bool Filesystem::FileExists(const ultralight::String &file_path) {
    auto env = utils::env::GetOrAttach();
    auto str = utils::ultralight::ToJavaString(env, file_path);
    bool res = env->CallBooleanMethod(
        filesystem_,
        Refs::get().UltralightFilesystem.exists,
        str.get()
    );
    utils::jni::JniException::throwIfPending(env);
    return res;
}

ultralight::String Filesystem::GetFileMimeType(const ultralight::String &file_path) {
    auto env = utils::env::GetOrAttach();
    auto str = utils::ultralight::ToJavaString(env, file_path);
    auto res = (jstring)env->CallObjectMethod(
        filesystem_,
        Refs::get().UltralightFilesystem.getMimeType,
        str.get()
    );
    utils::jni::JniException::throwIfPending(env);
    return utils::ultralight::FromJavaString(env,JniLocalRef<jstring>::WrapLocal(env, res));
}

ultralight::String Filesystem::GetFileCharset(const ultralight::String &file_path) {
    auto env = utils::env::GetOrAttach();
    auto str = utils::ultralight::ToJavaString(env, file_path);
    auto res = (jstring)env->CallObjectMethod(
        filesystem_,
        Refs::get().UltralightFilesystem.getCharset,
        str.get()
    );
    utils::jni::JniException::throwIfPending(env);
    return utils::ultralight::FromJavaString(env, JniLocalRef<jstring>::WrapLocal(env, res));
}

ultralight::RefPtr<ultralight::Buffer> Filesystem::OpenFile(const ultralight::String &file_path) {
    auto env = utils::env::GetOrAttach();
    auto str = utils::ultralight::ToJavaString(env, file_path);
    auto res = (jbyteArray)env->CallObjectMethod(
        filesystem_,
        Refs::get().UltralightFilesystem.read,
        str.get()
    );
    utils::jni::JniException::throwIfPending(env);
    return utils::ultralight::FromJavaByteArray(env, JniLocalRef<jbyteArray>::WrapLocal(env, res));
}
