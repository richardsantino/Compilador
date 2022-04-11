#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

#define MAX_NUM_EXPRESSION 500
#define MAX_NUM_TOKEN_VALUE 100

#define NO_LITERAL 0

#define QINITIAL 0    // estado inicial do automato
#define QSTART -2    // usado para verificar se o primeiro numero é negativo
#define QREJECT -3  // estado de rejeição

void remove_spaces(char *s){
    char *d = s;
    do{
        while(*d == ' ') ++d;
    } while (*s++ = *d++);
}

void adicionar_token(No_token** Tokens, token_type type, token_value value, int n){
    No_token *novo = (No_token*) malloc(sizeof(No_token));
    novo->type = type;
    novo->value = value;
    novo->n = n;
    novo->prox = NULL;

    if(*Tokens == NULL){
        *Tokens = novo;
    }
    else{
        No_token *aux = *Tokens;

        while(aux->prox != NULL){
            aux = aux->prox;
        }

        aux->prox = novo;
    }
}

No_token *get_proximo_token(No_token *Tokens, token_type *type, token_value *value, int *n){
    No_token *aux = Tokens, *Tokens_updated = aux->prox;
    
    *type = aux->type;
    *value = aux->value;
    *n = aux->n;
    
    free(aux);
    return Tokens_updated;
}

void imprimir_tokens(No_token *Tokens){
    No_token *aux = Tokens;

    while(aux != NULL){
        printf("<%d> <%d> %d\n", aux->type, aux->value, aux->n);
        aux = aux->prox;
    }
}

No_token *lexer(void){
    system("cls");
    printf("Lexer\n");
    No_token *Tokens = NULL;

    char expression[MAX_NUM_EXPRESSION], token_value[MAX_NUM_TOKEN_VALUE] = "";
    int estado_atual = QSTART, i = 0;

    //aux
    int number_int;
    float number_float;

    printf("Digite a expressao:\n");
    gets(expression);
    remove_spaces(expression);
    strncat(expression, "#", 1);
    printf("\n");

    while(expression[i] != '\0'){
        // printf("\nlendo: %c no estado: q%d", expression[i], estado_atual);
        // system("pause");
        
        switch(estado_atual){
            case QSTART: // q-2 (ESTADO DE INICIO (NÃO É O ESTADO INICIAL) QUE CHECA SE O PRIMEIRO NUMERO DA EXPRESSÃO É NEGATIVO)
                if(expression[i] == '-'){ // -> q-1
                    estado_atual = -1;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // primeiro numero não é negativo ou o operador não é de subtração, indo para o estado inicial do automato
                    estado_atual = QINITIAL;
                    i--;
                }

                break;

            case -1: // q-1 (ESTADO DE ACEITAÇÃO DO OPERADOR SUBTRAÇÃO)
                if(expression[i] >= '0' && expression[i] <= '9'){ // -> q3
                    estado_atual = 3;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == '.'){ // -> q4
                    estado_atual = 4;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // reconhece oque foi lido anteriormente
                    printf("Operador(Subtracao)\n");

                    adicionar_token(&Tokens, OPERADOR, SUBT, NO_LITERAL);

                    estado_atual = QINITIAL;
                    token_value[0] = '\0';
                    i--;
                }

                break;

            case QINITIAL: // q0 (ESTADO INICIAL)
                if(expression[i] == '+' || expression[i] == '-' ||
                     expression[i] == '*' || expression[i] == '/'){ // -> q1
                    estado_atual = 1;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == '(' || expression[i] == ')'){ // -> q3
                    estado_atual = 2;
                    strncat(token_value, &expression[i], 1);
                }

                else if(expression[i] >= '0' && expression[i] <= '9'){ // -> q3 
                    estado_atual = 3;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == '.'){ // -> q4
                    estado_atual = 4;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'E'){ // -> q10
                    estado_atual = 10;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'S'){ // -> q13
                    estado_atual = 13;
                    strncat(token_value, &expression[i], 1);
                }
                else estado_atual = QREJECT; // transição invalida

                break;

            case 1: // q1 (ESTADO DE ACEITAÇÃO DE OPERADOR)
                //printf("Operador(%s)\n", token_value);
                // reconhece oque foi lido anteriormente

                if(token_value[0] == '+'){
                    printf("Operador(Soma)\n");
                    adicionar_token(&Tokens, OPERADOR, SOMA, NO_LITERAL);
                }
                else if(token_value[0] == '-'){
                    printf("Operador(Subtracao)\n");
                    adicionar_token(&Tokens, OPERADOR, SUBT, NO_LITERAL);
                }
                else if(token_value[0] == '*'){
                    printf("Operador(Multiplicacao)\n");
                    adicionar_token(&Tokens, OPERADOR, MULT, NO_LITERAL);
                }
                else{
                    printf("Operador(Divisao)\n");
                    adicionar_token(&Tokens, OPERADOR, DIV, NO_LITERAL);
                } 

                estado_atual = QINITIAL;
                token_value[0] = '\0';
                i--;

                break;
            
            case 2: // q2 (ESTADO DE ACEITAÇÃO DE PONTUACAO)
                //printf("Pontuacao(%s)\n", token_value);
                // reconhece oque foi lido anteriormente
                
                if(token_value[0] == '('){
                    printf("Pontuacao(Abre Parenteses)\n");
                    adicionar_token(&Tokens, PONTUACAO, ABRE_P, NO_LITERAL);
                }
                else{
                    printf("Pontuacao(Fecha Parenteses)\n"), 
                    adicionar_token(&Tokens, PONTUACAO, FECHA_P, NO_LITERAL);
                }

                estado_atual = QINITIAL;
                token_value[0] = '\0';
                i--;

                break;

            case 3: // q3 (ESTADO DE ACEITAÇÃO DE NUMERO INTEIRO)
                if(expression[i] >= '0' && expression[i] <= '9'){ //looping
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == '.'){ // -> q9
                    estado_atual = 9;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'e'){ // -> q6
                    estado_atual = 6;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // reconhece oque foi lido anteriormente
                    printf("Numero INT(%s)\n", token_value);

                    number_int = atoi(token_value);
                    if(token_value[0] == '-') adicionar_token(&Tokens, INT, NUMERIC, number_int--);
                    else adicionar_token(&Tokens, INT, NUMERIC, number_int); 

                    estado_atual = QINITIAL;
                    token_value[0] = '\0';
                    i--;
                }

                break;

            case 4: // q4
                if(expression[i] >= '0' && expression[i] <= '9'){ // -> q5
                    estado_atual = 5;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 5: // q5 (ESTADO DE ACEITAÇÃO DE NUMERO (FLOAT))
                if(expression[i] >= '0' && expression[i] <= '9'){ // -> looping
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'e' || expression[i] == 'E'){ // -> q6
                    estado_atual = 6;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // reconhece oque foi lido anteriormente
                    printf("Numero FLOAT(%s)\n", token_value);

                    // number_float = strtod(token_value, NULL);

                    estado_atual = QINITIAL;
                    token_value[0] = '\n';
                    i--;
                }

                break;

            case 6: // q6 
                if(expression[i] == '+' || expression[i] == '-'){ // -> q7
                    estado_atual = 7;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] >= '0' && expression[i] <= '9'){ // -> q8
                    estado_atual = 8;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;
            
            case 7: // q7
                if(expression[i] >= '0' && expression[i] <= '9'){ // -> q8
                    estado_atual = 8;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;

            case 8: // q8 (ESTADO DE ACEITAÇÃO DE NUMERO (FLOAT))
                // reconhece oque foi lido anteriormente
                printf("Numero FLOAT(%s)\n", token_value);
                estado_atual = QINITIAL;
                token_value[0] = '\0';
                i--;

                break;

            case 9: // q9 (ESTADO DE ACEITAÇÃO DE NUMERO (FLOAT))
                if(expression[i] >= '0' && expression[i] <= '9'){ // -> q5
                    estado_atual = 5;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // reconhece oque foi lido anteriormente
                    printf("Numero FLOAT(%s)\n", token_value);
                    estado_atual = QINITIAL;
                    token_value[0] = '\0';
                    i--;
                }

                break;

            case 10: // q10
                if(expression[i] == 'N') { // -> q11
                    estado_atual = 11;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 11: // q11
                if(expression[i] == 'D') { // -> q12
                    estado_atual = 12;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 12: // q12 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (END))
                printf("Palavra reservada(END)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, END, NO_LITERAL);

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 13: // q13
                if(expression[i] == 'T') { // -> q14
                    estado_atual = 14;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 14: // q14
                if(expression[i] == 'A') { // -> q15
                    estado_atual = 15;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 15: // q15
                if(expression[i] == 'R') { // -> q16
                    estado_atual = 16;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;
            
            case 16: // q16
                if(expression[i] == 'T') { // -> q17
                    estado_atual = 17;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 17: // q17 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (START))
                printf("Palavra reservada(START)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, START, NO_LITERAL);

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;            

            default: // QREJECT (ESTADO DE REJEIÇÃO)
                system("cls");
                printf("Erro Lexico \n");
                return NULL;
        }

        i++;
    }

    printf("\n");
    return Tokens;
}
