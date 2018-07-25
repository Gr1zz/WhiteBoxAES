#include "aes_table.c"


#define GETU32(pt) (\
        ((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^\
        ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]) )
        
#define PUTU32(ct, st) {\
        (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16);\
        (ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }


void printstate(unsigned char * in){
        for(int i=0; i < 16; i++) {
                printf("%.2X", in[i]);

        }
        printf("\n");
        return;
}
char ascii2hex(char in){
    char out;

    if (('0' <= in) && (in <= '9'))
        out = in - '0';

    if (('A' <= in) && (in <= 'F'))
        out = in - 'A' + 10;

    if (('a' <= in) && (in <= 'f'))
        out = in - 'a' + 10;

    return out;
}

void asciiStr2hex (char * in, char * out, unsigned int len){
    int j = 0;
    for (int i = 0; i < len; i += 2)
        out[j++]  = (ascii2hex(in[i ]) << 4) +  ascii2hex(in[i+1]);
}

int main(int argc, char * argv[]){
        unsigned char OUT[32];
        unsigned char IN[32];
        asciiStr2hex(argv[1], (char *)IN, 32);
        //unsigned char IN[32] = "00112233445566778899aabbccddeeff";
        printstate(IN);

        aes_128_table_encrypt(IN, OUT);

        printstate(OUT);

        return 0;
}
