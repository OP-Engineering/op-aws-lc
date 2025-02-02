#include "op-aws-lc.h"
#include "HmacKey.h"
#include "macros.h"

namespace opawslc {
namespace jsi = facebook::jsi;
namespace react = facebook::react;

void install(jsi::Runtime &rt, std::shared_ptr<react::CallInvoker> invoker) {

  // TODO: I guess this is not only a sha256, it should take a param
  // HOST_STATIC_FN macro taking a name is pretty useless tbh, I don't know if
  // this is needed
  auto generate_hmac_sha256_key = HOST_STATIC_FN("generateHmacKey") {
    std::shared_ptr<HmacKey> key = std::make_shared<HmacKey>(rt);

    return jsi::Object::createFromHostObject(rt, key);
  });

  jsi::Object proxy = jsi::Object(rt);
  proxy.setProperty(rt, "generateHmacKey", std::move(generate_hmac_sha256_key));
  rt.global().setProperty(rt, "__OPAwsLcProxy", std::move(proxy));
}
} // namespace opawslc
