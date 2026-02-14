#pragma once
#include <Ultralight/platform/Filesystem.h>
#include <jni.h>

#include "../jni/JniRef.h"

class Filesystem : public ultralight::FileSystem {
public:
    Filesystem(JNIEnv* env, jobject obj);

    bool FileExists(const ultralight::String &file_path) override;

    ultralight::String GetFileMimeType(const ultralight::String &file_path) override;

    ultralight::String GetFileCharset(const ultralight::String &file_path) override;

    ultralight::RefPtr<ultralight::Buffer> OpenFile(const ultralight::String &file_path) override;

private:
    JniGlobalRef<> filesystem_;
};
