#pragma once
#include <concepts>
#include <jni.h>
#include <utility>

#include "JniEnv.h"
#include "util/Util.h"

namespace detail {
    template<typename T>
    concept JniRefType = utils::OneOf<
        T,
        jobject,
        jclass,
        jthrowable,
        jstring,
        jarray,
        jbooleanArray,
        jbyteArray,
        jcharArray,
        jshortArray,
        jintArray,
        jlongArray,
        jfloatArray,
        jdoubleArray,
        jobjectArray
    >;

    template<JniRefType T>
    class JniBaseRef {
    public:
        JniBaseRef(const JniBaseRef &) = delete;
        JniBaseRef &operator=(const JniBaseRef &) = delete;

        JniBaseRef(JniBaseRef &&other) noexcept
            : handle_(std::exchange(other.handle_, nullptr)) {}

        JniBaseRef &operator=(JniBaseRef &&other) noexcept {
            std::swap(handle_, other.handle_);
            return *this;
        }

        operator T() const { return handle_; }
        T get() const { return handle_; }

    protected:
        explicit JniBaseRef(T handle) : handle_(handle) {}
        ~JniBaseRef() = default;
        T handle_;
    };
}

template<typename T = jobject>
class JniGlobalRef : public detail::JniBaseRef<T> {
public:
    static JniGlobalRef FromLocal(JNIEnv *env, T local_ref) {
        if (!local_ref) { return JniGlobalRef(nullptr); }
        T global_ref = static_cast<T>(env->NewGlobalRef(local_ref));
        return JniGlobalRef(global_ref);
    }

    static JniGlobalRef WrapGlobal(T global_ref) {
        return JniGlobalRef(global_ref);
    }

    JniGlobalRef(JniGlobalRef &&other) noexcept = default;
    JniGlobalRef &operator=(JniGlobalRef &&other) noexcept = default;

    ~JniGlobalRef() {
        if (this->handle_) {
            try {
                utils::env::GetOrAttach()->DeleteGlobalRef(this->handle_);
            } catch (...) {
                // Jvm is probably shutting down, just leak the reference
            }
        }
    }

private:
    using detail::JniBaseRef<T>::JniBaseRef;
};

template<typename T = jobject>
class JniLocalRef: public detail::JniBaseRef<T> {
public:
    static JniLocalRef WrapLocal(JNIEnv *env, T local) {
        return JniLocalRef(env, local);
    }

    JniLocalRef(JniLocalRef &&other) = default;
    JniLocalRef &operator=(JniLocalRef &&other) noexcept {
        std::swap(env_, other.env_);
        return detail::JniBaseRef<T>::operator=(std::move(other));
    }

    ~JniLocalRef() {
        if (this->handle_) {
            env_->DeleteLocalRef(this->handle_);
        }
    }

private:
    explicit JniLocalRef(JNIEnv* env, T handle) : detail::JniBaseRef<T>(handle), env_(env) {}
    JNIEnv *env_;
};