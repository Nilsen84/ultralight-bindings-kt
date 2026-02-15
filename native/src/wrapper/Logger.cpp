#include "Logger.h"

#include "Refs.h"
#include "jni/JniEnv.h"
#include "../util/JniUtil.h"
#include "../util/UlUtil.h"

Logger::Logger(JNIEnv *env, jobject obj)
    : logger_(JniGlobalRef<>::FromLocal(env, obj)) {}

void Logger::LogMessage(ultralight::LogLevel log_level, const ultralight::String &message) {
    auto env = utils::env::EnsureAttached();
    jfieldID javaLevelField;
    switch (log_level) {
        case ultralight::LogLevel::Error:
            javaLevelField = Refs::Get().UltralightLogger.Level.error;
            break;
        case ultralight::LogLevel::Warning:
            javaLevelField = Refs::Get().UltralightLogger.Level.warning;
            break;
        default: // ultralight::LogLevel::Info:
            javaLevelField = Refs::Get().UltralightLogger.Level.info;
            break;
    }
    jobject javaLevel = env->GetStaticObjectField(Refs::Get().UltralightLogger.Level.clazz, javaLevelField);
    utils::jni::JniException::ThrowIfPending(env);
    JniLocalRef javaLevelRef = JniLocalRef<>::WrapLocal(env, javaLevel);

    auto javaMessage = utils::ul::UlStringToJString(env, message);
    env->CallVoidMethod(
        logger_,
        Refs::Get().UltralightLogger.log,
        javaLevelRef.Get(),
        javaMessage.Get()
    );
    utils::jni::JniException::ThrowIfPending(env);
}
