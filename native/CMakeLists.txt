cmake_minimum_required(VERSION 3.31.6)
project(UltralightJNI)

set(ULTRALIGHT_SDK_DIR "${CMAKE_SOURCE_DIR}/ultralight-free-sdk-1.4.0-win-x64" CACHE PATH "Path to the Ultralight SDK")
set(RENAMED_LIB_DIR "${CMAKE_SOURCE_DIR}/renamedlibs" CACHE PATH "Path to the renamed libraries")

if(NOT ULTRALIGHT_SDK_DIR OR NOT EXISTS "${ULTRALIGHT_SDK_DIR}")
    message(FATAL_ERROR "ULTRALIGHT_SDK_DIR must be set to a valid Ultralight SDK directory")
endif()

if(NOT RENAMED_LIB_DIR OR NOT EXISTS "${RENAMED_LIB_DIR}")
    message(FATAL_ERROR "RENAMED_LIB_DIR must be set to a valid directory containing renamed libraries")
endif()

set(ULTRALIGHT_INCLUDE_DIR "${ULTRALIGHT_SDK_DIR}/include")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(JNI REQUIRED)

add_library(UltralightJNI SHARED
        src/Library.cpp
        src/jni/JniUtils.h
        src/Refs.cpp
        src/Refs.h
        src/wrapper/Filesystem.cpp
        src/wrapper/Filesystem.h
        src/jni/JniRef.h
        src/jni/JniEnv.h
        src/jni/JniEnv.cpp
        src/ultralight/Jni.h
        src/util/Util.h
        src/wrapper/Logger.cpp
        src/wrapper/Logger.h
        src/wrapper/Clipboard.cpp
        src/wrapper/Clipboard.h
)
set_target_properties(UltralightJNI PROPERTIES OUTPUT_NAME "ultralight-jni")

target_link_libraries(UltralightJNI PRIVATE
        JNI::JNI
        Sph0
        Sph1
        Sph2
        Sph3
)
target_include_directories(UltralightJNI PRIVATE "${ULTRALIGHT_INCLUDE_DIR}" "src")
target_link_directories(UltralightJNI PRIVATE "${RENAMED_LIB_DIR}")

if(APPLE)
    set_target_properties(UltralightJNI PROPERTIES
        INSTALL_RPATH "@loader_path"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
elseif(UNIX)
    set_target_properties(UltralightJNI PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

if (NOT DEFINED ULTRALIGHT_PLATFORM)
    if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        set(ULTRALIGHT_PLATFORM "windows-x64")
    elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
        if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
            set(ULTRALIGHT_PLATFORM "mac-arm64")
        else()
            set(ULTRALIGHT_PLATFORM "mac-x64")
        endif ()
    elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
        set(ULTRALIGHT_PLATFORM "linux-x64")
    else ()
        message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
    endif()
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set_property(
            CACHE CMAKE_INSTALL_PREFIX
            PROPERTY VALUE
            "${CMAKE_SOURCE_DIR}/../platform/src/main/resources/io/github/nilsen84/ultralight/natives/${ULTRALIGHT_PLATFORM}"
    )
endif()

install(
        TARGETS UltralightJNI
        RUNTIME DESTINATION "."
        LIBRARY DESTINATION "."
)

install(
        DIRECTORY "${RENAMED_LIB_DIR}/"
        DESTINATION "."
        FILES_MATCHING
        PATTERN "*.dll"
        PATTERN "*.so"
        PATTERN "*.dylib"
)