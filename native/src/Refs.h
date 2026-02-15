#pragma once
#include <jni.h>

struct Refs {
  inline static JavaVM *jvm{};

  struct { jclass clazz; } RuntimeException;
  struct { jclass clazz; } String;

  struct {
    jclass clazz;
    jmethodID throwableToString;
  } NativeUtils;

  struct {
    jclass clazz;
    jmethodID ctor;
  } IntRect;

  struct {
    jclass clazz;
    jmethodID ctor;
    jfieldID surface;
    jfieldID data;
  } UltralightBufferImpl;

  struct {
    jclass clazz;
    jmethodID ctor;
    jfieldID handle;
  } UltralightSurfaceImpl;

  struct {
    jclass clazz;
    jmethodID clear;
    jmethodID write;
    jmethodID read;
  } UltralightClipboard;

  struct {
    struct {
      jclass clazz;
      jfieldID error;
      jfieldID warning;
      jfieldID info;
    } Level;
    jclass clazz;
    jmethodID log;
  } UltralightLogger;

  struct {
    jclass clazz;
    jmethodID exists;
    jmethodID read;
    jmethodID getMimeType;
    jmethodID getCharset;
  } UltralightFilesystem;

  struct {
    jclass clazz;
    jmethodID invoke;
  } JsCallback;

  struct {
    jclass clazz;
  } UltralightNative;

  struct {
    jclass clazz;
    jmethodID ctor;
  } UltralightRendererImpl;

  struct {
    jclass clazz;
    jmethodID ctor;
    jfieldID handle;
  } UltralightViewImpl;

  static Refs &Get();
  void Init(JNIEnv *env);
};