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


uint8_t getSBoxValue (uint8_t num);
uint8_t getRconValue (uint8_t num);
void rotate (uint8_t *input);
void keyScheduleCore (uint8_t *input, int iteration);
void expandKey (uint8_t *key, keySize_t keySize, uint8_t *expandedKey);
void subBytes (uint8_t state[STATE_SIZE]);
void shiftRows (uint8_t state[STATE_SIZE]);
uint8_t gMul (uint8_t a, uint8_t b);
void mixColumn (uint8_t column[4]);
void mixColumns (uint8_t state[STATE_SIZE]);
void createRoundKeys (uint8_t  *expandedKey, uint8_t roundKeys[10][STATE_SIZE]);
void addRoundKey (uint8_t state[STATE_SIZE], uint8_t roundKey[STATE_SIZE]);
void expandKey (uint8_t *key, keySize_t keySize, uint8_t *expandedKey);
void invertMatrix (uint8_t out[STATE_SIZE], uint8_t in[STATE_SIZE]);
void aes_encrypt (uint8_t input[STATE_SIZE], uint8_t output[STATE_SIZE],
		  uint8_t *key, keySize_t keySize);
void shiftRows (uint8_t state[STATE_SIZE]);
#endif
