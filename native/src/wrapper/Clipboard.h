#pragma once
#include "jni/JniRef.h"
#include "Ultralight/platform/Clipboard.h"

class Clipboard final : public ultralight::Clipboard {
public:
    Clipboard(JNIEnv *env, jobject clipboard);

    void Clear() override;

    ultralight::String ReadPlainText() override;

    void WritePlainText(const ultralight::String &text) override;

private:
    JniGlobalRef<> clipboard_;
};
