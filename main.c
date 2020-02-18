#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256

//return the size of encoded information in bytes
size_t lzw_encoder(char* info, int size);
//return the size of the initial alphabet
size_t initialize_alphabet(unsigned char* alphabet[ALPHABET_SIZE], char* info, int info_size);
//dealloc all data dynamically allocated to prevent memory leaks
void dealloc_alphabet(unsigned char* alphabet[ALPHABET_SIZE], size_t size);

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

    lzw_encoder(buffer, fsize);

    free(buffer);

    return 0;
}

size_t initialize_alphabet(unsigned char* alphabet[ALPHABET_SIZE], char* info, int info_size) {
    size_t symbols_in_alpha = 0;

    //flag
    short has_symbol = 0;
    // populate initial alphabet
    for(int i = 0; i < info_size; i++){
        if(!symbols_in_alpha){
            alphabet[0] = malloc(sizeof(unsigned char*)+1);
            alphabet[0][0] = info[i];
            alphabet[0][1] = '\0';
            symbols_in_alpha++;
        } else {
            // look for the symbol in alphabet
            for(int alpha_i = 0; alpha_i < symbols_in_alpha; alpha_i++) {
                if((*alphabet[alpha_i]) == info[i]){
                    has_symbol = 1;
                    break;
                }
            }

            if(!has_symbol){
                alphabet[symbols_in_alpha] = malloc(sizeof(unsigned char*));;
                *(alphabet[symbols_in_alpha]) = info[i];
                alphabet[symbols_in_alpha][1] = '\0';
                symbols_in_alpha++;
            }

            has_symbol = 0;
        }
    }
    //initial alphabet ready 

    return symbols_in_alpha;
}

void dealloc_alphabet(unsigned char* alphabet[ALPHABET_SIZE], size_t size) {
    int index = 0;
    while(alphabet[index]){
        free(alphabet[index]);
        alphabet[index] = NULL;
        index++;
    }
}
/*
  se foi adicionado, retorna um valor maior que o tamanho atual do alfabeto
  dessa forma é sabido que o índice retornado foi resultado de uma adição ao alfabeto
*/
int alphabet_check(unsigned char** alphabet, unsigned char* phrase, short phrase_size) {
    int alphabet_index = 0;

    //flag
    short phrase_found = 0;
    while(alphabet[alphabet_index]){
        if(0 == strncmp(alphabet[alphabet_index], phrase, phrase_size)){
            phrase_found = 1;
            break;
        }
        alphabet_index++;
    }

    if(!phrase_found){
        alphabet_index = -1;
    }

    return alphabet_index;
}

void add_to_alphabet(unsigned char** alphabet, unsigned char* phrase, short phrase_size) {
    short alpha_index = 0;

    while (alphabet[alpha_index]) {
        alpha_index++;
    }

    alphabet[alpha_index] = malloc(phrase_size);
    alphabet[alpha_index] = phrase;
    alphabet[alpha_index][phrase_size] = '\0';
}

size_t lzw_encoder(char* info, int size) {
    unsigned char* alphabet[ALPHABET_SIZE] = {NULL}, code[ALPHABET_SIZE];
    size_t alphabet_size = initialize_alphabet(alphabet, info, size), code_size = 0;

    unsigned char** ptr_buffer = malloc(size*sizeof(unsigned char*));
    for(int i = 0; i < size; i++){
        ptr_buffer[i] = malloc(sizeof(unsigned char*));
        (*ptr_buffer[i]) = info[i];
    }

    int phrase_index;
    unsigned char* aux_phrase = {NULL};
    for(int i = 0; i < size; i++){
        for(int j = 1; j < size; j++) {
            aux_phrase = malloc(j);
            for(int k = 0; k < j; k++){
                aux_phrase[k] = info[i+k];
            }
            phrase_index = alphabet_check(alphabet, aux_phrase, j);
            if(phrase_index < 0){
                add_to_alphabet(alphabet, aux_phrase, j);
                alphabet_size++;
                break;
            } else {
                printf("%d\n", phrase_index);
            }
        }
    }

    for(int i = 0; i < alphabet_size; i++){
        for(int j = 0; alphabet[i][j]; j++)
            printf("%c", alphabet[i][j]);
        printf("\n");
    }

    // memory dealloc section
    for(int i = 0; i < size; i++){
        free(ptr_buffer[i]);
        ptr_buffer[i] = NULL;
    }
    free(aux_phrase);
    free(ptr_buffer);
    dealloc_alphabet(alphabet, alphabet_size);
    return 0;
}
