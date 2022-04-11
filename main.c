#include <stdio.h>
#include "lexer.h"

int main(void){
    No_token *Tokens = lexer();

    int n;
    token_type type;
    token_value value;

    printf("Lista dos Tokens (no formato que o parser vai ler)\n");
    imprimir_tokens(Tokens);
    
    printf("\nComo as informacoes do Token vao aparecer\n"); 
    Tokens = get_proximo_token(Tokens, &type, &value ,&n);
    printf("%d(TIPO DO TOKEN) - %d(VALOR DO TOKEN) - %d(LITERAL(INT) DO TOKEN)\n", type, value, n);

    return 0;
}