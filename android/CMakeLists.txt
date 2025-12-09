project(OpAwsLc)
cmake_minimum_required(VERSION 3.22.1)

set (PACKAGE_NAME "op-aws-lc")

include_directories(
  ../cpp
  src/main/cpp
  src/main/jni/include
)

file(GLOB_RECURSE shared_files RELATIVE ${CMAKE_SOURCE_DIR}
     "../cpp/**.cpp"
)

file(GLOB_RECURSE android_files RELATIVE ${CMAKE_SOURCE_DIR}
     "src/main/cpp/**.cpp"
)

cmake_path(SET LIB_AWS_LC_FIPS_CRYPTO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libaws_lc_fips_0_13_1_crypto.so NORMALIZE)
add_library(libaws_lc_fips_0_13_1_crypto SHARED IMPORTED)
set_target_properties(libaws_lc_fips_0_13_1_crypto PROPERTIES
  IMPORTED_LOCATION ${LIB_AWS_LC_FIPS_CRYPTO_PATH}
  IMPORTED_NO_SONAME ON
)

cmake_path(SET LIB_AWS_LC_FIPS_RUST_WRAPPER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libaws_lc_fips_0_13_1_rust_wrapper.so NORMALIZE)
add_library(libaws_lc_fips_0_13_1_rust_wrapper SHARED IMPORTED)
set_target_properties(libaws_lc_fips_0_13_1_rust_wrapper PROPERTIES
  IMPORTED_LOCATION ${LIB_AWS_LC_FIPS_RUST_WRAPPER_PATH}
  IMPORTED_NO_SONAME ON
)

add_library(libaws_lc SHARED IMPORTED)
set_target_properties(libaws_lc PROPERTIES
  IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libaws_lc.so
  IMPORTED_NO_SONAME ON
)

add_library(
  ${PACKAGE_NAME}
  SHARED
  ${shared_files}
  ${android_files}
)

find_package(ReactAndroid REQUIRED CONFIG)
find_package(fbjni REQUIRED CONFIG)
find_library(LOG_LIB log)

target_link_libraries(
  ${PACKAGE_NAME}
  libaws_lc_fips_0_13_1_rust_wrapper
  libaws_lc_fips_0_13_1_crypto
  libaws_lc
  ${LOG_LIB}
  ReactAndroid::reactnative
  ReactAndroid::jsi
  fbjni::fbjni
)
