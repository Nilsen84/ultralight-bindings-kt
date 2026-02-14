#pragma once
#include <jni.h>

namespace utils::env {
    JNIEnv* EnsureAttached();
    JNIEnv* GetOrAttach();
}
