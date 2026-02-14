#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <concepts>
#include <jni.h>

namespace utils::jni {
    class JniException final : public std::exception {
    public:
        explicit JniException(jthrowable exception) : exception_(exception) {}
        jthrowable java() const { return exception_; }
        static void throwIfPending(JNIEnv *env) {
            if (env->ExceptionCheck()) {
                jthrowable exception = env->ExceptionOccurred();
                env->ExceptionClear();
                throw JniException(exception);
            }
        }
    private:
        jthrowable exception_;
    };

    template<std::invocable F>
        requires (std::is_void_v<std::invoke_result_t<F>> || std::default_initializable<std::invoke_result_t<F>>)
    std::invoke_result_t<F> WrapCppException(JNIEnv *env, F &&func) {
        using ResultType = std::invoke_result_t<F>;
        try {
            return std::forward<F>(func)();
            // ReSharper disable once CppDFAUnreachableCode
        } catch (...) {
            try {
                throw;
            } catch (const JniException& ex) {
                if (env->Throw(ex.java()) != 0) std::abort();
            } catch (const std::exception &e) {
                jclass runtime = Refs::get().RuntimeException.clazz;
                if (!runtime) std::abort();
                if (env->ThrowNew(runtime, e.what()) != 0) std::abort();
            } catch (...) {
                jclass runtime = Refs::get().RuntimeException.clazz;
                if (!runtime) std::abort();
                if (env->ThrowNew(runtime, "An unknown error occurred in native code.") != 0) std::abort();
            }
            if constexpr (!std::is_void_v<ResultType>) return {};
            else return;
        }
    }

    inline jclass FindClass(JNIEnv *env, const char *name) {
        auto clazz = env->FindClass(name);
        JniException::throwIfPending(env);
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
        JniException::throwIfPending(env);
        if (!method) throw std::runtime_error(std::string("Failed to find method: ") + name + sig);
        return method;
    }

    inline jmethodID FindStaticMethod(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        auto method = env->GetStaticMethodID(clazz, name, sig);
        JniException::throwIfPending(env);
        if (!method) throw std::runtime_error(std::string("Failed to find static method: ") + name + sig);
        return method;
    }

    inline jfieldID FindField(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        auto field = env->GetFieldID(clazz, name, sig);
        JniException::throwIfPending(env);
        if (!field) throw std::runtime_error(std::string("Failed to find field: ") + name);
        return field;
    }

    inline jfieldID FindStaticField(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        auto field = env->GetStaticFieldID(clazz, name, sig);
        JniException::throwIfPending(env);
        if (!field) throw std::runtime_error(std::string("Failed to find static field: ") + name);
        return field;
    }
}

#define GET_FIELD_AS_PTR(env, obj, javaType, field) ([&]() -> auto { \
    jlong handle = env->GetLongField(obj, Refs::get().javaType.field); \
    utils::jni::JniException::throwIfPending(env);\
    return reinterpret_cast<void*>(handle); \
})()

#define GET_HANDLE(env, obj, nativeType, javaType) \
    reinterpret_cast<nativeType*>(GET_FIELD_AS_PTR(env, obj, javaType, handle))