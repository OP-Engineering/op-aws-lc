#pragma once

#include <jsi/jsi.h>
#include "libaws_lc.h"

namespace opawslc {
namespace jsi = facebook::jsi;

class JSI_EXPORT HmacKey : public jsi::HostObject {
public:
  HmacKey(jsi::Runtime &rt, aws_lc::CHmacAlgorithm algorithm);
//  std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime &rt) override;
//  jsi::Value get(jsi::Runtime &rt, const jsi::PropNameID &propNameID) override;

private:
  void *aws_lc_hmac_key;
};

} // namespace opawslc
