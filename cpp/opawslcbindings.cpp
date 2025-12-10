#include "opawslcbindings.hpp"
#include "hmac.hpp"
#include "libaws_lc.h"
#include "macros.hpp"
#include "rsa.hpp"
#include <string>
#include <unordered_map>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
namespace opawslc {

namespace jsi = facebook::jsi;
namespace react = facebook::react;

std::shared_ptr<react::CallInvoker> invoker;

// React native will try to clean the module on JS context invalidation
// (CodePush/Hot Reload) The clearState function is called
// void invalidate() {
//   for (auto &server : servers) {
//     if (server) {
//       server->stop();
//     }
//   }
//   servers.clear();
// }

void install(jsi::Runtime &rt,
             const std::shared_ptr<react::CallInvoker> &_invoker) {
  opawslc::invoker = _invoker;
  aws_lc::init_lib();
  jsi::Object proxy = jsi::Object(rt);
  add_hmac_module(rt, proxy);
  add_rsa_module(rt, proxy);
  rt.global().setProperty(rt, "__OPAwsLcProxy", std::move(proxy));
}

} // namespace opawslc

#pragma clang diagnostic pop
