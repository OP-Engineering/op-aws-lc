ARCHS_IOS = aarch64-apple-ios aarch64-apple-ios-sim
ARCHS_ANDROID = aarch64-linux-android armv7-linux-androideabi x86_64-linux-android i686-linux-android
IOS_LIB_FILE = libaws_lc.dylib
IOS_FINAL_LIB_FILE = libaws_lc
LIB_FILE = libaws_lc.so
XCFRAMEWORK = libaws_lc.xcframework
XCFRAMEWORK_DEBUG = libaws_lc-debug.xcframework
HEADER = libaws_lc.h

notify:
	osascript -e 'display notification "Build completed" with title "AWS LC"'
	echo "🟢 Build Completed!"

lint:
	cargo clippy --all-features --all-targets -- --no-deps -D warnings

fix:
	cargo clippy --all-targets --all-features --fix --allow-dirty --allow-staged
	cargo fmt --all

nuke:
	rm -rf ./target

ios: clean ios-build package-dylibs copy-ios notify

clean:
	rm -rf generated/$(XCFRAMEWORK)
	rm -rf ../ios/$(XCFRAMEWORK)

package-dylibs:
	mkdir -p generated
# Delete old files
	rm -rf generated/libaws_lc*.xcframework
	
# Reset the .xcframeworks from the templates
	cp -r templates/libaws_lc*.xcframework generated

# Pack device dylibs
	cp target/aarch64-apple-ios/release/$(IOS_LIB_FILE) generated/$(IOS_FINAL_LIB_FILE)
	cp target/aarch64-apple-ios/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* generated/
	mv -f generated/libaws_lc_fips_0_13_10_crypto.dylib generated/libaws_lc_crypto
	mv -f generated/libaws_lc_fips_0_13_10_rust_wrapper.dylib generated/libaws_lc_rust_wrapper

	cp generated/libaws_lc_crypto generated/libaws_lc_crypto.xcframework/ios-arm64/libaws_lc_crypto.framework/
	install_name_tool -id @rpath/libaws_lc_crypto.framework/libaws_lc_crypto generated/libaws_lc_crypto.xcframework/ios-arm64/libaws_lc_crypto.framework/libaws_lc_crypto

	cp generated/libaws_lc_rust_wrapper generated/libaws_lc_rust_wrapper.xcframework/ios-arm64/libaws_lc_rust_wrapper.framework
	install_name_tool -id @rpath/libaws_lc_rust_wrapper.framework/libaws_lc_rust_wrapper generated/libaws_lc_rust_wrapper.xcframework/ios-arm64/libaws_lc_rust_wrapper.framework/libaws_lc_rust_wrapper
	install_name_tool -change @rpath/libaws_lc_fips_0_13_10_crypto.dylib @rpath/libaws_lc_crypto.framework/libaws_lc_crypto generated/libaws_lc_rust_wrapper.xcframework/ios-arm64/libaws_lc_rust_wrapper.framework/libaws_lc_rust_wrapper

	cp generated/$(IOS_FINAL_LIB_FILE) generated/libaws_lc.xcframework/ios-arm64/libaws_lc.framework/
	install_name_tool -id @rpath/libaws_lc.framework/$(IOS_FINAL_LIB_FILE) generated/libaws_lc.xcframework/ios-arm64/libaws_lc.framework/$(IOS_FINAL_LIB_FILE)
	install_name_tool -change @rpath/libaws_lc_fips_0_13_10_crypto.dylib @rpath/libaws_lc_crypto.framework/libaws_lc_crypto generated/libaws_lc.xcframework/ios-arm64/libaws_lc.framework/$(IOS_FINAL_LIB_FILE)
	install_name_tool -change @rpath/libaws_lc_fips_0_13_10_rust_wrapper.dylib @rpath/libaws_lc_rust_wrapper.framework/libaws_lc_rust_wrapper generated/libaws_lc.xcframework/ios-arm64/libaws_lc.framework/$(IOS_FINAL_LIB_FILE)
	
	mkdir -p generated/libaws_lc.xcframework/ios-arm64/libaws_lc.framework/Headers
	cp generated/include/libaws_lc.h generated/libaws_lc.xcframework/ios-arm64/libaws_lc.framework/Headers/

# Pack simulator dylibs
	cp target/aarch64-apple-ios-sim/release/$(IOS_LIB_FILE) generated/$(IOS_FINAL_LIB_FILE)
	cp target/aarch64-apple-ios-sim/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* generated/
	mv -f generated/libaws_lc_fips_0_13_10_crypto.dylib generated/libaws_lc_crypto
	mv -f generated/libaws_lc_fips_0_13_10_rust_wrapper.dylib generated/libaws_lc_rust_wrapper

	cp generated/libaws_lc_crypto generated/libaws_lc_crypto.xcframework/ios-arm64-simulator/libaws_lc_crypto.framework/
	install_name_tool -id @rpath/libaws_lc_crypto.framework/libaws_lc_crypto generated/libaws_lc_crypto.xcframework/ios-arm64-simulator/libaws_lc_crypto.framework/libaws_lc_crypto

	cp generated/libaws_lc_rust_wrapper generated/libaws_lc_rust_wrapper.xcframework/ios-arm64-simulator/libaws_lc_rust_wrapper.framework
	install_name_tool -id @rpath/libaws_lc_rust_wrapper.framework/libaws_lc_rust_wrapper generated/libaws_lc_rust_wrapper.xcframework/ios-arm64-simulator/libaws_lc_rust_wrapper.framework/libaws_lc_rust_wrapper
	install_name_tool -change @rpath/libaws_lc_fips_0_13_10_crypto.dylib @rpath/libaws_lc_crypto.framework/libaws_lc_crypto generated/libaws_lc_rust_wrapper.xcframework/ios-arm64-simulator/libaws_lc_rust_wrapper.framework/libaws_lc_rust_wrapper

	cp generated/$(IOS_FINAL_LIB_FILE) generated/libaws_lc.xcframework/ios-arm64-simulator/libaws_lc.framework/
	install_name_tool -id @rpath/libaws_lc.framework/$(IOS_FINAL_LIB_FILE) generated/libaws_lc.xcframework/ios-arm64-simulator/libaws_lc.framework/$(IOS_FINAL_LIB_FILE)
	install_name_tool -change @rpath/libaws_lc_fips_0_13_10_crypto.dylib @rpath/libaws_lc_crypto.framework/libaws_lc_crypto generated/libaws_lc.xcframework/ios-arm64-simulator/libaws_lc.framework/$(IOS_FINAL_LIB_FILE)
	install_name_tool -change @rpath/libaws_lc_fips_0_13_10_rust_wrapper.dylib @rpath/libaws_lc_rust_wrapper.framework/libaws_lc_rust_wrapper generated/libaws_lc.xcframework/ios-arm64-simulator/libaws_lc.framework/$(IOS_FINAL_LIB_FILE)
	
	mkdir -p generated/libaws_lc.xcframework/ios-arm64-simulator/libaws_lc.framework/Headers
	cp generated/include/libaws_lc.h generated/libaws_lc.xcframework/ios-arm64-simulator/libaws_lc.framework/Headers/

copy-ios:
	cp -rf generated/*.xcframework ../ios
	cd ../example/ios && yarn pods

ios-build:
	cargo build --release --target aarch64-apple-ios 
	cargo build --release --target aarch64-apple-ios-sim 

android-ci: android-ci-build copy-android

android: android-build copy-android notify

copy-android:
# Copy header
	mkdir -p ../android/src/main/jni/include
	rm -rf ../android/src/main/jniLibs
	cp -rf generated/include/* ../android/src/main/jni/include

	mkdir -p ../android/src/main/jniLibs/x86
	cp target/i686-linux-android/release/$(LIB_FILE) ../android/src/main/jniLibs/x86
	cp target/i686-linux-android/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* ../android/src/main/jniLibs/x86

	mkdir -p ../android/src/main/jniLibs/x86_64
	cp target/x86_64-linux-android/release/$(LIB_FILE) ../android/src/main/jniLibs/x86_64
	cp target/x86_64-linux-android/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* ../android/src/main/jniLibs/x86_64

	mkdir -p ../android/src/main/jniLibs/armeabi-v7a
	cp target/armv7-linux-androideabi/release/$(LIB_FILE) ../android/src/main/jniLibs/armeabi-v7a
	cp target/armv7-linux-androideabi/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* ../android/src/main/jniLibs/armeabi-v7a

	mkdir -p ../android/src/main/jniLibs/arm64-v8a
	cp target/aarch64-linux-android/release/$(LIB_FILE) ../android/src/main/jniLibs/arm64-v8a
	cp target/aarch64-linux-android/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* ../android/src/main/jniLibs/arm64-v8a

android-ci-build:
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/sysroot" cargo ndk --target aarch64-linux-android --platform 31 build --release
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/sysroot" cargo ndk --target armv7-linux-androideabi --platform 31 build --release
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/sysroot" cargo ndk --target i686-linux-android --platform 31 build --release
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/sysroot" cargo ndk --target x86_64-linux-android --platform 31 build --release

android-build:
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64/sysroot" cargo ndk --target aarch64-linux-android --platform 31 build --release
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64/sysroot" cargo ndk --target armv7-linux-androideabi --platform 31 build --release
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64/sysroot" cargo ndk --target i686-linux-android --platform 31 build --release
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64/sysroot" cargo ndk --target x86_64-linux-android --platform 31 build --release
