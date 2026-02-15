#pragma once
#include <concepts>
#include <jni.h>
#include <utility>

#include "JniEnv.h"

namespace detail {
    template<typename T>
    concept JniRefType = std::convertible_to<T, jobject>;

    template<JniRefType T>
    class JniBaseRef {
    public:
        JniBaseRef(const JniBaseRef &) = delete;
        JniBaseRef &operator=(const JniBaseRef &) = delete;

        JniBaseRef(JniBaseRef &&other) noexcept
            : handle_(std::exchange(other.handle_, nullptr)) {}

        JniBaseRef &operator=(JniBaseRef &&other) noexcept {
            if (this != &other) {
                std::swap(handle_, other.handle_);
            }
            return *this;
        }

        explicit operator bool() const noexcept { return handle_ != nullptr; }

        operator T() const { return handle_; }
        T Get() const { return handle_; }
        T Leak() { return std::exchange(handle_, nullptr); }

    protected:
        explicit JniBaseRef(T handle) : handle_(handle) {}
        ~JniBaseRef() = default;
        T handle_ = nullptr; // Initialize explicitly
    };
}

template<typename T = jobject>
class JniGlobalRef : public detail::JniBaseRef<T> {
public:
    static JniGlobalRef FromLocal(JNIEnv *env, T local_ref) {
        return JniGlobalRef(env->NewGlobalRef(local_ref));
    }

    static JniGlobalRef WrapGlobal(T global_ref) {
        return JniGlobalRef(global_ref);
    }

    JniGlobalRef() : detail::JniBaseRef<T>(nullptr) {}
    JniGlobalRef(JniGlobalRef &&other) noexcept = default;
    JniGlobalRef &operator=(JniGlobalRef &&other) noexcept = default;

    ~JniGlobalRef() {
        if (this->handle_) {
            try {
                JNIEnv* env = utils::env::EnsureAttached();
                env->DeleteGlobalRef(this->handle_);
            } catch (...) {
                // leak reference if we're not attached
            }
        }
    }

private:
    using detail::JniBaseRef<T>::JniBaseRef;
};

template<typename T = jobject>
class JniLocalRef: public detail::JniBaseRef<T> {
public:
    explicit JniLocalRef(JNIEnv* env, T handle) : detail::JniBaseRef<T>(handle), env_(env) {}

    static JniLocalRef WrapLocal(JNIEnv *env, T local) {
        return JniLocalRef(env, local);
    }

    JniLocalRef(const JniLocalRef &other) :
        JniLocalRef(other.env_, static_cast<T>(other.env_->NewLocalRef(other.handle_))) {}

    JniLocalRef &operator=(const JniLocalRef &other) {
        if (this == &other) return *this;
        if (this->handle_) env_->DeleteLocalRef(this->handle_);
        this->env_ = other.env_;
        this->handle_ = static_cast<T>(env_->NewLocalRef(other.handle_));
        return *this;
    }

    JniLocalRef(JniLocalRef &&other) noexcept = default;

    JniLocalRef &operator=(JniLocalRef &&other) noexcept {
        if (this != &other) {
            std::swap(env_, other.env_);
            detail::JniBaseRef<T>::operator=(std::move(other));
        }
        return *this;
    }

    ~JniLocalRef() {
        if (this->handle_) {
            env_->DeleteLocalRef(this->handle_);
        }
    }

    JNIEnv *Env() const { return env_; }

private:
    JNIEnv *env_ = nullptr;
};