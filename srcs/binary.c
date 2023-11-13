#include <stdio.h>

// Função para converter decimal para binário e retornar como inteiro
int decimalParaBinario(int decimal) {
    int binario = 0, base = 1;

    if (decimal == 0) {
        return 0;
    }

    while (decimal > 0) {
        int resto = decimal % 2;
        binario = binario + resto * base;
        decimal = decimal / 2;
        base = base * 10;
    }

    return binario;
}

int main() {
    int decimal;

    printf("Digite um valor decimal: ");
    scanf("%d", &decimal);

    int binario = decimalParaBinario(decimal);

    printf("Em binário: %d\n", binario);

    return 0;
}
