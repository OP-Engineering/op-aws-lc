#include "OPThreadPool.h"
#include "RsaPrivateKey.h"
#include "RsaPublicKey.h"
#include "hmac.h"
#include "macros.h"

namespace opawslc {
namespace jsi = facebook::jsi;
namespace react = facebook::react;

// aws_lc::CHmacAlgorithm hmac_algorithm_from_double(double value) {
//   int intValue = static_cast<int>(value);
//   if (intValue < static_cast<int>(aws_lc::CHmacAlgorithm::SHA256) ||
//       intValue > static_cast<int>(aws_lc::CHmacAlgorithm::SHA512)) {
//     throw std::out_of_range("Invalid value for CHmacAlgorithm");
//   }
//   return static_cast<aws_lc::CHmacAlgorithm>(intValue);
// }

aws_lc::RsaKeySize rsa_key_size_from_double(double value) {
  int intValue = static_cast<int>(value);
  if (intValue < static_cast<int>(aws_lc::RsaKeySize::Rsa2048) ||
      intValue > static_cast<int>(aws_lc::RsaKeySize::Rsa8192)) {
    throw std::out_of_range("Invalid value for RsaKeySize");
  }
  return static_cast<aws_lc::RsaKeySize>(intValue);
}

aws_lc::RsaOaepEncryptionAlgorithm
rsa_oaep_encryption_algorithm_from_double(double value) {
  int intValue = static_cast<int>(value);
  if (intValue < static_cast<int>(
                     aws_lc::RsaOaepEncryptionAlgorithm::OaepSha1Mgf1Sha1) ||
      intValue >
          static_cast<int>(
              aws_lc::RsaOaepEncryptionAlgorithm::OaepSha512Mgf1Sha512)) {
    throw std::out_of_range("Invalid value for Rsa OAEP encryption algorithm");
  }
  return static_cast<aws_lc::RsaOaepEncryptionAlgorithm>(intValue);
}

void add_rsa_module(jsi::Runtime &rt,
                    std::shared_ptr<facebook::react::CallInvoker> invoker,
                    jsi::Object &root_module) {
  auto rsa_module = jsi::Object(rt);
  auto key_module = jsi::Object(rt);

  auto generate_rsa_private_key = HOST_STATIC_FN("generate_rsa_private_key") {
    aws_lc::RsaKeySize key_size = rsa_key_size_from_double(args[0].asNumber());
    std::shared_ptr<RsaPrivateKey> key =
        std::make_shared<RsaPrivateKey>(key_size);

    auto key_object = jsi::Object(rt);

    key_object.setNativeState(rt, key);

    return key_object;
  });

  key_module.setProperty(rt, "generatePrivateKey",
                         std::move(generate_rsa_private_key));

  auto generate_rsa_public_key = HOST_STATIC_FN("generate_rsa_public_key") {
    std::shared_ptr<RsaPrivateKey> private_key =
        args[0].asObject(rt).getNativeState<RsaPrivateKey>(rt);
    std::shared_ptr<RsaPublicKey> key =
        std::make_shared<RsaPublicKey>(private_key.get()->rsa_private_key);

    auto key_object = jsi::Object(rt);

    key_object.setNativeState(rt, key);

    return key_object;
  });

  key_module.setProperty(rt, "generatePublicKey",
                         std::move(generate_rsa_public_key));

  rsa_module.setProperty(rt, "Key", std::move(key_module));

  auto oaep_encrypt = HOST_STATIC_FN("oaep_encrypt") {
    aws_lc::RsaOaepEncryptionAlgorithm algorithm =
        rsa_oaep_encryption_algorithm_from_double(args[0].asNumber());
    std::shared_ptr<RsaPublicKey> public_key =
        args[1].asObject(rt).getNativeState<RsaPublicKey>(rt);
    std::string plaintext = args[2].asString(rt).utf8(rt);
    void *oaep_public_key_handle;
    size_t output_len;
    const char *err;

    aws_lc::generate_rsa_oaep_key(public_key.get()->rsa_public_key,
                                  &oaep_public_key_handle, &output_len, &err);
    auto *output = new uint8_t[output_len];
    
    aws_lc::encrypt_with_rsa_oaep_key(algorithm,
                                      oaep_public_key_handle,
                                      reinterpret_cast<const unsigned char *>(plaintext.c_str()),
                                      plaintext.length(),
                                      output,
                                      &output_len,
                                      &err);
    
    jsi::Function array_buffer_ctor =
    rt.global().getPropertyAsFunction(rt, "ArrayBuffer");
    jsi::Object o =
    array_buffer_ctor.callAsConstructor(rt, (int)output_len).getObject(rt);
    jsi::ArrayBuffer buf = o.getArrayBuffer(rt);
    memcpy(buf.data(rt), output, output_len);
    
    return o;
  });

  rsa_module.setProperty(rt, "oaepEncrypt", std::move(oaep_encrypt));

  root_module.setProperty(rt, "rsa", std::move(rsa_module));
}
} // namespace opawslc
