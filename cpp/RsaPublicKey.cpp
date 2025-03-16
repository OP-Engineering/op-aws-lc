#include "RsaPublicKey.h"

namespace opawslc {
namespace jsi = facebook::jsi;

RsaPublicKey::RsaPublicKey(void *private_key) {
  aws_lc::RsaError status;
  const char *err;

  status = aws_lc::generate_rsa_public_key(private_key, &rsa_public_key, &err);

  if (status != aws_lc::RsaError::Ok) {
    throw std::runtime_error(err);
  }
}

RsaPublicKey RsaPublicKey::fromRawPointer(void *rsa_public_key_raw_pointer) {
  RsaPublicKey rsa_public_key;
  rsa_public_key.rsa_public_key = rsa_public_key_raw_pointer;
  return rsa_public_key;
}

} // namespace opawslc
