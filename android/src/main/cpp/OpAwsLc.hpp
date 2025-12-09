#pragma once

#include <ReactCommon/CallInvoker.h>
#include <ReactCommon/CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <jsi/jsi.h>

namespace opawslc {

using namespace facebook;

class JOpAwsLc final : public jni::HybridClass<JOpAwsLc> {
public:
  static auto constexpr kJavaDescriptor = "Lcom/opawslc/OpAwsLcModule;";
  static constexpr auto TAG = "OpAwsLcModule";

  static void registerNatives();

  static void install(
      jni::alias_ref<jni::JClass>, jlong runtimePointer,
      jni::alias_ref<react::CallInvokerHolder::javaobject> callInvokerHolder);

private:
  explicit JOpAwsLc() = default;
};

} // namespace opawslc
