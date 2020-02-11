#include <stdio.h>
#include <stdlib.h>

//return the size of encoded information in bytes
size_t lzw_encoder(char** info, int size);

int main(int argc, char** argv) {
    FILE* input_fp;
    char* buffer;

    input_fp = fopen(argv[1], "rb");

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
    char* alphabet = NULL;
    int alphabet_size = 0;

    //flag
    short hasSymbol = 0;
    
    for(int i = 0; i < size; i++) {
        if (alphabet == NULL) {
            alphabet = malloc(1);
            alphabet[alphabet_size] = (*info)[i];
            alphabet_size++;
        } else {
            for(int alpha_i = 0; alpha_i < alphabet_size; alpha_i++) {
                if(alphabet[alpha_i] == (*info)[i]){
                    hasSymbol = 1;
                    break;
                }
            }
            if(!hasSymbol){
                alphabet = realloc(alphabet, alphabet_size+1);
                alphabet[alphabet_size] = (*info)[i];
                alphabet_size++;
            }
            
            hasSymbol = 0;
        }
    }

    printf("%s\n", alphabet);

    return 0;
}