#include "Logger.h"

#include "Refs.h"
#include "jni/JniEnv.h"
#include "jni/JniUtils.h"
#include "ultralight/Jni.h"

Logger::Logger(JNIEnv *env, jobject obj)
    : logger_(JniGlobalRef<>::FromLocal(env, obj)) {}

void Logger::LogMessage(ultralight::LogLevel log_level, const ultralight::String &message) {
    auto env = utils::env::EnsureAttached();
    jfieldID javaLevelField;
    switch (log_level) {
        case ultralight::LogLevel::Error:
            javaLevelField = Refs::get().UltralightLogger.Level.error;
            break;
        case ultralight::LogLevel::Warning:
            javaLevelField = Refs::get().UltralightLogger.Level.warning;
            break;
        default: // ultralight::LogLevel::Info:
            javaLevelField = Refs::get().UltralightLogger.Level.info;
            break;
    }
    jobject javaLevel = env->GetStaticObjectField(Refs::get().UltralightLogger.Level.clazz, javaLevelField);
    utils::jni::JniException::throwIfPending(env);
    JniLocalRef javaLevelRef = JniLocalRef<>::WrapLocal(env, javaLevel);

    auto javaMessage = utils::ultralight::ToJavaString(env, message);
    env->CallVoidMethod(
        logger_,
        Refs::get().UltralightLogger.log,
        javaLevelRef.get(),
        javaMessage.get()
    );
    utils::jni::JniException::throwIfPending(env);
}
