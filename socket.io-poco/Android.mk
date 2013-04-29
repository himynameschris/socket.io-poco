LOCAL_PATH := $(call my-dir)

## STATIC LIBRARY

LOCAL_MODULE := socketiopoco_static

LOCAL_MODULE_FILENAME := libsocketiopoco

LOCAL_SRC_FILES := SIOClient.cpp \
				   SIOEventRegistry.cpp \
				   SIOEventTarget.cpp \
				   SIONotificationHandler.cpp 

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
							$(LOCAL_PATH)../../poco/Net/include \
							$(LOCAL_PATH)../../poco/Foundation/include \
							$(LOCAL_PATH)../../poco/JSON/include 
							

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
							$(LOCAL_PATH)../../poco/Net/include \
							$(LOCAL_PATH)../../poco/Foundation/include \
							$(LOCAL_PATH)../../poco/JSON/include 
					
LOCAL_STATIC_LIBRARIES := pocofoundation_static poconet_static pocojson_static

include $(BUILD_STATIC_LIBRARY)