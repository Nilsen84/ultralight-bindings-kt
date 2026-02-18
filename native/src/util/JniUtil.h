#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <concepts>
#include <jni.h>

#include "Refs.h"
#include "Util.h"
#include "jni/JniRef.h"
#include "JniMacros.h"

namespace utils::jni {
    class JniException final : public std::exception {
    public:
        explicit JniException(JNIEnv *env, jthrowable exception) : exception_(env, exception) {
        }

        const JniLocalRef<jthrowable> &Java() const { return exception_; }
        JniLocalRef<jthrowable> &Java() { return exception_; }

    private:
        JniLocalRef<jthrowable> exception_;
    };

    template<std::invocable F>
        requires (std::is_void_v<std::invoke_result_t<F> > || std::default_initializable<std::invoke_result_t<F> >)
    std::invoke_result_t<F> WrapCppException(JNIEnv *env, F &&func) {
        using ResultType = std::invoke_result_t<F>;
        try {
            return std::forward<F>(func)();
        } catch (const JniException &ex) {
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
        return JNI_CHECK_EX_AND_NULL(env, FindClass, name);
    }

    inline jclass FindClassGlobal(JNIEnv *env, const char *name) {
        auto clazz = FindClass(env, name);
        auto globalClazz = static_cast<jclass>(env->NewGlobalRef(clazz));
        env->DeleteLocalRef(clazz);
        return globalClazz;
    }

    inline jmethodID FindMethod(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        return JNI_CHECK_EX_AND_NULL(env, GetMethodID, clazz, name, sig);
    }

    inline jmethodID FindStaticMethod(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        return JNI_CHECK_EX_AND_NULL(env, GetStaticMethodID, clazz, name, sig);
    }

    inline jfieldID FindField(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        return JNI_CHECK_EX_AND_NULL(env, GetFieldID, clazz, name, sig);
    }

    inline jfieldID FindStaticField(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
        return JNI_CHECK_EX_AND_NULL(env, GetStaticFieldID, clazz, name, sig);
    }

    inline std::string JStringToStdString(JNIEnv *env, jstring jStr) {
        const char *chars = JNI_CHECK_NULL(env, GetStringUTFChars, jStr, nullptr);
        ScopeGuard releaseChars([&] { env->ReleaseStringUTFChars(jStr, chars); });
        return chars;
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
