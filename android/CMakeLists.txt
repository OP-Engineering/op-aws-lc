cmake_minimum_required(VERSION 3.9.0)
project(opawslc)


set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 20)

# Find all C++ files (shared and platform specifics)
file(GLOB_RECURSE shared_files RELATIVE ${CMAKE_SOURCE_DIR}
    "../cpp/**.cpp"
)
file(GLOB_RECURSE android_files RELATIVE ${CMAKE_SOURCE_DIR}
    "src/main/cpp/**.cpp"
)

add_library(${CMAKE_PROJECT_NAME} SHARED
        ${shared_files}
        ${android_files}
        cpp-adapter.cpp
)

# Specifies a path to native header files.
include_directories(
    ../cpp
    # Android-specific C++ includes
    # src/main/cpp/core
    # src/main/cpp/registry
    # src/main/cpp/turbomodule
    # src/main/cpp/platform
    # src/main/cpp/utils
)


cmake_path(SET LIB_AWS_LC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/librn_aws_lc.a NORMALIZE)
add_library(librn_aws_lc STATIC IMPORTED)
set_target_properties(librn_aws_lc PROPERTIES IMPORTED_LOCATION ${LIB_AWS_LC_PATH})

cmake_path(SET LIB_AWS_LC_FIPS_CRYPTO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libaws_lc_fips_0_13_1_crypto.so NORMALIZE)
add_library(libaws_lc_fips_0_13_1_crypto SHARED IMPORTED)
set_target_properties(libaws_lc_fips_0_13_1_crypto PROPERTIES IMPORTED_LOCATION ${LIB_AWS_LC_FIPS_CRYPTO_PATH})

cmake_path(SET LIB_AWS_LC_FIPS_RUST_WRAPPER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libaws_lc_fips_0_13_1_rust_wrapper.so NORMALIZE)
add_library(libaws_lc_fips_0_13_1_rust_wrapper SHARED IMPORTED)
set_target_properties(libaws_lc_fips_0_13_1_rust_wrapper PROPERTIES IMPORTED_LOCATION ${LIB_AWS_LC_FIPS_RUST_WRAPPER_PATH})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jni/include)

find_library(LOG_LIB log)
find_package(ReactAndroid REQUIRED CONFIG)
find_package(fbjni REQUIRED CONFIG)

if (ReactAndroid_VERSION_MINOR GREATER_EQUAL 76)
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  librn_aws_lc
  libaws_lc_fips_0_13_1_crypto
  libaws_lc_fips_0_13_1_rust_wrapper
  ${LOG_LIB}
  ReactAndroid::reactnative
  ReactAndroid::jsi
  fbjni::fbjni
)
else()
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  ${LOG_LIB}
  librn_aws_lc
  libaws_lc_fips_0_13_1_crypto
  libaws_lc_fips_0_13_1_rust_wrapper
  fbjni::fbjni
  ReactAndroid::jsi
  ReactAndroid::turbomodulejsijni
  ReactAndroid::react_nativemodule_core
  android
)
endif()
