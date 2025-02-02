#include "op-aws-lc.h"
#include <ReactCommon/CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <jni.h>
#include <jsi/jsi.h>
#include <typeinfo>

namespace jsi = facebook::jsi;
namespace react = facebook::react;
namespace jni = facebook::jni;

// This file is not using raw jni but rather fbjni, do not change how the native
// functions are registered
// https://github.com/facebookincubator/fbjni/blob/main/docs/quickref.md
struct OpAwsModule : jni::JavaClass<OpAwsModule> {
  static constexpr auto kJavaDescriptor = "Lcom/opawslc/OpAwsLcModule;";

  static void registerNatives() {
    javaClassStatic()->registerNatives({
        makeNativeMethod("installNativeJsi", OpAwsModule::installNativeJsi),
    });
  }

private:
  static void
  installNativeJsi(jni::alias_ref<jni::JObject> thiz, jlong jsiRuntimePtr,
                   jni::alias_ref<react::CallInvokerHolder::javaobject>
                       jsCallInvokerHolder) {
    auto jsiRuntime = reinterpret_cast<jsi::Runtime *>(jsiRuntimePtr);
    auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();

    opawslc::install(*jsiRuntime, jsCallInvoker);
  }

  static void clearStateNativeJsi(jni::alias_ref<jni::JObject> thiz) {

    // TODO add invalidate
    // opawslc::invalidate();
  }
};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
  return jni::initialize(vm, [] { OpAwsModule::registerNatives(); });
}
