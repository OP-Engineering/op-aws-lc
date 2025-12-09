#include "HmacKey.hpp"

namespace opawslc {
namespace jsi = facebook::jsi;

HmacKey::HmacKey(aws_lc::CHmacAlgorithm algorithm) {
  aws_lc::CHmacError status;
  char *err;

  status = aws_lc::generate_hmac_key(algorithm, &hmac_key, &err);

  if (status != aws_lc::CHmacError::Ok) {
    throw std::runtime_error(err);
  }
}

} // namespace opawslc
