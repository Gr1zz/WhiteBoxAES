#include "include/aes.h"

#include <stdio.h>
#include <stdlib.h>

uint8_t state[STATE_SIZE] = {0x53, 0x2c, 0x3a, 0x4F,
			       0x53, 0x2c, 0x3a, 0x4F,
			     0x53, 0x2c, 0x3a, 0x4F,
			     0x53, 0x2c, 0x3a, 0x4F};

void printState () {
  for (int i = 0; i < STATE_SIZE; i++) {
    printf ("%c%x ", (i%4 == 0 ? '\n':' '), state[i]);
}

void test_shiftRows () {
  printState ();
  shiftRows (state);
  printState ();
  }
}

void test_expandKey (keySize_t keySize) {
int expandedKeySize = (keySize == SIZE_16 ? 176 : (keySize == SIZE_24 ? 208 : 240));
  uint8_t expandedKey[expandedKeySize];
uint8_t key[keySize] = {0};

  expandKey (key, keySize, expandedKey);
  printf ("exp : %d\n", expandedKeySize);
  for (int i = 0; i < expandedKeySize; i++) {
    printf ("%x ", expandedKey[i]);
    if ((i+1)%16 == 0)
      printf ("\n");
  }
  printf ("\n");
  return EXIT_SUCCESS;
}

int main (int argc, char **argv) {
  test_shiftRows ();
  return EXIT_SUCCESS;
}
