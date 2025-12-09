#pragma once

#include <jsi/jsi.h>
#include <libaws_lc.h>

namespace opawslc {
namespace jsi = facebook::jsi;

class JSI_EXPORT HmacKey : public jsi::NativeState {
public:
  HmacKey(aws_lc::CHmacAlgorithm algorithm);
  void *hmac_key;
};

} // namespace opawslc
