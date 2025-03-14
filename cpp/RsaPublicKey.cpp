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

} // namespace opawslc
