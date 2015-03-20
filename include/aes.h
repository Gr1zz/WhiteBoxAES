#ifndef H_AES
#define H_AES

#include <inttypes.h>

/* Size of the input word in bytes */
#define INPUT_SIZE 4
/* Number of 32 bit words in the cipher key */
//#define NK 4
/* Number of 8 bits blocks in the state */
#define STATE_SIZE 16

typedef enum {
  SIZE_16 = 16,
  SIZE_24 = 24,
  SIZE_32 = 32
} keySize_t;

void expandKey (uint8_t *key, keySize_t keySize, uint8_t *expandedKey);
void aes_encrypt (uint8_t input[STATE_SIZE], uint8_t output[STATE_SIZE], uint8_t *key,
		  keySize_t keySize);
void shiftRows (uint8_t state[STATE_SIZE]);
#endif
