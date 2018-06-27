#include "aes.h"
#include "tables.h"

void mixColumns_table2(u8 state[16]) {
  u8 out[16];
  u32 tmp;
    for (int j = 0; j < 4; j++)
    {
      tmp = TyiTables[0][state[4*j]] ^ TyiTables[1][state[4*j + 1]]
      ^ TyiTables[2][state[4*j + 2]] ^ TyiTables[3][state[4*j + 3]];
      out[4*j + 0] = (u8) (tmp >> 24);
      out[4*j + 1] = (u8) (tmp >> 16);
      out[4*j + 2] = (u8) (tmp >> 8);
      out[4*j + 3] = (u8) (tmp >> 0);
    }

  memcpy(state, out, sizeof(out));
}

void mixColumns_table(u8 state[16]) {
  u8 out[16];
  u32 tmp, a, b, c, d, aa, bb, cc, dd;
    for (int j = 0; j < 4; j++)
    {
      a = TyiTables[0][state[4*j]];
      b = TyiTables[1][state[4*j + 1]];
      c = TyiTables[2][state[4*j + 2]];
      d = TyiTables[3][state[4*j + 3]];
      aa = ((a >> 16) & 0xffff) ^ ((b >> 16) & 0xffff)
         | ((a >>  0) & 0xffff) ^ ((b >>  0) & 0xffff);
      bb = c ^ d;
      cc = aa ^ bb;
      out[4*j + 0] = (u8) (cc >> 24);
      out[4*j + 1] = (u8) (cc >> 16);
      out[4*j + 2] = (u8) (cc >> 8);
      out[4*j + 3] = (u8) (cc >> 0);
      //printf("%.2X %.2X %.2X %.2X\n", out[j], out[j+1], out[j+2], out[j+3]);
    }

  memcpy(state, out, sizeof(out));
}

void aes_128_table_encrypt (u8 input[16], u8 output[16]) {
  u32 a, b, c, d;
  u8 tmp[16];
  for (int i = 0; i < 9; i++) {
    shiftRows (input); 

    /*for (int j = 0; j < 16; j++) {
      tmp[j] = TBoxes[i][j][input[j]];
    }
    if (i == 1)
      mixColumns_table2 (tmp);*/


    for (int j = 0; j < 4; j++)
    {
      a = TyiBoxes[i][4*j + 0][input[4*j + 0]];
      b = TyiBoxes[i][4*j + 1][input[4*j + 1]];
      c = TyiBoxes[i][4*j + 2][input[4*j + 2]];
      d = TyiBoxes[i][4*j + 3][input[4*j + 3]];

      input[4*j + 3] = (xor[xor[(a >> 4) & 0xf][(b >> 4) & 0xf]][xor[(c >> 4) & 0xf][(d >> 4) & 0xf]] << 4)
              | xor[xor[(a >> 0) & 0xf][(b >> 0) & 0xf]][xor[(c >> 0) & 0xf][(d >> 0) & 0xf]];


      input[4*j + 2] = (xor[xor[(a >> 12) & 0xf][(b >> 12) & 0xf]][xor[(c >> 12) & 0xf][(d >> 12) & 0xf]] << 4)
      | xor[xor[(a >> 8) & 0xf][(b >> 8) & 0xf]][xor[(c >> 8) & 0xf][(d >> 8) & 0xf]];

      input[4*j + 1] = (xor[xor[(a >> 20) & 0xf][(b >> 20) & 0xf]][xor[(c >> 20) & 0xf][(d >> 20) & 0xf]] << 4)
      | xor[xor[(a >> 16) & 0xf][(b >> 16) & 0xf]][xor[(c >> 16) & 0xf][(d >> 16) & 0xf]];

      input[4*j + 0] = (xor[xor[(a >> 28) & 0xf][(b >> 28) & 0xf]][xor[(c >> 28) & 0xf][(d >> 28) & 0xf]] << 4)
      | xor[xor[(a >> 24) & 0xf][(b >> 24) & 0xf]][xor[(c >> 24) & 0xf][(d >> 24) & 0xf]];
    }

  }
  shiftRows (input);

  for (int j = 0; j < 16; j++) {
    input[j] = TBoxes[9][j][input[j]];
  }

  for (int i = 0; i < 16; i++)
    output[i] = input[i];

}

int main(void) {
  u8 out[16];
  //u8 in2[16] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
  u8 in[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

  printState(in);
  aes_128_table_encrypt(in, out);
  printState(out);

  return 0;
}
