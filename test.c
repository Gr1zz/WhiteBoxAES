#include "include/aes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t state[STATE_SIZE] = {0x0e, 0xf9, 0x03, 0x33,
			     0x3b, 0xa9, 0x61, 0x38,
			     0x97, 0x06, 0x0a, 0x04,
			     0x51, 0x1d, 0xfa, 0x9f};
uint8_t state2[STATE_SIZE] = {0x53, 0x2c, 0x3a, 0x4F,
			     0x53, 0x2c, 0x3a, 0x4F,
			     0x53, 0x2c, 0x3a, 0x4F,
			     0x53, 0x2c, 0x3a, 0x4F};

static void printState () {
  for (int i = 0; i < STATE_SIZE; i++) {
    printf ("%c%x ", (i%4 == 0 ? '\n':' '), state[i]);
  }
  printf("\n");
}

// e f9 3 33 a9 61 38 3b a 4 97 6 9f 51 1d fa
static void test_shiftRows () {
  printState ();
  shiftRows (state);
  printState ();
}

static void test_expandKey (keySize_t keySize) {
  // 2b7e151628aed2a6abf7158809cf4f3c
  uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16,
		     0x28, 0xae, 0xd2, 0xa6,
		     0xab, 0xf7, 0x15, 0x88,
		     0x09, 0xcf, 0x4f, 0x3c };
  int expandedKeySize = (keySize == SIZE_16 ? 176 : (keySize == SIZE_24 ? 208 : 240));
  uint8_t expandedKey[expandedKeySize];

  expandKey (key, keySize, expandedKey);
  printf ("exp : %d\n", expandedKeySize);
  for (int i = 0; i < expandedKeySize; i++) {
    printf ("%x ", expandedKey[i]);
    if ((i+1)%16 == 0)
      printf ("\n");
  }
  printf ("\n");
}

static void test_gMul () {
  uint8_t a = 0x53;
  uint8_t b = 0xCA;
  printf ("%xx%x=%x\n", a, b, gMul (a, b));
  printf ("%xx%x=%x\n", a, b, gMul (a, b));
}

static void test_mixColumn () {
  uint8_t column[4] = {0xdb, 0x13, 0x53, 0x45};

  for (int i = 0; i < 4; i++)
    printf ("%x ", column[i]);

  printf ("\n");
  mixColumn (column);

  for (int i = 0; i < 4; i++)
    printf ("%x ", column[i]);
}
static void print_state (uint8_t *state) {
  for (int i = 0; i < STATE_SIZE; i++)
    printf ("%x%c", state[i], (i%4 == 3) ? '\n':' ');
}
static void test_mixColumns () {
  uint8_t state[STATE_SIZE] = {0xdb, 0xf2, 0x01,0xd4,
			       0x13, 0x0a, 0x01, 0xd4,
			       0x53, 0x22, 0x01, 0xd4,
			       0x45, 0x5c, 0x01, 0xd5 };

  print_state (state);
  mixColumns (state);
  printf ("\n");
  print_state (state);
}

/* P1 = 014BAF2278A69D331D5180103643E99A
 * K1 = E8E9EAEBEDEEEFF0F2F3F4F5F7F8F9FA
 * C1 = 6743C3D1519AB4F2CD9A78AB09A511BD
 *
 * P2 = 7768617464757020666f6c6b733f3f3f
 * K2 = 30766572636c306b40646f6172652d65
 * C2 = eeee83f6e41c7a2ac7c9b60ebc131a57
 */
static void test_aes_encrypt () {
  uint8_t key3[STATE_SIZE] = {0x30, 0x76, 0x65, 0x72,
			     0x63, 0x6c, 0x30, 0x6b,
			     0x40, 0x64, 0x6f, 0x61,
			     0x72, 0x65, 0x2d, 0x65};
  uint8_t key2[STATE_SIZE] = {0xE8, 0xE9, 0xEA, 0xEB,
			      0xED, 0xEE, 0xEF, 0xF0,
			      0xF2, 0xF3, 0xF4, 0xF5,
			      0xF7, 0xF8, 0xF9, 0xFA};
  uint8_t key[STATE_SIZE] = {0x2b, 0x7e, 0x15, 0x16,
			     0x28, 0xae, 0xd2, 0xa6,
			     0xab, 0xf7, 0x15, 0x88,
			     0x09, 0xcf, 0x4f, 0x3c};
  uint8_t input3[STATE_SIZE] = {0x77, 0x68, 0x61, 0x74,
			       0x64, 0x75, 0x70, 0x20,
			       0x66, 0x6f, 0x6c, 0x6b,
			       0x73, 0x3f, 0x3f, 0x3f};
  uint8_t input2[STATE_SIZE] = {0x01, 0x4B, 0xAF, 0x22,
				0x78, 0xA6, 0x9D, 0x33,
				0x1D, 0x51, 0x80, 0x10,
				0x36, 0x43, 0xE9, 0x9A};
  uint8_t input[STATE_SIZE] = {0x32, 0x43, 0xf6, 0xa8,
				0x88, 0x5a, 0x30, 0x8d,
				0x31, 0x31, 0x98, 0xa2,
				0xe0, 0x37, 0x07, 0x34};

  uint8_t output[STATE_SIZE];
  print_state (input);
  aes_encrypt (input, output, key, SIZE_16);
  printf ("\n");
  print_state (output);
}
int main (int argc, char **argv) {

  //test_shiftRows ();
  //test_expandKey (SIZE_16);

  test_aes_encrypt ();
  return EXIT_SUCCESS;
}
