#include "aes.h"

unsigned char tboxes[10][16][255];

void compute_tboxes (u8 expandedKey[176]) {
  for (int i = 0; i < 9; i++) {
    shiftRows (expandedKey + 16*i);
    for (int j = 0; j < 16; j++) {
      for (u8 k = 0; k < 255; k++) {
        tboxes[i][j][k] = sBox[k ^ expandedKey[16 * i + j]];
      }
    }
  }
  shiftRows (expandedKey + 144);
  for (int j = 0; j < 16; j++) {
    for (u8 k = 0; k < 255; k++) {
      tboxes[9][j][k] = sBox[k ^ expandedKey[144 + j]] ^ expandedKey[160 + j];
    }
  }
}

void print_tboxes() {
  FILE *tables = fopen ("tables.h", "w");
  fprintf(tables, "unsigned char tboxes[10][16][255] =\n{\n\t");
  for (int i = 0; i < 10; i++) {
    fprintf(tables, "{\n\t\t");
    for (int j = 0; j < 16; j++) {
      fprintf(tables, "{\n\t\t\t");
      for (u8 k = 0; k < 255; k++) {
        fprintf(tables, "0x%.2x%s", tboxes[i][j][k], (k != 254) ? ", " : "");
      }
      fprintf(tables, "\n\t\t}%s\n\t\t", (j != 15) ? ", " : "");
    }
    fprintf(tables, "}%s\n", (i != 9) ? ", " : "");
  }
  fprintf (tables, "};");
  fclose (tables);
}

void genTables () {
  u8 expandedKey[176];
  u8 key[16] = "Thats my Kung Fu";
  u8 rk[16];

  expandKey (key, expandedKey);
  compute_tboxes(expandedKey);
  print_tboxes();
}

int main(void) {
  printf("Generating tables...\n");
  genTables();
  printf("Done!\n");

  return 0;
}