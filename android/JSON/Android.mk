LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS     := -DPOCO_ANDROID -DPOCO_NO_FPENVIRONMENT -DPOCO_NO_WSTRING -DPOCO_NO_SHAREDMEMORY

LOCAL_MODULE := pocojson_static

LOCAL_MODULE_FILENAME := libpocojson

LOCAL_SRC_FILES := \
src/Array.cpp \
src/Handler.cpp \
src/JSONException.cpp \
src/Object.cpp \
src/ParseHandler.cpp \
src/Parser.cpp \
src/PrintHandler.cpp \
src/Query.cpp \
src/Stringifier.cpp \
src/Template.cpp \
src/TemplateCache.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/../Foundation/include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/../Foundation/include

LOCAL_EXPORT_LDLIBS := -llog

include $(BUILD_STATIC_LIBRARY)