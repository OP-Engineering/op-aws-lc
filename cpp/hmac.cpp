#include "hmac.h"
#include "HmacKey.h"
#include "OPThreadPool.h"
#include "macros.h"

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

void add_hmac_module(jsi::Runtime &rt,
                     std::shared_ptr<facebook::react::CallInvoker> invoker,
                     jsi::Object &root_module) {
  auto base_module = jsi::Object(rt);

  auto generate = HOST_STATIC_FN("generate") {
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

  auto sign = HOST_STATIC_FN("sign") {
    std::shared_ptr<HmacKey> key =
        args[0].asObject(rt).getNativeState<HmacKey>(rt);
    std::string message = args[1].asString(rt).utf8(rt);
    uint8_t output_buffer[64];
    size_t output_len;
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
        array_buffer_ctor.callAsConstructor(rt, (int)output_len).getObject(rt);
    jsi::ArrayBuffer buf = o.getArrayBuffer(rt);
    memcpy(buf.data(rt), output_buffer, output_len);

    return o;
  });

  auto verify = HOST_STATIC_FN("verify") {
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

  auto sign_async = HOST_STATIC_FN("signAsync") {
    std::shared_ptr<HmacKey> key =
        args[0].asObject(rt).getNativeState<HmacKey>(rt);
    std::string message = args[1].asString(rt).utf8(rt);

    auto promiseCtr = rt.global().getPropertyAsFunction(rt, "Promise");
    auto promise = promiseCtr.callAsConstructor(
        rt, HOST_STATIC_FN("executor") {
      auto resolve = std::make_shared<jsi::Value>(rt, args[0]);
      auto reject = std::make_shared<jsi::Value>(rt, args[1]);
      auto task = [&rt, message, key, invoker, resolve, reject]() {
        uint8_t output_buffer[64];
        size_t output_len;
        const char *err;
        try {
          auto status = aws_lc::hmac_sign(
              key.get()->hmac_key,
              reinterpret_cast<const unsigned char *>(message.c_str()),
              message.length(), output_buffer, &output_len, &err);

          if (status != aws_lc::CHmacError::Ok) {
            throw std::runtime_error(err);
          }

          invoker->invokeAsync([&rt, output_buffer, output_len, resolve]() {
            jsi::Function array_buffer_ctor =
                rt.global().getPropertyAsFunction(rt, "ArrayBuffer");
            jsi::Object o =
                array_buffer_ctor.callAsConstructor(rt, (int)output_len)
                    .getObject(rt);
            jsi::ArrayBuffer buf = o.getArrayBuffer(rt);
            memcpy(buf.data(rt), output_buffer, output_len);

            resolve->asObject(rt).asFunction(rt).call(rt, std::move(o));
          });
        } catch (const std::exception &e) {
          std::string what = e.what();
          invoker->invokeAsync([&rt, what, reject]() {
            auto errorCtr = rt.global().getPropertyAsFunction(rt, "Error");
            auto error = errorCtr.callAsConstructor(
                rt, jsi::String::createFromUtf8(rt, what));
            reject->asObject(rt).asFunction(rt).call(rt, std::move(error));
          });
        }
      };

      ThreadPool::getInstance().queueWork(task);

      return {};
    }));
    return promise;
  });

  auto verify_async = HOST_STATIC_FN("verifyAsync") {
    std::shared_ptr<HmacKey> key =
        args[0].asObject(rt).getNativeState<HmacKey>(rt);
    std::string message = args[1].asString(rt).utf8(rt);
    auto tag = args[2].asObject(rt).getArrayBuffer(rt);
    auto tag_size = tag.size(rt);
    uint8_t *data = new uint8_t[tag_size];
    memcpy(data, tag.data(rt), tag_size);

    auto promiseCtr = rt.global().getPropertyAsFunction(rt, "Promise");
    auto promise = promiseCtr.callAsConstructor(
        rt, HOST_STATIC_FN("executor") {
      auto resolve = std::make_shared<jsi::Value>(rt, args[0]);
      auto reject = std::make_shared<jsi::Value>(rt, args[1]);
      auto task = [&rt, message, key, data, tag_size, invoker, resolve,
                   reject]() {
        try {
          auto status = aws_lc::hmac_verify(
              key.get()->hmac_key,
              reinterpret_cast<const unsigned char *>(message.c_str()),
              message.length(), data, tag_size);

          if (status != aws_lc::CHmacError::Ok) {
            invoker->invokeAsync([&rt, resolve]() {
              resolve->asObject(rt).asFunction(rt).call(rt, false);
            });
          } else {
            invoker->invokeAsync([&rt, resolve]() {
              resolve->asObject(rt).asFunction(rt).call(rt, true);
            });
          }
        } catch (const std::exception &e) {
          std::string what = e.what();
          invoker->invokeAsync([&rt, what, reject]() {
            auto errorCtr = rt.global().getPropertyAsFunction(rt, "Error");
            auto error = errorCtr.callAsConstructor(
                rt, jsi::String::createFromUtf8(rt, what));
            reject->asObject(rt).asFunction(rt).call(rt, std::move(error));
          });
        }
      };

      ThreadPool::getInstance().queueWork(task);

      return {};
    }));
    return promise;
  });

  base_module.setProperty(rt, "sign", std::move(sign));
  base_module.setProperty(rt, "verify", std::move(verify));
  base_module.setProperty(rt, "signAsync", std::move(sign_async));
  base_module.setProperty(rt, "verifyAsync", std::move(verify_async));

  root_module.setProperty(rt, "hmac", std::move(base_module));
}
} // namespace opawslc
