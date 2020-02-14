#include <stdio.h>
#include <stdlib.h>

#define ALPHABET_SIZE 256

//return the size of encoded information in bytes
size_t lzw_encoder(char** info, int size);

int main(int argc, char** argv) {
    FILE* input_fp;
    char* buffer;

    input_fp = fopen(argv[1], "rb");
    if(!input_fp){
        puts("(!) error open file. (!)");
        return -1;
    }

    //compute file size
    fseek(input_fp, 0, SEEK_END);
    size_t fsize = ftell(input_fp);
    rewind(input_fp);

    buffer = malloc(fsize+1);

    fread(buffer, 1, fsize, input_fp);

    fclose(input_fp);

    lzw_encoder(&buffer, fsize);

    return 0;
}

size_t lzw_encoder(char** info, int size) {
    unsigned char* alphabet[ALPHABET_SIZE] = {NULL};
    int symbols_in_alpha = 0;

    //flag
    short has_symbol = 0;
    // populate initial alphabet
    for(int i = 0; i < size; i++){
        if(!symbols_in_alpha){
            alphabet[0] = malloc(1);
            (*alphabet[0]) = (*info)[i];
            symbols_in_alpha++;
        } else {
            // look for the symbol in alphabet
            for(int alpha_i = 0; alpha_i < symbols_in_alpha; alpha_i++) {
                if((*alphabet[alpha_i]) == (*info)[i]){
                    has_symbol = 1;
                    break;
                }
            }

            if(!has_symbol){
                alphabet[symbols_in_alpha] = malloc(1);
                (*alphabet[symbols_in_alpha]) = (*info)[i];
                symbols_in_alpha++;
            }

            has_symbol = 0;
        }
    }
    //initial alphabet read
    
    for(int i = 0; i < symbols_in_alpha; i++){
        printf("%c\n", (*alphabet[i]));
    }

    return 0;
}
