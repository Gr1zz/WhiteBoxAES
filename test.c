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
  uint8_t key[keySize];
  int expandedKeySize = (keySize == SIZE_16 ? 176 : (keySize == SIZE_24 ? 208 : 240));
  uint8_t expandedKey[expandedKeySize];

  for (int i = 0; i < keySize; i++) {
    key[i] = 0x00;
  }
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
static void test_aes_encrypt () {
  uint8_t key[16] = {0x00, 0x00, 0x00, 0x00,
		     0x00, 0x00, 0x00, 0x00,
		     0x00, 0x00, 0x00, 0x00,
		     0x00, 0x00, 0x00, 0x00 };
  uint8_t input[STATE_SIZE] = {0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00 };
  uint8_t input2[STATE_SIZE] = {0xf3, 0x44, 0x81, 0xec,
			       0x3c, 0xc6, 0x27, 0xba,
			       0xcd, 0x5d, 0xc3, 0xfb,
			       0x08, 0xf2, 0x73, 0xe6};
  uint8_t output[STATE_SIZE];
  print_state (input);
  aes_encrypt (input, output, key, 16);
  print_state (input);
}
int main (int argc, char **argv) {

  //test_shiftRows ();
  //test_expandKey (SIZE_16);

  test_aes_encrypt ();
  return EXIT_SUCCESS;
}
