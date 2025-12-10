#pragma once

#include <ReactCommon/CallInvoker.h>
#include <memory>

namespace opawslc {
extern std::shared_ptr<facebook::react::CallInvoker> invoker;
extern bool invalidated;
} // namespace opawslc