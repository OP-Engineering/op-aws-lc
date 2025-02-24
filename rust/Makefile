ARCHS_IOS = aarch64-apple-ios aarch64-apple-ios-sim
ARCHS_ANDROID = aarch64-linux-android armv7-linux-androideabi x86_64-linux-android i686-linux-android
LIB_FILE = libaws_lc.a
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

ios: clean ios-build package-xcframework package-dylibs copy-ios notify

clean:
	rm -rf generated/$(XCFRAMEWORK)
	rm -rf ../ios/$(XCFRAMEWORK)

package-dylibs:
# Delete old files
	rm -rf generated/libaws_lc_fips*.xcframework
# Reset the xcframework folders from the templates
	cp -r templates/libaws_lc_fips.xcframework generated
	cp -r templates/libaws_lc_fips_rust_wrapper.xcframework generated
# Extract the dylibs from the target folder
	cp target/aarch64-apple-ios/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* generated/
	mv -f generated/libaws_lc_fips_*_crypto.dylib generated/libaws_lc_fips
	mv -f generated/libaws_lc_fips_*_rust_wrapper.dylib generated/libaws_lc_fips_rust_wrapper
# Put the iOS device versions in the xcframework
	cp generated/libaws_lc_fips generated/libaws_lc_fips.xcframework/ios-arm64/libaws_lc_fips.framework
	install_name_tool -id @rpath/libaws_lc_fips.framework/libaws_lc_fips generated/libaws_lc_fips.xcframework/ios-arm64/libaws_lc_fips.framework/libaws_lc_fips
	cp generated/libaws_lc_fips_rust_wrapper generated/libaws_lc_fips_rust_wrapper.xcframework/ios-arm64/libaws_lc_fips_rust_wrapper.framework
	install_name_tool -id @rpath/libaws_lc_fips_rust_wrapper.framework/libaws_lc_fips_rust_wrapper generated/libaws_lc_fips_rust_wrapper.xcframework/ios-arm64/libaws_lc_fips_rust_wrapper.framework/libaws_lc_fips_rust_wrapper
# Do the same for the iOS simulator dylibs, extract the dylibs from the target folder
	cp target/aarch64-apple-ios-sim/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* generated/
	mv -f generated/libaws_lc_fips_*_crypto.dylib generated/libaws_lc_fips
	mv -f generated/libaws_lc_fips_*_rust_wrapper.dylib generated/libaws_lc_fips_rust_wrapper
# Put the iOS simulator versions in the xcframework
	cp generated/libaws_lc_fips generated/libaws_lc_fips.xcframework/ios-arm64-simulator/libaws_lc_fips.framework
	install_name_tool -id @rpath/libaws_lc_fips.framework/libaws_lc_fips generated/libaws_lc_fips.xcframework/ios-arm64-simulator/libaws_lc_fips.framework/libaws_lc_fips
	cp generated/libaws_lc_fips_rust_wrapper generated/libaws_lc_fips_rust_wrapper.xcframework/ios-arm64-simulator/libaws_lc_fips_rust_wrapper.framework
	install_name_tool -id @rpath/libaws_lc_fips_rust_wrapper.framework/libaws_lc_fips_rust_wrapper generated/libaws_lc_fips_rust_wrapper.xcframework/ios-arm64-simulator/libaws_lc_fips_rust_wrapper.framework/libaws_lc_fips_rust_wrapper

copy-ios:
	cp -rf generated/*.xcframework ../ios
	cd ../example/ios && yarn pods

package-xcframework:
	xcodebuild -create-xcframework -library target/aarch64-apple-ios/release/$(LIB_FILE) -headers generated/include -library target/aarch64-apple-ios-sim/release/$(LIB_FILE) -headers generated/include -output generated/$(XCFRAMEWORK)

ios-build:
	cargo build --release --target aarch64-apple-ios # Works
	cargo build --release --target aarch64-apple-ios-sim # Works
	# cargo build --release --target x86_64-apple-ios # not supported

android: android-build copy-android notify

copy-android:
# Copy header
	mkdir -p ../android/src/main/jni/include
	cp -rf generated/include/* ../android/src/main/jni/include
# Copy per architecture generated .a and needed .so files
	mkdir -p ../android/jniLibs/x86
	cp target/i686-linux-android/release/$(LIB_FILE) ../android/jniLibs/x86
	cp target/i686-linux-android/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* ../android/jniLibs/x86

	mkdir -p ../android/jniLibs/x86_64
	cp target/x86_64-linux-android/release/$(LIB_FILE) ../android/jniLibs/x86_64
	cp target/x86_64-linux-android/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* ../android/jniLibs/x86_64

	mkdir -p ../android/jniLibs/armeabi-v7a
	cp target/armv7-linux-androideabi/release/$(LIB_FILE) ../android/jniLibs/armeabi-v7a
	cp target/armv7-linux-androideabi/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* ../android/jniLibs/armeabi-v7a

	mkdir -p ../android/jniLibs/arm64-v8a
	cp target/aarch64-linux-android/release/$(LIB_FILE) ../android/jniLibs/arm64-v8a
	cp target/aarch64-linux-android/release/build/aws-lc-fips-sys-*/out/build/artifacts/libaws_lc_fips_* ../android/jniLibs/arm64-v8a

android-build:
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64/sysroot" cargo ndk --target aarch64-linux-android --platform 31 build --release
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64/sysroot" cargo ndk --target armv7-linux-androideabi --platform 31 build --release
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64/sysroot" cargo ndk --target i686-linux-android --platform 31 build --release
	BINDGEN_EXTRA_CLANG_ARGS="--sysroot=$$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64/sysroot" cargo ndk --target x86_64-linux-android --platform 31 build --release
