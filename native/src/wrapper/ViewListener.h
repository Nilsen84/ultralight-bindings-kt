#pragma once

#include <Ultralight/Listener.h>
#include <Ultralight/StringSTL.h>
#include "../jni/JniRef.h"

class ViewListenerWrapper : public ultralight::ViewListener {
public:
    explicit ViewListenerWrapper(JNIEnv* env, jobject listener)
        : listener_(JniGlobalRef<>::FromLocal(env, listener)) {}

    void OnChangeTitle(ultralight::View* caller, const ultralight::String& title) override;
    void OnChangeURL(ultralight::View* caller, const ultralight::String& url) override;
    void OnChangeTooltip(ultralight::View* caller, const ultralight::String& tooltip) override;
    void OnAddConsoleMessage(ultralight::View* caller, const ultralight::ConsoleMessage& message) override;

private:
    JniGlobalRef<> listener_;
};
