#pragma once
#include <jni.h>
#include <JavaScriptCore/JSStringRef.h>

#include "JniUtil.h"
#include "Util.h"

namespace utils::js {
    inline JniLocalRef<jstring> JSStringToJString(JNIEnv *env, JSStringRef jsStr) {
        auto chars = JSStringGetCharactersPtr(jsStr);
        auto len = JSStringGetLength(jsStr);
        jstring result = env->NewString((const jchar*) chars, (jsize)len);
        jni::JniException::ThrowIfPending(env);
        return JniLocalRef<jstring>::WrapLocal(env, result);
    }

    inline JSStringRef JStringToJSString(JNIEnv *env, jstring jStr) {
        auto len = env->GetStringLength(jStr);
        const jchar* chars = env->GetStringCritical(jStr, nullptr);
        if (!chars) throw std::runtime_error("GetStringCritical returned null");
        ScopeGuard releaseChars([&] { env->ReleaseStringCritical(jStr, chars); });
        return JSStringCreateWithCharacters((const JSChar*)chars, len);
    }

    inline void ThrowJSException(JSContextRef ctx, JSValueRef exception) {
        JSStringRef exStr = JSValueToStringCopy(ctx, exception, nullptr);
        if (!exStr) throw std::runtime_error("Unknown JavaScript Exception");
        size_t bufSize = JSStringGetMaximumUTF8CStringSize(exStr);
        std::string buf(bufSize, '\0');
        JSStringGetUTF8CString(exStr, buf.data(), bufSize);
        JSStringRelease(exStr);
        throw std::runtime_error("JavaScript exception: " + buf);
    }

    template<std::invocable F>
    requires (std::is_void_v<std::invoke_result_t<F>> || std::default_initializable<std::invoke_result_t<F>>)
    std::invoke_result_t<F> WrapCppException(JSContextRef ctx, JSValueRef* exception, F &&func) {
        using ResultType = std::invoke_result_t<F>;
        try {
            return std::forward<F>(func)();
        } catch (const jni::JniException& ex) {
            auto s = jni::ThrowableToString(ex.Java().Env(), ex.Java());
            JSStringRef jsMsg = JSStringCreateWithUTF8CString(("Java Exception: " + s).c_str());
            *exception = JSValueMakeString(ctx, jsMsg);
            JSStringRelease(jsMsg);
        } catch (const std::exception& ex) {
            JSStringRef jsMsg = JSStringCreateWithUTF8CString((std::string("C++ exception: ") + ex.what()).c_str());
            *exception = JSValueMakeString(ctx, jsMsg);
            JSStringRelease(jsMsg);
        } catch (...) {
            JSStringRef jsMsg = JSStringCreateWithUTF8CString("Unknown native exception");
            *exception = JSValueMakeString(ctx, jsMsg);
            JSStringRelease(jsMsg);
        }
        if constexpr (!std::is_void_v<ResultType>) return {};
        else return;
    }
}

