#if !defined(__FNV_H__)
#define __FNV_H__

#include <string.h>

#define USE_FNV
typedef unsigned long Hash_Size_t;

#define FNV0_32_INIT ((Hash_Size_t)0)

#define FNV1_32_INIT ((Hash_Size_t)0x811c9dc5)
#define FNV1_32A_INIT FNV1_32_INIT

extern Hash_Size_t fnv32(char *buf, Hash_Size_t hashval);
extern Hash_Size_t fnv32a(char *buf, Hash_Size_t hashval);
extern Hash_Size_t RSHash(char *str);

#endif /* __FNV_H__ */
