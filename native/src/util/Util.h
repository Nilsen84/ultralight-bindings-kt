#pragma once
#include <concepts>

namespace utils {
    template <std::integral R, typename T>
    R PtrToInteger(const T *ptr) {
        static_assert(sizeof(T*) == sizeof(R), "Integer type must be the same size as pointer");
        return reinterpret_cast<R>(ptr);
    }

    template <typename T, std::integral R>
    T *IntegerToPtr(R integer) {
        static_assert(sizeof(T*) == sizeof(R), "Integer type must be the same size as pointer");
        return reinterpret_cast<T*>(integer);
    }

    template <typename T, typename... Ts>
    concept OneOf = (std::same_as<T, Ts> || ...);

    template <std::invocable F>
    class ScopeGuard {
    public:
        explicit ScopeGuard(F&& f) : f(std::forward<F>(f)) {}
        ScopeGuard(ScopeGuard&& other) = delete;
        ScopeGuard& operator=(ScopeGuard&& other) = delete;
        ScopeGuard(const ScopeGuard&) = delete;
        ScopeGuard& operator=(const ScopeGuard&) = delete;
        ~ScopeGuard() { f(); }
    private:
        F f;
    };
}
