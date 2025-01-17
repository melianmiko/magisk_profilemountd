LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_CFLAGS += -Wall
LOCAL_LDLIBS := -llog -g
LOCAL_C_INCLUDES := bionic
LOCAL_MODULE := profilemountd
LOCAL_SRC_FILES := ../main.c
include $(BUILD_EXECUTABLE)
