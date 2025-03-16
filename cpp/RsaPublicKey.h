#pragma once

#include "RsaPrivateKey.h"
#include <jsi/jsi.h>
#include <libaws_lc.h>

namespace opawslc {
namespace jsi = facebook::jsi;

class JSI_EXPORT RsaPublicKey : public jsi::NativeState {
public:
  RsaPublicKey() {};
  RsaPublicKey(void *rsa_private_key);
  static RsaPublicKey fromRawPointer(void *rsa_public_key);
  void *rsa_public_key;
};

} // namespace opawslc
