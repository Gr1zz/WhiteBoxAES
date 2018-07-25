extern "C" {
  #include "aes.h"
}
#include "RandMat.h"

u8 TBoxes[10][16][256];
u32 TyiTables[4][256];
u32 TyiBoxes[9][16][256];
u32 mixBijIn[10][16][256];
u32 mixBijOut[10][16][256];
u8 xorTable[9][96][16][16];

typedef union {
  u8 bytes[4];
  u32 all;
} tyi;

vec_GF2 int2vec (u32 iIn, int size)
{
    vec_GF2 vOut;
    vOut.SetLength(size);
    for (int i = 0; i < size; i++)
    {
        if ((iIn >> i) % 2 == 1)
        {
            vOut[size-1 - i] = 1;
        }
        else
        {
            vOut[size-1 - i] = 0;
        }
    }
  return vOut;
}

u32 vec2int (vec_GF2 vIn, int size)
{
    u32 iOut = 0;
    for (int i = 0; i < size; i++)
    {
        iOut *= 2;
        if (vIn[i] == 1)
            iOut += 1;
    }
    return iOut;
}

void computeTables (u8 expandedKey[176])
{
  // Tyi tables implement MixColumns
  for (int j = 0; j < 4; j++)
  {
    for (int x = 0; x < 256; x++)
    {
      TyiTables[0][x] = (gMul(2, x) << 24) | (x << 16) | (x << 8) | gMul(3, x);
      TyiTables[1][x] = (gMul(3, x) << 24) | (gMul(2, x) << 16) | (x << 8) | x;
      TyiTables[2][x] = (x << 24) | (gMul(3, x) << 16) | (gMul(2, x) << 8) | x;
      TyiTables[3][x] = (x << 24) | (x << 16) | (gMul(3, x) << 8) | gMul(2, x);
    }
  }

  vec_GF2 vAA, vBB, vCC, vDD;
  u32 aa, bb, cc, dd;
  RandMat mb_8x8[10][16], mb_32x32[10][4];
 
  // Initialize random mixing bijections
  for (int i = 0; i < 10; i++)
  {
    for (int k = 0; k < 4; k++) {
      mb_32x32[i][k].init(32);
    }

    for (int k = 0; k < 16; k++) {
      mb_8x8[i][k].init(8);
    }
  }
  // Compute tboxes and compose them at their output with Tyi tables
  for (int i = 0; i < 10; i++)
  {
    shiftRows (expandedKey + 16*i);

    for (int j = 0; j < 16; j++)
    {
      for (int x = 0; x < 256; x++)
      {
        mixBijIn[i][j][x] = vec2int(mb_8x8[i][j].mat * int2vec(x, 8), 8);
      }
      for (int x = 0; x < 256; x++)
      {
        u32 tmp = x;
        // Input mixing bijection
        if ((i > 0) && (i < 9)) { 
          tmp = vec2int(mb_8x8[i-1][j].invMat * int2vec(x, 8), 8);       
        }
        
        if ((i > 0) && (i < 9))
        {
          tmp = mixBijIn[i-1][j][tmp];
        }
        tmp = SBox[tmp ^ expandedKey[16 * i + j]];
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

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 4; j++)
    {
        for (int x = 0; x < 256; x++)
        {
          if (i < 9)
          {
            mixBijIn[i][4*j + 0][x] = vec2int(mb_8x8[i][4*j + 0].mat * int2vec(x, 8), 8);
            mixBijIn[i][4*j + 1][x] = vec2int(mb_8x8[i][4*j + 1].mat * int2vec(x, 8), 8);
            mixBijIn[i][4*j + 2][x] = vec2int(mb_8x8[i][4*j + 2].mat * int2vec(x, 8), 8);
            mixBijIn[i][4*j + 3][x] = vec2int(mb_8x8[i][4*j + 3].mat * int2vec(x, 8), 8);

            mixBijIn[i][4*j + 0][x] = x;
            mixBijIn[i][4*j + 1][x] = x;
            mixBijIn[i][4*j + 2][x] = x;
            mixBijIn[i][4*j + 3][x] = x;
          }
        }

        for (int x = 0; x < 256; x++)
        {
          if (i < 9)
          {
            vAA = int2vec(TyiBoxes[i][4*j + 0][x], 32);
            vBB = int2vec(TyiBoxes[i][4*j + 1][x], 32);
            vCC = int2vec(TyiBoxes[i][4*j + 2][x], 32);
            vDD = int2vec(TyiBoxes[i][4*j + 3][x], 32);

            TyiBoxes[i][4*j + 0][x] = vec2int(mb_32x32[i][j].mat * vAA, 32);
            TyiBoxes[i][4*j + 1][x] = vec2int(mb_32x32[i][j].mat * vBB, 32);
            TyiBoxes[i][4*j + 2][x] = vec2int(mb_32x32[i][j].mat * vCC, 32);
            TyiBoxes[i][4*j + 3][x] = vec2int(mb_32x32[i][j].mat * vDD, 32);        

            vAA = int2vec(x << 24, 32);
            vBB = int2vec(x << 16, 32);
            vCC = int2vec(x << 8, 32);
            vDD = int2vec(x << 0, 32);

            aa = vec2int(mb_32x32[i][j].invMat * vAA, 32);
            bb = vec2int(mb_32x32[i][j].invMat * vBB, 32);
            cc = vec2int(mb_32x32[i][j].invMat * vCC, 32);
            dd = vec2int(mb_32x32[i][j].invMat * vDD, 32);

            mixBijOut[i][4*j + 0][x] = (mixBijIn[i][4*j + 0][(aa >> 24) & 0xff] << 24)
                                     | (mixBijIn[i][4*j + 1][(aa >> 16) & 0xff] << 16)
                                     | (mixBijIn[i][4*j + 2][(aa >>  8) & 0xff] <<  8)
                                     | (mixBijIn[i][4*j + 3][(aa >>  0) & 0xff] <<  0);
            mixBijOut[i][4*j + 1][x] = (mixBijIn[i][4*j + 0][(bb >> 24) & 0xff] << 24)
                                     | (mixBijIn[i][4*j + 1][(bb >> 16) & 0xff] << 16)
                                     | (mixBijIn[i][4*j + 2][(bb >>  8) & 0xff] <<  8)
                                     | (mixBijIn[i][4*j + 3][(bb >>  0) & 0xff] <<  0);
            mixBijOut[i][4*j + 2][x] = (mixBijIn[i][4*j + 0][(cc >> 24) & 0xff] << 24)
                                     | (mixBijIn[i][4*j + 1][(cc >> 16) & 0xff] << 16)
                                     | (mixBijIn[i][4*j + 2][(cc >>  8) & 0xff] <<  8)
                                     | (mixBijIn[i][4*j + 3][(cc >>  0) & 0xff] <<  0);                                 
            mixBijOut[i][4*j + 3][x] = (mixBijIn[i][4*j + 0][(dd >> 24) & 0xff] << 24)
                                     | (mixBijIn[i][4*j + 1][(dd >> 16) & 0xff] << 16)
                                     | (mixBijIn[i][4*j + 2][(dd >>  8) & 0xff] <<  8)
                                     | (mixBijIn[i][4*j + 3][(dd >>  0) & 0xff] <<  0);
          }
        }
      
    }
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 96; j++)
    {
      for (int x = 0; x < 16; x++)
      {
        for (int y = 0; y < 16; y++)
        {
          xorTable[i][j][x][y] = x ^ y;
        }
      }
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

  fprintf(tables, "unsigned int mixBijOut[10][16][256] =\n{\n\t");
  for (int i = 0; i < 10; i++)
  {
    fprintf(tables, "{\n\t\t");
    for (int j = 0; j < 16; j++)
    {
      fprintf(tables, "{\n\t\t\t");
      for (int x = 0; x < 256; x++)
      {
        fprintf(tables, "0x%.8x%s", mixBijOut[i][j][x], (x != 255) ? ", " : "");
      }
      fprintf(tables, "\n\t\t}%s\n\t\t", (j != 15) ? ", " : "");
    }
    fprintf(tables, "}%s\n", (i != 9) ? ", " : "");
  }
  fprintf (tables, "};\n");

  fprintf(tables, "unsigned char xorTable[9][96][16][16] =\n{\n\t");
  for (int i = 0; i < 9; i++)
  {
    fprintf(tables, "{\n\t\t");
    for (int j = 0; j < 96; j++)
    {
      fprintf(tables, "{\n\t\t");
      for (int x = 0; x < 16; x++)
      {
        fprintf(tables, "{\n\t\t\t");
        for (int y = 0; y < 16; y++)
        {
          fprintf(tables, "0x%.1x%s", xorTable[i][j][x][y], (y != 15) ? ", " : "");
        }
        fprintf(tables, "\n\t\t}%s\n\t\t", (x != 15) ? ", " : "");
      }
      fprintf(tables, "\n\t\t}%s\n\t\t", (j != 95) ? ", " : "");
    }
    fprintf(tables, "}%s\n", (i != 8) ? ", " : "");
  }
  fprintf(tables, "};\n");

  fclose (tables);
}



int main(void) {
  u8 expandedKey[176];
  u8 key[16] = {0};
  
  printf("Generating tables.h...\n");
  expandKey (key, expandedKey);
  computeTables(expandedKey);
  printTables();
  printf("Done!\n");

  return 0;
}