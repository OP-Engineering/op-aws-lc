#include "HmacKey.h"
#include "libaws_lc.h"

namespace opawslc {
namespace jsi = facebook::jsi;

HmacKey::HmacKey(jsi::Runtime &rt) {
  int status = aws_lc::generate_hmac_sha256_key(&aws_lc_hmac_key);
  if (status != 0) {
    throw std::runtime_error("Failed to generate HMAC key");
  }
}

} // namespace opawslc
