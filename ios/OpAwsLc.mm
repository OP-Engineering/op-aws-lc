#import "OpAwsLc.h"
#import "opawslcbindings.hpp"
#import <ReactCommon/CallInvoker.h>
#import <ReactCommon/RCTTurboModuleWithJSIBindings.h>

using namespace facebook;

@interface OpAwsLc () <RCTTurboModuleWithJSIBindings>
@end

@implementation OpAwsLc {
  bool _didInstall;
  std::weak_ptr<react::CallInvoker> _callInvoker;
}

- (void)
    installJSIBindingsWithRuntime:(jsi::Runtime &)runtime
                      callInvoker:
                          (const std::shared_ptr<facebook::react::CallInvoker>
                               &)callinvoker {
  try {
    opawslc::install(runtime, callinvoker);
    _didInstall = true;
  } catch (const std::exception &exc) {
    _didInstall = false;
  }
}

- (std::shared_ptr<facebook::react::TurboModule>)getTurboModule:
    (const facebook::react::ObjCTurboModule::InitParams &)params {
  return std::make_shared<facebook::react::NativeOpAwsLcSpecJSI>(params);
}

+ (NSString *)moduleName {
  return @"OpAwsLc";
}

- (nonnull NSNumber *)install {
  return @1;
}

@end
