#include "HmacKey.h"

namespace opawslc {
namespace jsi = facebook::jsi;

HmacKey::HmacKey(jsi::Runtime &rt, aws_lc::CHmacAlgorithm algorithm) {

  aws_lc::CHmacError status;
  void *key_handle;
  char *err;
  
  status = aws_lc::generate_hmac_key(algorithm, &key_handle, &err);
  
  if (status != aws_lc::CHmacError::Ok) {
    throw std::runtime_error(err);
  }
}

} // namespace opawslc
