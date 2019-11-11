#ifndef XCORTEX_VERIFY_H
#define XCORTEX_VERIFY_H

#include <stdint.h>
#include <vector>
#ifdef __cplusplus
extern "C"
{
#endif
bool verify(uint32_t nonce, std::vector<uint8_t> header, const int8_t *hash);
#ifdef __cplusplus
}
#endif

#endif
