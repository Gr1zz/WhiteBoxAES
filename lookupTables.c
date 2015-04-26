#include "include/aes.h"

#include <stdio.h>
#include <stdlib.h>

static uint8_t tBoxes[10][16][256];
static uint8_t yBoxes[4][4][255];
static uint8_t tyBoxes[9][16][256];

void generate_tables (uint8_t *expandedKey) {
  uint8_t roundKeys[11][STATE_SIZE];
  uint8_t roundKeys2[11][STATE_SIZE];
  uint8_t mixPoly[4] = {0x01, 0x01, 0x03, 0x02};

  createRoundKeys (expandedKey, roundKeys);

  for (int i = 0; i < 10; i++) {
    fprintf (stderr, "key#%d ", i);
    for (int j = 0; j < 16; j++) {
      fprintf (stderr, "%x ", roundKeys[i][j]);
    }
    fprintf (stderr, "\n");
  }
  fprintf (stderr, "static uint8_t tBoxes[10][16][256] = \n{\n");

  for (int i = 0; i < 10; i++) {
    fprintf (stderr, "\t{\n");
    invertMatrix (roundKeys2[i], roundKeys[i]);
    shiftRows (roundKeys2[i]);
    fprintf (stderr, "shiftkey : ");

    for (int j = 0; j < 16; j++) {
      fprintf (stderr, "%x ", roundKeys2[i][j]);
    }
    fprintf (stderr, "\n");
    for (int j = 0; j < 16; j++) {
      fprintf (stderr, "\t\t{\n\t\t\t");;
      for (int k = 0; k < 256; k++) {
	if ((k%16 == 0) && (k != 0)) {
	  fprintf (stderr, "\n\t\t\t");
	}
	tBoxes[i][j][k] = getSBoxValue(k ^ roundKeys2[i][j]);
	if (i == 9) {
	  invertMatrix (roundKeys2[10], roundKeys[10]);
	  tBoxes[i][j][k] ^= roundKeys2[10][j];
	}
	fprintf (stderr, "0x%02x%s", tBoxes[i][j][k], (k < 255) ? ", ":"");
      }
      fprintf (stderr, "\n\t\t}%s\n", (j < 15) ? ",": "");
    }
    fprintf (stderr, "\t}%s\n", (i < 9) ? "," : "");
  }
  fprintf (stderr, "};\n\n");
  /*for (int i = 3; i >= 0; i--) {
    for (int j = 0; j < 4; j++) {
    for (int k = 0; k < 256; k++) {
    yBoxes[i][j][k] = gMul (k, mixPoly[j]);
    }
    }
    rotate (mixPoly);
    }*/
}
void aes_table_main (uint8_t state[STATE_SIZE]) {
  for (int i = 0; i < 9; i++) {
    shiftRows (state);
    for (int j = 0; j < 16; j++) {
      uint8_t tmp = tBoxes[i][j][state[j]];
      state[j] = tmp;
    }
    mixColumns (state);
  }
  shiftRows (state);

  for (int j = 0; j < 16; j++) {
    uint8_t tmp = tBoxes[9][j][state[j]];
    state[j] = tmp;
  }

}

void aes_table_encrypt (uint8_t input[STATE_SIZE], uint8_t output[STATE_SIZE], uint8_t *inputKey,
			keySize_t keySize) {
  int expandedKeySize = (keySize == SIZE_16 ? 176 : (keySize == SIZE_24 ? 208 : 240));
  uint8_t expandedKey[expandedKeySize];
  uint8_t state[STATE_SIZE];
  uint8_t key[16];
  invertMatrix (state, input);
  expandKey (inputKey, keySize, expandedKey);
  generate_tables (expandedKey);
  aes_table_main (state);
  invertMatrix (output, state);
}
/* P1 = 014BAF2278A69D331D5180103643E99A
 * K1 = E8E9EAEBEDEEEFF0F2F3F4F5F7F8F9FA
 * C1 = 6743C3D1519AB4F2CD9A78AB09A511BD
 *
 * P2 = 7768617464757020666f6c6b733f3f3f
 * K2 = 30766572636c306b40646f6172652d65
 * C2 = eeee83f6e41c7a2ac7c9b60ebc131a57
 */
int main (int argc, char **argv) {
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
  for (int i = 0; i < STATE_SIZE; i++) {
    printf ("%c%x ", (i%4 == 0 ? '\n':' '), input[i]);
  }
  printf("\n");
  aes_table_encrypt (input, output, key, SIZE_16);
  for (int i = 0; i < STATE_SIZE; i++) {
    printf ("%c%x ", (i%4 == 0 ? '\n':' '), output[i]);
  }
  printf("\n");
  return EXIT_SUCCESS;
}
