#include "aes.h"

u8 TBoxes[10][16][256];
u32 TyiTables[4][256];
u32 TyiBoxes[9][16][256];
u8 xor[16][16];

typedef union {
  u8 bytes[4];
  u32 all;
} tyi;

void computeTables (u8 expandedKey[176])
{
  for (int j = 0; j < 4; j++)
  {
    for (int x = 0; x < 256; x++)
    {
      tyi tmp;
      TyiTables[0][x] = (gMul(2, x) << 24) | (x << 16) | (x << 8) | gMul(3, x);
      TyiTables[1][x] = (gMul(3, x) << 24) | (gMul(2, x) << 16) | (x << 8) | x;
      TyiTables[2][x] = (x << 24) | (gMul(3, x) << 16) | (gMul(2, x) << 8) | x;
      TyiTables[3][x] = (x << 24) | (x << 16) | (gMul(3, x) << 8) | gMul(2, x);
    }
  }

  for (int i = 0; i < 10; i++)
  {
    shiftRows (expandedKey + 16*i);

    for (int j = 0; j < 16; j++)
    {
      for (int x = 0; x < 256; x++)
      {
        u32 tmp = SBox[x ^ expandedKey[16 * i + j]];
        TBoxes[i][j][x] = tmp;

        if (i == 9)
        {
          TBoxes[i][j][x] ^= expandedKey[160 + j];
        }
        else
        {
          TyiBoxes[i][j][x] = TyiTables[j % 4][tmp];
        }
      }
    }
  }

  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      xor[i][j] = i ^ j;
    }
  }
}

void printTables()
{
  FILE *tables = fopen ("tables.h", "w");
  fprintf(tables, "unsigned char TBoxes[10][16][256] =\n{\n\t");
  for (int i = 0; i < 10; i++)
  {
    fprintf(tables, "{\n\t\t");
    for (int j = 0; j < 16; j++)
    {
      fprintf(tables, "{\n\t\t\t");
      for (int x = 0; x < 256; x++)
      {
        fprintf(tables, "0x%.2x%s", TBoxes[i][j][x], (x != 255) ? ", " : "");
      }
      fprintf(tables, "\n\t\t}%s\n\t\t", (j != 15) ? ", " : "");
    }
    fprintf(tables, "}%s\n", (i != 9) ? ", " : "");
  }
  fprintf (tables, "};\n");

  fprintf(tables, "unsigned int TyiTables[4][256] =\n{\n\t");
  for (int j = 0; j < 4; j++)
  {
    fprintf(tables, "{\n\t\t\t");
    for (int x = 0; x < 256; x++)
    {
      fprintf(tables, "0x%.8x%s", TyiTables[j][x], (x != 255) ? ", " : "");
    }
    fprintf(tables, "\n\t\t}%s\n\t\t", (j != 3) ? ", " : "");
  }
  fprintf(tables, "};\n");

  fprintf(tables, "unsigned int TyiBoxes[9][16][256] =\n{\n\t");
  for (int i = 0; i < 9; i++)
  {
    fprintf(tables, "{\n\t\t");
    for (int j = 0; j < 16; j++)
    {
      fprintf(tables, "{\n\t\t\t");
      for (int x = 0; x < 256; x++)
      {
        fprintf(tables, "0x%.8x%s", TyiBoxes[i][j][x], (x != 255) ? ", " : "");
      }
      fprintf(tables, "\n\t\t}%s\n\t\t", (j != 15) ? ", " : "");
    }
    fprintf(tables, "}%s\n", (i != 8) ? ", " : "");
  }
  fprintf (tables, "};\n");

  fprintf(tables, "unsigned char xor[16][16] =\n{\n\t");
  for (int i = 0; i < 16; i++)
  {
    fprintf(tables, "{\n\t\t\t");
    for (int j = 0; j < 16; j++)
    {
      fprintf(tables, "0x%.1x%s", xor[i][j], (j != 15) ? ", " : "");
    }
    fprintf(tables, "\n\t\t}%s\n\t\t", (i != 15) ? ", " : "");
  }
  fprintf(tables, "};\n");

  fclose (tables);
}

int main(void) {
  u8 expandedKey[176];
  u8 key[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  //u8 key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  //u8 key[16] = "0vercl0k@doare-e";
  u8 rk[16];

  printf("Generating tables.h...\n");
  expandKey (key, expandedKey);
  computeTables(expandedKey);
  printTables();
  printf("Done!\n");

  return 0;
}