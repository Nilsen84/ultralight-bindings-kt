#pragma once
#include <concepts>
#include <jni.h>
#include <utility>

#include "util/JniMacros.h"
#include "jni/JniEnv.h"

namespace detail {
    template<typename T>
    concept JniRefType = std::convertible_to<T, jobject>;

    template<JniRefType T>
    class JniBaseRef {
    public:
        JniBaseRef(const JniBaseRef &) = delete;
        JniBaseRef &operator=(const JniBaseRef &) = delete;
        JniBaseRef(JniBaseRef &&other) = delete;
        JniBaseRef &operator=(JniBaseRef &&other) = delete;

        explicit operator bool() const noexcept { return handle_ != nullptr; }
        operator T() const { return handle_; }
        T Get() const { return handle_; }
        T Leak() { return std::exchange(handle_, nullptr); }

    protected:
        explicit JniBaseRef(T handle) : handle_(handle) {}
        ~JniBaseRef() = default;

        T handle_;
    };
}

template<typename T = jobject>
class JniGlobalRef : public detail::JniBaseRef<T> {
public:
    static JniGlobalRef WrapGlobal(T global_ref) { return JniGlobalRef(global_ref); }

    static JniGlobalRef FromLocal(JNIEnv *env, T local_ref) {
        if (!local_ref) JniGlobalRef(nullptr);
        return JniGlobalRef(static_cast<T>(JNI_CHECK_NULL(env, NewGlobalRef, local_ref)));
    }

    JniGlobalRef(JniGlobalRef &&other) noexcept
        : detail::JniBaseRef<T>(std::exchange(other.handle_, nullptr)) {}

    JniGlobalRef &operator=(JniGlobalRef &&other) noexcept {
        std::swap(other.handle_, this->handle_);
        return *this;
    }

    ~JniGlobalRef() {
        if (this->handle_) {
            try {
                JNIEnv *env = utils::env::EnsureAttached();
                env->DeleteGlobalRef(this->handle_);
            } catch (...) {
                // Can't do much about it, and we're already unwinding, so swallow any exceptions.
            }
        }
    }

protected:
    using detail::JniBaseRef<T>::JniBaseRef;
};

template<typename T = jobject>
class JniLocalRef : public detail::JniBaseRef<T> {
public:
    explicit JniLocalRef(JNIEnv *env, T handle) : detail::JniBaseRef<T>(handle), env_(env) {}
    static JniLocalRef WrapLocal(JNIEnv *env, T local) { return JniLocalRef(env, local); }

    JniLocalRef(const JniLocalRef &other)
        : detail::JniBaseRef<T>(nullptr)
        , env_(other.env_) {
        if (other.handle_ != nullptr) {
            this->handle_ = static_cast<T>(JNI_CHECK_NULL(this->env_, NewLocalRef, other.handle_));
        }
    }

    JniLocalRef &operator=(const JniLocalRef &other) {
        if (this == &other) return *this;
        if (this->handle_) this->env_->DeleteLocalRef(this->handle_);
        this->handle_ = nullptr;
        this->env_ = other.env_;
        if (other.handle_ != nullptr)
            this->handle_ = static_cast<T>(JNI_CHECK_NULL(this->env_, NewLocalRef, other.handle_));
        return *this;
    }

    JniLocalRef(JniLocalRef &&other) noexcept
        : detail::JniBaseRef<T>(std::exchange(other.handle_, nullptr))
        , env_(other.env_) {
    }

    JniLocalRef &operator=(JniLocalRef &&other) noexcept {
        std::swap(other.handle_, this->handle_);
        std::swap(other.env_, this->env_);
        return *this;
    }

    ~JniLocalRef() {
        env_->DeleteLocalRef(this->handle_);
    }

    JNIEnv *Env() const { return env_; }

private:
    JNIEnv *env_;
};