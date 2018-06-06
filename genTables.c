#include "aes.h"

void genTables () {
  u8 expandedKey[176];
  u8 key[16] = "Thats my Kung Fu";
  u8 rk[16];

  FILE *tables = fopen ("tables.h", "w");
  expandKey (key, expandedKey);

  fprintf(tables, "unsigned char tbox[10][16][255] =\n{\n\t");
  for (int i = 1; i < 10; i++) {
    shiftRows (expandedKey + 16 * (i-1));

    fprintf(tables, "{\n\t\t");

    for (int j = 0; j < 16; j++) {
      fprintf(tables, "{\n\t\t\t");
      for (u8 k = 0; k < 0xff; k++) {
        fprintf(tables, "0x%.2x,", sBox[k ^ expandedKey[16 * (i-1) + j]]);
      }
      fprintf(tables, "\n\t\t},\n\t\t");
    }
    fprintf(tables, "},\n");
  }
  fprintf (tables, "};");
  fclose (tables);
}

int main(void) {
  // your code goes here
  u32 lol;
  u8 out[16];
  u8 in[16] = "Two One Nine Two";

  printState(in);
  genTables();
  aes_128_encrypt(in, out);
  printState(out);


  return 0;
}