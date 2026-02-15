#pragma once
#include <jni.h>

#include <Ultralight/String.h>
#include <Ultralight/Buffer.h>

#include "util/Util.h"
#include "jni/JniRef.h"

#include <stdexcept>

namespace utils::ul {
    inline JniLocalRef<jstring> UlStringToJString(JNIEnv *env, const ::ultralight::String& string) {
        auto u16 = string.utf16();
        jstring res = env->NewString(u16.udata(), u16.length());
        return JniLocalRef<jstring>::WrapLocal(env, res);
    }

    inline ultralight::String16 JStringToUlString(JNIEnv *env, jstring string) {
        auto length = env->GetStringLength(string);
        const jchar* chars = env->GetStringCritical(string, nullptr);
        ScopeGuard releaseChars([&] { env->ReleaseStringCritical(string, chars); });
        return ultralight::String16(chars, length);
    }

    inline ultralight::RefPtr<ultralight::Buffer> JByteArrayToUlBuffer(JNIEnv *env, jbyteArray array) {
        auto length = env->GetArrayLength(array);
        auto bytes = (jbyte*)env->GetPrimitiveArrayCritical(array, nullptr);
        if (!bytes) throw std::runtime_error("GetPrimitiveArrayCritical returned null");
        ScopeGuard releaseBytes([&] { env->ReleasePrimitiveArrayCritical(array, bytes, JNI_ABORT); });
        return ultralight::Buffer::CreateFromCopy(bytes, length);
    }
}
