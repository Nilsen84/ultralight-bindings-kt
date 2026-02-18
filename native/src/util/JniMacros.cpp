#include "JniMacros.h"

#include "JniUtil.h"

namespace utils::jni {
    void ThrowIfPending(JNIEnv *env) {
        if (env->ExceptionCheck()) {
            auto ex = env->ExceptionOccurred();
            env->ExceptionClear();
            throw JniException(env, ex);
        }
    }
}