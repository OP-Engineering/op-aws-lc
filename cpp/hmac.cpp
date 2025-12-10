#include "hmac.hpp"
#include "HmacKey.hpp"
#include "OPThreadPool.hpp"
#include "macros.hpp"
#include "utils.hpp"
#include <any>
#include <utility>

namespace opawslc {
namespace jsi = facebook::jsi;
namespace react = facebook::react;

aws_lc::CHmacAlgorithm hmac_algorithm_from_double(double value) {
  int intValue = static_cast<int>(value);
  if (intValue < static_cast<int>(aws_lc::CHmacAlgorithm::SHA256) ||
      intValue > static_cast<int>(aws_lc::CHmacAlgorithm::SHA512)) {
    throw std::out_of_range("Invalid value for CHmacAlgorithm");
  }
  return static_cast<aws_lc::CHmacAlgorithm>(intValue);
}

void add_hmac_module(jsi::Runtime &rt, jsi::Object &root_module) {
  auto base_module = jsi::Object(rt);

  auto generate = HFN0 {
    aws_lc::CHmacAlgorithm algorithm =
        hmac_algorithm_from_double(args[0].asNumber());
    std::shared_ptr<HmacKey> key = std::make_shared<HmacKey>(algorithm);

    auto key_object = jsi::Object(rt);

    key_object.setNativeState(rt, key);

    return key_object;
  });

  auto key_module = jsi::Object(rt);
  key_module.setProperty(rt, "generate", std::move(generate));
  base_module.setProperty(rt, "Key", std::move(key_module));

  auto sign = HFN0 {
    std::shared_ptr<HmacKey> key =
        args[0].asObject(rt).getNativeState<HmacKey>(rt);
    std::string message = args[1].asString(rt).utf8(rt);
    uint8_t output_buffer[64];
    size_t output_len = 0;
    const char *err;

    auto status = aws_lc::hmac_sign(
        key.get()->hmac_key,
        reinterpret_cast<const unsigned char *>(message.c_str()),
        message.length(), output_buffer, &output_len, &err);

    if (status != aws_lc::CHmacError::Ok) {
      throw std::runtime_error(err);
    }

    jsi::Function array_buffer_ctor =
        rt.global().getPropertyAsFunction(rt, "ArrayBuffer");
    jsi::Object o =
        array_buffer_ctor.callAsConstructor(rt, static_cast<int>(output_len))
            .getObject(rt);
    jsi::ArrayBuffer buf = o.getArrayBuffer(rt);
    memcpy(buf.data(rt), output_buffer, output_len);

    return o;
  });

  auto verify = HFN0 {
    std::shared_ptr<HmacKey> key =
        args[0].asObject(rt).getNativeState<HmacKey>(rt);
    std::string message = args[1].asString(rt).utf8(rt);
    auto tag = args[2].asObject(rt).getArrayBuffer(rt);
    auto tag_size = tag.size(rt);
    uint8_t *data = new uint8_t[tag_size];
    memcpy(data, tag.data(rt), tag_size);

    auto status = aws_lc::hmac_verify(
        key.get()->hmac_key,
        reinterpret_cast<const unsigned char *>(message.c_str()),
        message.length(), data, tag_size);

    if (status != aws_lc::CHmacError::Ok) {
      return false;
    } else {
      return true;
    }
  });

  auto sign_async = HFN0 {
    std::shared_ptr<HmacKey> key =
        args[0].asObject(rt).getNativeState<HmacKey>(rt);
    std::string message = args[1].asString(rt).utf8(rt);

    // Use shared_ptr to ensure the buffer lives across async calls
    auto output_buffer = std::make_shared<std::array<uint8_t, 64>>();
    auto output_len = std::make_shared<size_t>(0);

    return promisify(
        rt,
        [output_buffer, output_len, message, key] {
          const char *err;
          auto status = aws_lc::hmac_sign(
              key.get()->hmac_key,
              reinterpret_cast<const unsigned char *>(message.c_str()),
              message.length(), output_buffer->data(), output_len.get(), &err);

          if (status != aws_lc::CHmacError::Ok) {
            throw std::runtime_error(err);
          }
          return 0;
        },
        [output_buffer, output_len](jsi::Runtime &rt, const std::any&) {
          jsi::Function array_buffer_ctor =
              rt.global().getPropertyAsFunction(rt, "ArrayBuffer");
          jsi::Object o =
              array_buffer_ctor
                  .callAsConstructor(rt, static_cast<int>(*output_len))
                  .getObject(rt);
          jsi::ArrayBuffer buf = o.getArrayBuffer(rt);
          memcpy(buf.data(rt), output_buffer->data(), *output_len);
          return buf;
        });
  });

  auto verify_async = HFN0 {
    std::shared_ptr<HmacKey> key =
        args[0].asObject(rt).getNativeState<HmacKey>(rt);
    std::string message = args[1].asString(rt).utf8(rt);
    auto tag = args[2].asObject(rt).getArrayBuffer(rt);
    auto tag_size = tag.size(rt);
    auto *data = new uint8_t[tag_size];
    memcpy(data, tag.data(rt), tag_size);

    return promisify(
        rt,
        [&key, &data, message, tag_size]() {
          auto status = aws_lc::hmac_verify(
              key.get()->hmac_key,
              reinterpret_cast<const unsigned char *>(message.c_str()),
              message.length(), data, tag_size);

          if (status != aws_lc::CHmacError::Ok) {
            return false;
          }

          return true;
        },
        [](jsi::Runtime &, std::any prev) {
          return std::any_cast<bool>(prev);
        });
  });

  base_module.setProperty(rt, "sign", std::move(sign));
  base_module.setProperty(rt, "verify", std::move(verify));
  base_module.setProperty(rt, "signAsync", std::move(sign_async));
  base_module.setProperty(rt, "verifyAsync", std::move(verify_async));

  root_module.setProperty(rt, "hmac", std::move(base_module));
}
} // namespace opawslc
