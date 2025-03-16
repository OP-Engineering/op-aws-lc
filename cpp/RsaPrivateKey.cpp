#include "RsaPrivateKey.h"

namespace opawslc {
namespace jsi = facebook::jsi;

RsaPrivateKey::RsaPrivateKey(aws_lc::RsaKeySize key_size) {
  aws_lc::RsaError status;
  const char *err;

  status = aws_lc::generate_rsa_private_key(key_size, &rsa_private_key, &err);

  if (status != aws_lc::RsaError::Ok) {
    throw std::runtime_error(err);
  }
}

RsaPrivateKey::RsaPrivateKey(void *rsa_private_key)
    : rsa_private_key(rsa_private_key) {}

} // namespace opawslc
