#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <concepts>
#include <jni.h>

#include "Refs.h"
#include "Util.h"
#include "jni/JniRef.h"

namespace utils::jni {
    class JniException final : public std::exception {
    public:
        explicit JniException(JNIEnv* env, jthrowable exception) : exception_(env, exception) {}

        const JniLocalRef<jthrowable>& Java() const { return exception_; }
        JniLocalRef<jthrowable>& Java() { return exception_; }

        static void ThrowIfPending(JNIEnv *env) {
            if (env->ExceptionCheck()) {
                jthrowable exception = env->ExceptionOccurred();
                env->ExceptionClear();
                throw JniException(env, exception);
            }
        }
    private:
        JniLocalRef<jthrowable> exception_;
    };

    template<std::invocable F>
        requires (std::is_void_v<std::invoke_result_t<F>> || std::default_initializable<std::invoke_result_t<F>>)
    std::invoke_result_t<F> WrapCppException(JNIEnv *env, F &&func) {
        using ResultType = std::invoke_result_t<F>;
        try {
            return std::forward<F>(func)();
        } catch (const JniException& ex) {
            if (env->Throw(ex.Java()) != 0) std::abort();
        } catch (const std::exception &e) {
            auto runtime = Refs::Get().RuntimeException.clazz;
            if (!runtime || env->ThrowNew(runtime, e.what()) != 0) std::abort();
        } catch (...) {
            auto runtime = Refs::Get().RuntimeException.clazz;
            if (!runtime || env->ThrowNew(runtime, "An unknown error occurred in native code.") != 0) std::abort();
        }
        if constexpr (!std::is_void_v<ResultType>) return {};
        else return;
    }

    inline jclass FindClass(JNIEnv *env, const char *name) {
        auto clazz = env->FindClass(name);
        JniException::ThrowIfPending(env);
        if (!clazz) throw std::runtime_error(std::string("Failed to find class: ") + name);
        return clazz;
    }

    inline jclass FindClassGlobal(JNIEnv *env, const char *name) {
        auto clazz = FindClass(env, name);
        auto globalClazz = static_cast<jclass>(env->NewGlobalRef(clazz));
        env->DeleteLocalRef(clazz);
        return globalClazz;
    }

    inline jmethodID FindMethod(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        auto method = env->GetMethodID(clazz, name, sig);
        JniException::ThrowIfPending(env);
        return method;
    }

    inline jmethodID FindStaticMethod(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        auto method = env->GetStaticMethodID(clazz, name, sig);
        JniException::ThrowIfPending(env);
        return method;
    }

    inline jfieldID FindField(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        auto field = env->GetFieldID(clazz, name, sig);
        JniException::ThrowIfPending(env);
        return field;
    }

    inline jfieldID FindStaticField(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        auto field = env->GetStaticFieldID(clazz, name, sig);
        JniException::ThrowIfPending(env);
        return field;
    }

    inline std::string JStringToStdString(JNIEnv *env, jstring jStr) {
        const char* chars = env->GetStringUTFChars(jStr, nullptr);
        ScopeGuard releaseChars([&] { env->ReleaseStringUTFChars(jStr, chars); });
        std::string result(chars);
        return result;
    }

    inline std::string ThrowableToString(JNIEnv *env, jthrowable throwable) {
        auto str = (jstring) env->CallStaticObjectMethod(
            Refs::Get().NativeUtils.clazz,
            Refs::Get().NativeUtils.throwableToString,
            throwable
        );
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
            return "Unknown exception (failed to get exception string)";
        }
        JniLocalRef strRef(env, str);
        return JStringToStdString(env, strRef);
    }
}

#define GET_FIELD_AS_PTR(env, obj, javaType, field) ([&]() -> auto { \
    jlong handle = env->GetLongField(obj, Refs::Get().javaType.field); \
    utils::jni::JniException::ThrowIfPending(env);\
    return reinterpret_cast<void*>(handle); \
})()

#define GET_HANDLE(env, obj, nativeType, javaType) \
    reinterpret_cast<nativeType*>(GET_FIELD_AS_PTR(env, obj, javaType, handle))