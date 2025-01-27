#import "OpAwsLc.h"
#import <ReactCommon/CallInvoker.h>
#import <ReactCommon/RCTTurboModuleWithJSIBindings.h>

using namespace facebook;

@implementation OpAwsLc {
  bool _didInstall;
  std::weak_ptr<react::CallInvoker> _callInvoker;
}
RCT_EXPORT_MODULE()

- (void)installJSIBindingsWithRuntime:(jsi::Runtime &)runtime {
  auto callInvoker = _callInvoker.lock();
  if (callInvoker == nullptr) {
    throw std::runtime_error("CallInvoker is missing");
  }

  opawslc::install(runtime, callInvoker);
  _didInstall = true;
}

// The installJSIBindingsWithRuntime method is called by TurboModuleManager when
// the module is lazily initialized Therefore this function is just there to
// trigger the initialization of the turbo module
- (void)install {
  if (!_didInstall) {
    throw std::runtime_error("OP-AWS-LC module not installed");
  }
}

- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params {
  return std::make_shared<facebook::react::NativeOpAwsLcSpecJSI>(params);
}

@end
