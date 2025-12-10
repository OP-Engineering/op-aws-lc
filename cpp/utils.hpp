#pragma once

#include <ReactCommon/CallInvoker.h>
#include <jsi/jsi.h>
#include <string>
#include <vector>
#include <any>

namespace opawslc {

namespace jsi = facebook::jsi;
namespace react = facebook::react;

jsi::Value
promisify(jsi::Runtime &rt, std::function<std::any()> lambda,
          std::function<jsi::Value(jsi::Runtime &rt, std::any result)>
              resolve_callback);

} // namespace opawslc
