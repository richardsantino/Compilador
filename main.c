#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void programm(No_token *Tokens);
No_token *read_START(No_token *Tokens);
No_token *read_END(No_token *Tokens);
No_token *read_NUMERIC(No_token *Tokens);

// No_token *<>(No_token *Tokens);

int main(void){
    No_token *Tokens = lexer();

    printf("---------------\n\n");

    programm(Tokens);
    printf("PROGRAMM OK"); 
    
    return 0;
}

void programm(No_token *Tokens){
    // START
    Tokens = read_START(Tokens);

    // NUMERO
    Tokens = read_NUMERIC(Tokens);

    // END
    Tokens = read_END(Tokens);

    if(Tokens != NULL){ // se ainda existir token depois do END
        printf("NOT OK\n\n");
        exit(1);
    }
}

No_token *read_START(No_token *Tokens){
    printf("* esperando por START *\n");
    printf("lendo -> (type)<%d> (value)<%d> (literal)<%d>\n", Tokens->type, Tokens->value, Tokens->n);

    if(Tokens->type == PALAVRA_RESERVADA && Tokens->value == START){ 
        printf("OK\n\n");
        return go_to_next_token(Tokens);
    }
    else{
        printf("NOT OK\n\n");
        exit(1);
    }
}

No_token *read_END(No_token *Tokens){
    printf("* esperando por END *\n");
    printf("lendo -> (type)<%d> (value)<%d> (literal)<%d>\n", Tokens->type, Tokens->value, Tokens->n);

    if(Tokens->type == PALAVRA_RESERVADA && Tokens->value == END){
        printf("OK\n\n");
        return go_to_next_token(Tokens);
    }
    else{
        printf("NOT OK\n\n");
        exit(1);
    }
}

No_token *read_NUMERIC(No_token *Tokens){
    printf("* esperando por NUMERO *\n");
    printf("lendo -> (type)<%d> (value)<%d> (literal)<%d>\n", Tokens->type, Tokens->value, Tokens->n);

    if((Tokens->type == INT || Tokens->type == FLOAT) && Tokens->value == NUMERIC){
        printf("OK - %d\n\n", Tokens->n);
        return go_to_next_token(Tokens);
    }
    else{
        printf("NOT OK\n\n");
        exit(1);
    }
}


/*
No_token *<>(No_token *Tokens){
    printf("* esperando por <> *\n");
    printf("lendo -> (type)<%d> (value)<%d> (literal)<%d>\n", Tokens->type, Tokens->value, Tokens->n);

    if(Tokens->type == <> && Tokens->value == <>>){
        printf("OK\n\n");
        return go_to_next_token(Tokens);
    }
    else{
        printf("NOT OK\n\n");
        exit(1);
    }
}
*/
