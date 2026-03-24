#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: custom_wc <filename>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    int lines = 0, words = 0, characters = 0, in_word = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        characters++;
        if (ch == '\n') lines++;
        if (isspace(ch)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }

    printf("Lines: %d, Words: %d, Characters: %d\n", lines, words, characters);
    fclose(file);
    return 0;
}
