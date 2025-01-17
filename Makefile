NDK_RELEASE := $(shell ls -Art ${ANDROID_SDK_PATH}/ndk | tail -n 1)
NDK_PATH := ${ANDROID_SDK_PATH}/ndk/${NDK_RELEASE}

all:
	${NDK_PATH}/ndk-build

magisk: all
	cp libs/arm64-v8a/profilemountd module/
	cd module && zip -r ../profilemountd.zip .

push2sd: magisk
	adb push profilemountd.zip /sdcard
