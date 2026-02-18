#pragma once
#include <jni.h>
#include <stdexcept>
#include <type_traits>

#include "Util.h"

namespace utils::jni {
    void ThrowIfPending(JNIEnv *env);

    template <typename F>
    std::invoke_result_t<F> JniCheckEx(JNIEnv *env, F &&func) {
        if constexpr (std::is_void_v<std::invoke_result_t<F>>) {
            std::forward<F>(func)();
            ThrowIfPending(env);
            return;
        } else {
            decltype(auto) res = std::forward<F>(func)();
            ThrowIfPending(env);
            return res;
        }
    }
}

#define JNI_CHECK_NULL(env, func, ...) ([&]() { \
    auto result = env->func(__VA_ARGS__); \
    if (!result) { \
        env->ExceptionClear(); \
        throw std::runtime_error("JNI call returned null: " #func); \
    } \
    return result; \
})()

#define JNI_CHECK_EX(env, func, ...) \
    ::utils::jni::JniCheckEx(env, [&]() { return env->func(__VA_ARGS__); });

#define JNI_CHECK_EX_AND_NULL(env, func, ...) ([&]() { \
    auto result = env->func(__VA_ARGS__); \
    utils::jni::ThrowIfPending(env); \
    if (!result) throw std::runtime_error("JNI call returned null: " #func); \
    return result; \
})()

#define GET_FIELD_AS_PTR(targetType, env, obj, javaType, field) ([&]() -> auto { \
    jlong handle = env->GetLongField(obj, Refs::Get().javaType.field); \
    utils::jni::ThrowIfPending(env);\
    return utils::IntegerToPtr<targetType>(handle); \
})()

#define GET_HANDLE(nativeType, env, obj, javaType) \
    reinterpret_cast<nativeType*>(GET_FIELD_AS_PTR(void, env, obj, javaType, handle))