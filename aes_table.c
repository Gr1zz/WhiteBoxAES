#include "aes.h"
#include "tables.h"

void aes_128_table_encrypt (u8 input[16], u8 output[16]) {
  u8 expandedKey[176];
  u8 key[16] = "Thats my Kung Fu";

  expandKey (key, expandedKey);

  for (int i = 0; i < 9; i++) {
    shiftRows (input); 
    shiftRows (expandedKey + 16*i);

    for (int j = 0; j < 16; j++) {
      input[j] = tboxes[i][j][input[j]];
    }
    mixColumns (input);  
  }
  
  for (int j = 0; j < 16; j++) {
    input[j] = tboxes[9][j][input[j]];
  }
  for (int i = 0; i < 16; i++)
    output[i] = input[i];

}

int main(void) {
  u32 lol;
  u8 out[16];
  u8 in[16] = "Two One Nine Two";

  printState(in);
  aes_128_table_encrypt(in, out);
  printState(out);
  return 0;
}