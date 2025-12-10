#pragma once

#include <ReactCommon/CallInvoker.h>
#include <jsi/jsi.h>
#include <jsi/jsilib.h>

namespace opawslc {

namespace jsi = facebook::jsi;
namespace react = facebook::react;

void install(jsi::Runtime &rt,
             const std::shared_ptr<react::CallInvoker> &invoker);
// void invalidate();

} // namespace opawslc
