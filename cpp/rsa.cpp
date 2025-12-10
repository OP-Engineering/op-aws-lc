#include "OPThreadPool.hpp"
#include "RsaPrivateKey.hpp"
#include "RsaPublicKey.hpp"
#include "hmac.hpp"
#include "macros.hpp"

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

void add_rsa_module(jsi::Runtime &rt, jsi::Object &root_module) {
  auto rsa_module = jsi::Object(rt);
  auto key_module = jsi::Object(rt);

  auto generate_rsa_private_key = HFN0 {
    aws_lc::RsaKeySize key_size = rsa_key_size_from_double(args[0].asNumber());
    std::shared_ptr<RsaPrivateKey> key =
        std::make_shared<RsaPrivateKey>(key_size);

    auto key_object = jsi::Object(rt);

    key_object.setNativeState(rt, key);

    return key_object;
  });

  key_module.setProperty(rt, "generatePrivateKey",
                         std::move(generate_rsa_private_key));

  auto generate_rsa_public_key = HFN0 {
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

  auto rsa_public_to_DER_X509_bytes = HFN0 {
    std::shared_ptr<RsaPublicKey> public_key =
        args[0].asObject(rt).getNativeState<RsaPublicKey>(rt);
    const char *err;
    auto *output =
        new uint8_t[1024 * 1024]; // Reserve a 1MB buffer, because we do not
                                  // know the size of the output before hand
    size_t output_len = 0;

    aws_lc::rsa_public_key_to_DER_X509_bytes(public_key.get()->rsa_public_key,
                                             output, &output_len, &err);

    jsi::Function array_buffer_ctor =

        rt.global().getPropertyAsFunction(rt, "ArrayBuffer");
    jsi::Object o =
        array_buffer_ctor.callAsConstructor(rt, static_cast<int>(output_len))
            .getObject(rt);
    jsi::ArrayBuffer buf = o.getArrayBuffer(rt);
    memcpy(buf.data(rt), output, output_len);

    delete[] output;

    return o;
  });

  key_module.setProperty(rt, "publicKeyToDERX509Bytes",
                         std::move(rsa_public_to_DER_X509_bytes));

  auto DER_X509_bytes_to_rsa_public = HFN0 {
    auto array_buffer = args[0].asObject(rt).getArrayBuffer(rt);
    size_t len = array_buffer.size(rt);
    auto *data = new uint8_t[len];
    memcpy(data, array_buffer.data(rt), len);
    void *public_key_handle;
    const char *err;

    aws_lc::rsa_public_key_from_DER_X509_bytes(data, len, &public_key_handle,
                                               &err);

    std::shared_ptr<RsaPublicKey> key = std::make_shared<RsaPublicKey>(
        RsaPublicKey::fromRawPointer(public_key_handle));

    auto key_object = jsi::Object(rt);

    key_object.setNativeState(rt, key);

    return key_object;
  });

  key_module.setProperty(rt, "publicKeyFromDERX509Bytes",
                         std::move(DER_X509_bytes_to_rsa_public));

  auto rsa_private_to_DER_PKCS8_bytes = HFN0 {
    std::shared_ptr<RsaPrivateKey> private_key =
        args[0].asObject(rt).getNativeState<RsaPrivateKey>(rt);
    const char *err;
    auto *output =
        new uint8_t[1024 * 1024]; // Reserve a 1MB buffer, because we do not
                                  // know the size of the output before hand
    size_t output_len = 0;

    aws_lc::rsa_private_key_to_DER_PKCS8_bytes(
        private_key.get()->rsa_private_key, output, &output_len, &err);

    jsi::Function array_buffer_ctor =
        rt.global().getPropertyAsFunction(rt, "ArrayBuffer");
    jsi::Object o =
        array_buffer_ctor.callAsConstructor(rt, static_cast<int>(output_len))
            .getObject(rt);
    jsi::ArrayBuffer buf = o.getArrayBuffer(rt);
    memcpy(buf.data(rt), output, output_len);

    delete[] output;
    return o;
  });

  key_module.setProperty(rt, "privateKeyToDERPKCS8Bytes",
                         std::move(rsa_private_to_DER_PKCS8_bytes));

  auto DER_PKCS8_bytes_to_rsa_private = HFN0 {
    auto array_buffer = args[0].asObject(rt).getArrayBuffer(rt);
    size_t len = array_buffer.size(rt);
    auto *data = new uint8_t[len];
    memcpy(data, array_buffer.data(rt), len);
    void *private_key_handle;
    const char *err;

    aws_lc::rsa_private_key_from_DER_PKCS8_bytes(data, len, &private_key_handle,
                                                 &err);

    std::shared_ptr<RsaPrivateKey> key =
        std::make_shared<RsaPrivateKey>(private_key_handle);

    auto key_object = jsi::Object(rt);

    key_object.setNativeState(rt, key);

    return key_object;
  });

  key_module.setProperty(rt, "privateKeyFromDERPKCS8Bytes",
                         std::move(DER_PKCS8_bytes_to_rsa_private));

  rsa_module.setProperty(rt, "Key", std::move(key_module));

  auto oaep_encrypt = HFN0 {
    auto args_obj = args[0].asObject(rt);

    aws_lc::RsaOaepEncryptionAlgorithm algorithm =
        rsa_oaep_encryption_algorithm_from_double(
            args_obj.getProperty(rt, "algorithm").asNumber());
    std::shared_ptr<RsaPublicKey> public_key =
        args_obj.getProperty(rt, "publicKey")
            .asObject(rt)
            .getNativeState<RsaPublicKey>(rt);
    std::string plaintext =
        args_obj.getProperty(rt, "plaintext").asString(rt).utf8(rt);

    if (args_obj.hasProperty(rt, "label")) {
    }

    void *oaep_public_key_handle;
    size_t output_len = 0;
    const char *err;

    aws_lc::RsaError oaep_key_status = aws_lc::generate_rsa_oaep_key(
        public_key->rsa_public_key, &oaep_public_key_handle, &output_len, &err);

    if (oaep_key_status != aws_lc::RsaError::Ok) {
      throw std::runtime_error(err ? err : "Failed to generate OAEP key");
    }

    auto *output = new uint8_t[output_len];

    aws_lc::RsaError encrypt_status = aws_lc::encrypt_with_rsa_oaep_key(
        algorithm, oaep_public_key_handle,
        reinterpret_cast<const unsigned char *>(plaintext.c_str()),
        plaintext.length(), output, &output_len, &err);

    if (encrypt_status != aws_lc::RsaError::Ok) {
      delete[] output;
      throw std::runtime_error(err ? err : "RSA OAEP encryption failed");
    }

    jsi::Function array_buffer_ctor =
        rt.global().getPropertyAsFunction(rt, "ArrayBuffer");
    jsi::Object o =
        array_buffer_ctor.callAsConstructor(rt, static_cast<int>(output_len))
            .getObject(rt);
    jsi::ArrayBuffer buf = o.getArrayBuffer(rt);
    memcpy(buf.data(rt), output, output_len);

    return o;
  });
  rsa_module.setProperty(rt, "oaepEncrypt", std::move(oaep_encrypt));

  auto oaep_decrypt = HFN0 {
    auto args_obj = args[0].asObject(rt);
    aws_lc::RsaOaepEncryptionAlgorithm algorithm =
        rsa_oaep_encryption_algorithm_from_double(
            args_obj.getProperty(rt, "algorithm").asNumber());
    std::shared_ptr<RsaPrivateKey> private_key =
        args_obj.getProperty(rt, "privateKey")
            .asObject(rt)
            .getNativeState<RsaPrivateKey>(rt);
    auto cipher_array_buffer =
        args_obj.getProperty(rt, "ciphertext").asObject(rt).getArrayBuffer(rt);

    size_t cipher_text_len = cipher_array_buffer.size(rt);
    auto *cipher_text = new uint8_t[cipher_text_len];
    memcpy(cipher_text, cipher_array_buffer.data(rt), cipher_text_len);

    // Allocate buffer for output - max plaintext size is typically same as
    // ciphertext
    auto *output = new uint8_t[cipher_text_len];
    size_t output_len = 0;
    const char *err;

    aws_lc::RsaError status = aws_lc::rsa_oaep_decrypt(
        algorithm, private_key.get()->rsa_private_key, cipher_text,
        cipher_text_len, output, &output_len, &err);

    if (status != aws_lc::RsaError::Ok) {
      delete[] cipher_text;
      delete[] output;
      throw std::runtime_error(err ? err : "RSA OAEP decryption failed");
    }

    std::string plaintext(reinterpret_cast<char *>(output), output_len);
    delete[] cipher_text;
    delete[] output;

    return jsi::String::createFromUtf8(rt, plaintext);
  });

  rsa_module.setProperty(rt, "oaepDecrypt", std::move(oaep_decrypt));

  root_module.setProperty(rt, "rsa", std::move(rsa_module));
}
} // namespace opawslc
