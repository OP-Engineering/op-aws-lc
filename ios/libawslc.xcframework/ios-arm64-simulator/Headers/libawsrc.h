#ifndef awsrc_h
#define awsrc_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
namespace awsrc {
#endif  // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int32_t generate_hmac_sha256_key(void **key_ptr);

Key *retake_key(void *raw_ptr);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#ifdef __cplusplus
}  // namespace awsrc
#endif  // __cplusplus

#endif  /* awsrc_h */
