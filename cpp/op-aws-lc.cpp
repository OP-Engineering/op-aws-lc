#include "op-aws-lc.h"
#include "HmacKey.h"
#include "macros.h"
#include "libaws_lc.h"

namespace opawslc {
namespace jsi = facebook::jsi;
namespace react = facebook::react;
  
  aws_lc::CHmacAlgorithm fromDouble(double value) {
    int intValue = static_cast<int>(value);
    if (intValue < static_cast<int>(aws_lc::CHmacAlgorithm::SHA256) || intValue > static_cast<int>(aws_lc::CHmacAlgorithm::SHA512)) {
      throw std::out_of_range("Invalid value for CHmacAlgorithm");
    }
    return static_cast<aws_lc::CHmacAlgorithm>(intValue);
  }

void install(jsi::Runtime &rt, std::shared_ptr<react::CallInvoker> invoker) {
  auto generate_hmac_sha256_key = HOST_STATIC_FN("generateHmacKey") {
    aws_lc::CHmacAlgorithm algorithm = fromDouble(args[0].asNumber());
    std::shared_ptr<HmacKey> key = std::make_shared<HmacKey>(rt,
                                                             algorithm);

    return jsi::Object::createFromHostObject(rt, key);
  });

  jsi::Object proxy = jsi::Object(rt);
  proxy.setProperty(rt, "generateHmacKey", std::move(generate_hmac_sha256_key));
  rt.global().setProperty(rt, "__OPAwsLcProxy", std::move(proxy));
}
} // namespace opawslc
