#pragma once

#include <jsi/jsi.h>
#include <libaws_lc.h>

namespace opawslc {
namespace jsi = facebook::jsi;

class JSI_EXPORT RsaPrivateKey : public jsi::NativeState {
public:
  RsaPrivateKey(aws_lc::RsaKeySize key_size);
  void *rsa_private_key;
};

} // namespace opawslc
