#include "JniEnv.h"

#include "../Refs.h"
#include <stdexcept>
#include <string>

static thread_local struct Detacher {
    bool shouldDetach = false;

    ~Detacher() {
        if (shouldDetach) {
            Refs::jvm->DetachCurrentThread();
        }
    }
} detacher;

namespace utils::env {
    JNIEnv* EnsureAttached() {
        JNIEnv *env;
        auto res = Refs::jvm->GetEnv((void**)&env, JNI_VERSION_1_8);
        if (res == JNI_EDETACHED)
            throw std::runtime_error("Failed to get JNIEnv: thread is not attached to JVM");
        if (res != JNI_OK)
            throw std::runtime_error("Failed to get JNIEnv: GetEnv failed with code " + std::to_string(res));
        return env;
    }

    JNIEnv *GetOrAttach() {
        JNIEnv *env;
        auto res = Refs::jvm->GetEnv((void**)&env, JNI_VERSION_1_8);
        if (res == JNI_EDETACHED) {
            res = Refs::jvm->AttachCurrentThread((void**)&env, nullptr);
            if (res != JNI_OK) {
                throw std::runtime_error("AttachCurrentThread failed with code " + std::to_string(res));
            }
            detacher.shouldDetach = true;
        } else if (res != JNI_OK) {
            throw std::runtime_error("GetEnv failed with code " + std::to_string(res));
        }
        return env;
    }
}