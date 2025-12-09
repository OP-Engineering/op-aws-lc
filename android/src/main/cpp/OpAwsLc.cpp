#include "../cpp/bindings.hpp"
#include "OpAwsLc.hpp"

#include <exception>

namespace opawslc {

using namespace facebook;

void JOpAwsLc::install(
    jni::alias_ref<jni::JClass>, jlong runtimePointer,
    jni::alias_ref<react::CallInvokerHolder::javaobject> callInvokerHolder) {
  auto runtime = reinterpret_cast<jsi::Runtime *>(runtimePointer);
  if (runtime == nullptr) {
    throw std::invalid_argument("jsi::Runtime was null!");
  }

  if (callInvokerHolder == nullptr) {
    throw std::invalid_argument("CallInvokerHolder was null!");
  }
  auto callInvoker = callInvokerHolder->cthis()->getCallInvoker();
  if (callInvoker == nullptr) {
    throw std::invalid_argument("CallInvoker was null!");
  }

  opawslc::install(*runtime, callInvoker);
}

void JOpAwsLc::registerNatives() {
  javaClassStatic()->registerNatives({
      makeNativeMethod("install", JOpAwsLc::install),
  });
}

} // namespace opawslc
