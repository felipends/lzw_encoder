#include <stdio.h>
#include <stdlib.h>

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

    printf("%s\n", buffer);

    return 0;
}