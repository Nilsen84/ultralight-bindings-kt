#pragma once
#include "jni/JniRef.h"
#include "Ultralight/platform/Logger.h"

class Logger final : public ultralight::Logger {
public:
    Logger(JNIEnv* env, jobject obj);
    void LogMessage(ultralight::LogLevel log_level, const ultralight::String &message) override;
private:
    JniGlobalRef<> logger_;
};
