#pragma once
#include <jni.h>

#include <Ultralight/String.h>
#include <Ultralight/Buffer.h>

#include "util/Util.h"
#include "jni/JniRef.h"

#include <stdexcept>

namespace utils::ul {
    inline JniLocalRef<jstring> UlStringToJString(JNIEnv *env, const ultralight::String& string) {
        auto u16 = string.utf16();
        jstring s = JNI_CHECK_EX_AND_NULL(env, NewString, u16.udata(), u16.length());
        return JniLocalRef<jstring>::WrapLocal(env, s);
    }

    inline ultralight::String16 JStringToUlString(JNIEnv *env, jstring string) {
        auto length = env->GetStringLength(string);
        auto chars = JNI_CHECK_NULL(env, GetStringCritical, string, nullptr);
        ScopeGuard releaseChars([&] { env->ReleaseStringCritical(string, chars); });
        return ultralight::String16(chars, length);
    }

    inline ultralight::RefPtr<ultralight::Buffer> JByteArrayToUlBuffer(JNIEnv *env, jbyteArray array) {
        auto length = env->GetArrayLength(array);
        auto bytes = JNI_CHECK_EX_AND_NULL(env, GetPrimitiveArrayCritical, array, nullptr);
        ScopeGuard releaseBytes([&] { env->ReleasePrimitiveArrayCritical(array, bytes, JNI_ABORT); });
        return ultralight::Buffer::CreateFromCopy(bytes, length);
    }
}
