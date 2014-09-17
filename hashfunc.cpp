#include "hashfunc.h"


/* 32 bit magic FNV-0 and FNV-1 prime */
#define FNV_32_PRIME ((Hash_Size_t)0x01000193)

Hash_Size_t fnv32(char *str, Hash_Size_t hval)
{
    unsigned char *s = (unsigned char *)str;	/* unsigned string */

    /*
     * FNV-1 hash each octet in the buffer
     */
    while (*s) {

	/* multiply by the 32 bit FNV magic prime mod 2^32 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
	hval *= FNV_32_PRIME;
#else
	hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
#endif

	/* xor the bottom with the current octet */
	hval ^= (Hash_Size_t)*s++;
    }

    /* return our new hash value */
    return hval;
}

Hash_Size_t fnv32a(char *str, Hash_Size_t hval)
{
    unsigned char *s = (unsigned char *)str;	/* unsigned string */

    /*
     * FNV-1a hash each octet in the buffer
     */
    while (*s) {

	/* xor the bottom with the current octet */
	hval ^= (Hash_Size_t)*s++;

	/* multiply by the 32 bit FNV magic prime mod 2^32 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
	hval *= FNV_32_PRIME;
#else
	hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
#endif
    }

    /* return our new hash value */
    return hval;
}

Hash_Size_t RSHash(char *str)
{
   Hash_Size_t b    = 378551;
   Hash_Size_t a    = 63689;
   Hash_Size_t hash = 0;
   Hash_Size_t i;
   
   for(i = 0; i<strlen(str); i++)
   {
      hash = hash * a + str[i];
      a    = a * b;
   }

   return (hash);
}
