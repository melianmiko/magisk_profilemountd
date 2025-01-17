This Magisk module mount device owner's Internal storage (`/data/media/0`) inside
other users internal storage as `PriamryFS` subfolder, if this folder exist.

It can be used to obtain persistent access to owner's filesystem in Work profile,
"Privacy Sandbok" (which technically is a separate user account) or in other
user's profile.

Tested on Android 15 (crDroid) with Magisk. **Install this module at your own risk**.

## Build from sources
Install Android SDK with NDK, `make` and `zip`, then:

```bash
make ANDROID_SDK_PATH=$PATH_TO_YOUR_ANDROID_SDK_ROOT magisk
```

## Download prebuilt
See Releases section.
