#pragma once
#include <concepts>

namespace utils {
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
