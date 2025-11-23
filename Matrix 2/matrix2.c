#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 5
#define PW_LEN 25

void multiply_matrix(int A[SIZE][SIZE], int B[SIZE][SIZE], long long R[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            R[i][j] = 0;
            for (int k = 0; k < SIZE; k++)
                R[i][j] += (long long)A[i][k] * B[k][j];
        }
}

void print_ascii() {
    puts(" __    __     ______     ______   ______     __     __  __        __     __    ");
    puts("/\\ \"-./  \\   /\\  __ \\   /\\__  _\\ /\\  == \\   /\\ \\   /\\_\\_\\_\\      /\\ \\   /\\ \\   ");
    puts("\\ \\ \\-./\\ \\  \\ \\  __ \\  \\/_/\\ \\/ \\ \\  __<   \\ \\ \\  \\/_\\/_/_     \\ \\ \\  \\ \\ \\  ");
    puts(" \\ \\_\\ \\ \\_\\  \\ \\_\\ \\_\\    \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_\\   /\\_\\/\\_\\     \\ \\_\\  \\ \\_\\ ");
    puts("  \\/_/  \\/_/   \\/_/\\/_/     \\/_/   \\/_/ /_/   \\/_/   \\/_/\\/_/      \\/_/   \\/_/ ");
    puts("");
}

int main() {
    srand(time(NULL));

    puts("Matrix Challenge — Everything Randomized");
    print_ascii();

    char PASSWORD[PW_LEN + 1];
    char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int clen = strlen(chars);
    for (int i = 0; i < PW_LEN; i++)
        PASSWORD[i] = chars[rand() % clen];
    PASSWORD[PW_LEN] = '\0';

    

    char input[128];
    printf("Enter password: ");
    if (!fgets(input, sizeof(input), stdin)) return 1;

    if (strlen(input) < PW_LEN) {
        puts("Password too short.");
        return 1;
    }

    int userMatrix[SIZE][SIZE];
    for (int i = 0; i < PW_LEN; i++)
        userMatrix[i/SIZE][i%SIZE] = (unsigned char)input[i];

    int transform[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            transform[i][j] = (rand() % 10) + 1;

    long long result[SIZE][SIZE];
    multiply_matrix(userMatrix, transform, result);

    int pwMatrix[SIZE][SIZE];
    for (int i = 0; i < PW_LEN; i++)
        pwMatrix[i/SIZE][i%SIZE] = (unsigned char)PASSWORD[i];

    long long target[SIZE][SIZE];
    multiply_matrix(pwMatrix, transform, target);

    int ok = 1;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (result[i][j] != target[i][j])
                ok = 0;

    if (ok) {
        FILE *f = fopen("flag.txt","r");
        if (!f) { puts("flag.txt missing!"); return 1; }
        char flag[256];
        fgets(flag, sizeof(flag), f);
        fclose(f);
        printf("Access Granted! %s\n", flag);
    } else {
        puts("Wrong password.");
    }

    return 0;
}
