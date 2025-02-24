#include "op-aws-lc.h"
#include "hmac.h"
#include "libaws_lc.h"
#include "macros.h"
#include <iomanip>
#include <iostream>

namespace opawslc {
namespace jsi = facebook::jsi;
namespace react = facebook::react;

void install(jsi::Runtime &rt, std::shared_ptr<react::CallInvoker> invoker) {
  jsi::Object proxy = jsi::Object(rt);
  add_hmac_module(rt, invoker, proxy);
  rt.global().setProperty(rt, "__OPAwsLcProxy", std::move(proxy));
}
} // namespace opawslc
