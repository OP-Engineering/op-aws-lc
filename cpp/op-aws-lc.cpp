#include "op-aws-lc.h"
#include "macros.h"
#include "librn_aws_lc.h"
#include <iomanip>
#include <iostream>

namespace opawslc {
namespace jsi = facebook::jsi;
namespace react = facebook::react;

void install(jsi::Runtime &rt,
             std::shared_ptr<react::CallInvoker> invoker) {

  auto hmac = HOST_STATIC_FN("makeHmac") {
    awsrc::CHmacKeyHandle *key = awsrc::generate_hmac_key(awsrc::CHmacAlgorithm::SHA256);
    
    const char* message = "Hello, this is a test message to be signed";
    const size_t message_len = strlen(message);
    unsigned char tag_buffer[64];
    size_t tag_len;

    awsrc::CHmacError result = awsrc::hmac_sign(
        key,
        reinterpret_cast<const unsigned char*>(message),
        message_len,
        tag_buffer,
        &tag_len
    );
    
    std::string tag_hex = "";
    if (result == awsrc::CHmacError::Success) {
        // Convert tag to hex string for display
      std::stringstream ss;
      for (size_t i = 0; i < tag_len; ++i) {
          ss << std::hex << std::setw(2) << std::setfill('0')
             << static_cast<int>(tag_buffer[i]);
      }
      tag_hex = ss.str();
      std::cout << "HMAC Tag (hex): " << tag_hex << std::endl;
      awsrc::CHmacError verifyResult = awsrc::hmac_verify(key,
                                                    reinterpret_cast<const unsigned char*>(message),
                                                    message_len,
                                                    tag_buffer,
                                                    tag_len);
      if (verifyResult == awsrc::CHmacError::Success){
        std::cout << "Message verified" << std::endl;
      }
      char tampered_message[100];
      strcpy(tampered_message, message);
      tampered_message[0] = 'h';  // Change first letter
      awsrc::CHmacError verifyTamperedResult = awsrc::hmac_verify(key,
                                                    reinterpret_cast<const unsigned char*>(tampered_message),
                                                    message_len,
                                                    tag_buffer,
                                                    tag_len);
      if (verifyTamperedResult != awsrc::CHmacError::Success){
        std::cout << "Tampered message verify message success" << std::endl;
      } else {
        std::cout << "Tamper verification failed" << std::endl;
      }
    } else {
        std::cerr << "HMAC signing failed with error code: " << static_cast<int>(result) << std::endl;
    }
    awsrc::hmac_free(key);
    return jsi::String::createFromUtf8(rt, tag_hex);
  });

  jsi::Object proxy = jsi::Object(rt);
  proxy.setProperty(rt, "hmac", std::move(hmac));
  rt.global().setProperty(rt, "__OPAwsLcProxy", std::move(proxy));
}
} // namespace opawslc
