#include "Filesystem.h"

#include "Refs.h"
#include "jni/JniEnv.h"
#include "../util/JniUtil.h"
#include "../util/UlUtil.h"

Filesystem::Filesystem(JNIEnv *env, jobject obj)
    : filesystem_(JniGlobalRef<>::FromLocal(env, obj)) {}

bool Filesystem::FileExists(const ultralight::String &file_path) {
    auto env = utils::env::GetOrAttach();
    auto str = utils::ul::UlStringToJString(env, file_path);
    bool res = env->CallBooleanMethod(
        filesystem_,
        Refs::Get().UltralightFilesystem.exists,
        str.Get()
    );
    utils::jni::ThrowIfPending(env);
    return res;
}

ultralight::String Filesystem::GetFileMimeType(const ultralight::String &file_path) {
    auto env = utils::env::GetOrAttach();
    auto str = utils::ul::UlStringToJString(env, file_path);
    auto res = (jstring)env->CallObjectMethod(
        filesystem_,
        Refs::Get().UltralightFilesystem.getMimeType,
        str.Get()
    );
    utils::jni::ThrowIfPending(env);
    return utils::ul::JStringToUlString(env,JniLocalRef<jstring>::WrapLocal(env, res));
}

ultralight::String Filesystem::GetFileCharset(const ultralight::String &file_path) {
    auto env = utils::env::GetOrAttach();
    auto str = utils::ul::UlStringToJString(env, file_path);
    auto res = (jstring)env->CallObjectMethod(
        filesystem_,
        Refs::Get().UltralightFilesystem.getCharset,
        str.Get()
    );
    utils::jni::ThrowIfPending(env);
    return utils::ul::JStringToUlString(env, JniLocalRef<jstring>::WrapLocal(env, res));
}

ultralight::RefPtr<ultralight::Buffer> Filesystem::OpenFile(const ultralight::String &file_path) {
    auto env = utils::env::GetOrAttach();
    auto str = utils::ul::UlStringToJString(env, file_path);
    auto res = (jbyteArray)env->CallObjectMethod(
        filesystem_,
        Refs::Get().UltralightFilesystem.read,
        str.Get()
    );
    utils::jni::ThrowIfPending(env);
    return utils::ul::JByteArrayToUlBuffer(env, JniLocalRef<jbyteArray>::WrapLocal(env, res));
}
