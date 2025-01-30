#include "op-aws-lc.h"
#include "macros.h"
#include "librn_aws_lc.h"

namespace opawslc {
namespace jsi = facebook::jsi;
namespace react = facebook::react;

void install(jsi::Runtime &rt,
             std::shared_ptr<react::CallInvoker> invoker) {

  auto hmac = HOST_STATIC_FN("makeHmac") {
    void *key;
    awsrc::generate_hmac_sha256_key(&key);
    return jsi::String::createFromUtf8(rt, "Hello, World!");
  });

  jsi::Object proxy = jsi::Object(rt);
  proxy.setProperty(rt, "hmac", std::move(hmac));
  rt.global().setProperty(rt, "__OPAwsLcProxy", std::move(proxy));
}
} // namespace opawslc
