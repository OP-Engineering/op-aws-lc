#include <jni.h>
#include "op-aws-lc.h"

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_opawslc_OpAwsLcModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return opawslc::multiply(a, b);
}
