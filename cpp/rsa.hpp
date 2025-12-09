#pragma once

#include <ReactCommon/CallInvoker.h>
#include <jsi/jsi.h>

namespace opawslc {
namespace jsi = facebook::jsi;
namespace react = facebook::react;

void add_rsa_module(jsi::Runtime &runtime,
                    std::shared_ptr<facebook::react::CallInvoker> invoker,
                    jsi::Object &base_module);

} // namespace opawslc
