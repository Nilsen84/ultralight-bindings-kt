#pragma once

#include <iostream>
#include <Ultralight/Listener.h>
#include <Ultralight/StringSTL.h>
#include "../jni/JniRef.h"

class LoadListenerWrapper : public ultralight::LoadListener {
public:
    explicit LoadListenerWrapper(JNIEnv* env, jobject listener)
        : listener_(JniGlobalRef<>::FromLocal(env, listener)) {}

    void OnBeginLoading(ultralight::View* caller,
                        uint64_t frame_id,
                        bool is_main_frame,
                        const ultralight::String& url) override;

    void OnFinishLoading(ultralight::View* caller,
                         uint64_t frame_id,
                         bool is_main_frame,
                         const ultralight::String& url) override;

    void OnFailLoading(ultralight::View* caller,
                       uint64_t frame_id,
                       bool is_main_frame,
                       const ultralight::String& url,
                       const ultralight::String& description,
                       const ultralight::String& error_domain,
                       int error_code) override;

    void OnWindowObjectReady(ultralight::View* caller,
                             uint64_t frame_id,
                             bool is_main_frame,
                             const ultralight::String& url) override;

    void OnDOMReady(ultralight::View* caller,
                    uint64_t frame_id,
                    bool is_main_frame,
                    const ultralight::String& url) override;

    void OnUpdateHistory(ultralight::View* caller) override;

private:
    JniGlobalRef<> listener_;
};
