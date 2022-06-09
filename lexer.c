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
        while(*d == '\n') ++d;
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

No_token *go_to_next_token(No_token *Tokens){
    No_token *aux = Tokens, *Tokens_updated = aux->prox;
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
    printf("------- LEXER TOKENS -------\n");
    No_token *Tokens = NULL;

    // char expression[MAX_NUM_EXPRESSION], token_value[MAX_NUM_TOKEN_VALUE] = "";
    char *expression, token_value[MAX_NUM_TOKEN_VALUE] = "";
    int estado_atual = QSTART, i = 0;
    FILE *textfile;
    long numbytes;

    //aux
    int number_int;
    float number_float;

    // read code in txt
    textfile = fopen("code.txt", "r");
    if(textfile == NULL) return NULL;
    fseek(textfile, 0L, SEEK_END);
    numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);
    expression = (char*)calloc(numbytes, sizeof(char));
    if(expression == NULL) return NULL;
    fread(expression, sizeof(char), numbytes, textfile);
    fclose(textfile);
    // printf(expression);
    strncat(expression, "#", 1);
    remove_spaces(expression);

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
                else if(expression[i] == '(' || expression[i] == ')' ||
                     expression[i] == ',' || expression[i] == '\"' ||
                     expression[i] == '='){ // -> q2
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
                else if(expression[i] == 'I'){ // -> q18
                    estado_atual = 18;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'F'){ // -> q21
                    estado_atual = 21;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] >= 'a' && expression[i] <= 'z'){ // -> q30
                    estado_atual = 30;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'R'){ // -> q31
                    estado_atual = 31;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'T'){ // -> q37
                    estado_atual = 37;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == '>' || expression[i] == '<' ||
                     expression[i] == '!'){ // -> q49
                    estado_atual = 49;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'W'){ // -> q54
                    estado_atual = 54;
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
                if(expression[i] == '='){ // q50
                    estado_atual = 50;
                    strncat(token_value, &expression[i], 1); 
                }
                else{ // reconhece oque foi lido anteriormente
                    if(token_value[0] == '('){
                        printf("Pontuacao(Abre Parenteses)\n");
                        adicionar_token(&Tokens, PONTUACAO, ABRE_P, NO_LITERAL);
                    }
                    else if(token_value[0] == ','){
                        printf("Pontuacao(Virgula)\n");
                        adicionar_token(&Tokens, PONTUACAO, VIRGULA, NO_LITERAL);
                    }
                    else if(token_value[0] == '\"'){
                        printf("Pontuacao(Aspas)\n");
                        adicionar_token(&Tokens, PONTUACAO, ASPAS, NO_LITERAL);
                    }
                    else if(token_value[0] == '='){
                        printf("Pontuacao(Atribuicao)\n");
                        adicionar_token(&Tokens, PONTUACAO, ATRIBUICAO, NO_LITERAL);
                    }
                    else{
                        printf("Pontuacao(Fecha Parenteses)\n"), 
                        adicionar_token(&Tokens, PONTUACAO, FECHA_P, NO_LITERAL);
                    }

                    estado_atual = QINITIAL;
                    token_value[0] = '\0';
                    i--;
                }  

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

                    if(token_value[0] == '-') adicionar_token(&Tokens, FLOAT, NUMERIC, -666);
                    else adicionar_token(&Tokens, FLOAT, NUMERIC, 666); 

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

                    if(token_value[0] == '-') adicionar_token(&Tokens, FLOAT, NUMERIC, -666);
                    else adicionar_token(&Tokens, FLOAT, NUMERIC, 666); 
                    
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
                else if(expression[i] == 'L'){ // -> q64
                    estado_atual = 64;
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
                if(expression[i] == 'F') { // -> q45
                    estado_atual = 45;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'I'){ // -> q52
                    estado_atual = 52;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'W'){ // -> q59
                    estado_atual = 59;
                    strncat(token_value, &expression[i], 1);
                }
                else{
                    printf("Palavra reservada(END)\n");
                    adicionar_token(&Tokens, PALAVRA_RESERVADA, END, NO_LITERAL);

                    estado_atual = QSTART;
                    token_value[0] = '\0';
                    i--;
                }

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
                else if(expression[i] == 'R') { // -> q26
                    estado_atual = 26;
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

            case 18: // q18
                if(expression[i] == 'N'){ // -> q19
                    estado_atual = 19;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'F'){ // -> q51
                    estado_atual = 51;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 19: // q19
                if(expression[i] == 'T'){ // -> q20
                    estado_atual = 20;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 20: // q20 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (INT))
                printf("Palavra reservada(INT)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, _INT, NO_LITERAL);

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 21: // q21
                if(expression[i] == 'L'){ // -> q22
                    estado_atual = 22;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'A'){ // -> q41
                    estado_atual = 41;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 22: // q22
                if(expression[i] == 'O'){ // -> q23
                    estado_atual = 23;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 23: // q23
                if(expression[i] == 'A'){ // -> q24
                    estado_atual = 24;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 24: // q24
                if(expression[i] == 'T'){ // -> q25
                    estado_atual = 25;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 25: // q25 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (FLOAT))
                printf("Palavra reservada(FLOAT)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, _FLOAT, NO_LITERAL);

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 26: // q26
                if(expression[i] == 'I') { // -> q27
                    estado_atual = 27;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 27: // q27
                if(expression[i] == 'N') { // -> q28
                    estado_atual = 28;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 28: // q28
                if(expression[i] == 'G') { // -> q29
                    estado_atual = 29;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 29: // q29 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (STRING))
                printf("Palavra reservada(STRING)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, STRING, NO_LITERAL);

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 30: // q30 (ESTADO DE ACEITAÇÃO DE INDENTIFICADOR NOME)
                if(expression[i] >= 'a' && expression[i] <= 'z'){ //looping
                    strncat(token_value, &expression[i], 1);
                }
                else{ // reconhece oque foi lido anteriormente
                    printf("Indentificador(%s)\n", token_value);

                    adicionar_token(&Tokens, INDENTIFICADOR, NOME, NO_LITERAL);

                    estado_atual = QINITIAL;
                    token_value[0] = '\0';
                    i--;
                }

                break;

            case 31: // q31
                if(expression[i] == 'E'){ // -> q32
                    estado_atual = 32;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;

            case 32: // q32
                if(expression[i] == 'T'){ // -> q33
                    estado_atual = 33;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;

            case 33: // q33
                if(expression[i] == 'U'){ // -> q34
                    estado_atual = 34;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;

            case 34: // q34
                if(expression[i] == 'R'){ // -> q35
                    estado_atual = 35;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;

            case 35: // q35
                if(expression[i] == 'N'){ // -> q36
                    estado_atual = 36;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;

            case 36: // q36 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (RETURN))
                // reconhece oque foi lido anteriormente
                printf("Palavra reservada(RETURN)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, RETURN, NO_LITERAL);

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 37: // q37
                if(expression[i] == 'R'){ // -> q38
                    estado_atual = 38;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;

            case 38: // q38
                if(expression[i] == 'U'){ // -> q39
                    estado_atual = 39;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;

            case 39: // q39
                if(expression[i] == 'E'){ // -> q40
                    estado_atual = 40;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }

                break;

            case 40: // q40 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (TRUE))
                // reconhece oque foi lido anteriormente
                printf("Palavra reservada(TRUE)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, TRUE, NO_LITERAL);

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 41: // q41
                if(expression[i] == 'L'){ // -> q42
                    estado_atual = 42;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 42: // q42
                if(expression[i] == 'S'){ // -> q43
                    estado_atual = 43;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 43: // q43
                if(expression[i] == 'E'){ // -> q44
                    estado_atual = 44;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 44: // q44 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (FALSE))
                // reconhece oque foi lido anteriormente
                printf("Palavra reservada(FALSE)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, FALSE, NO_LITERAL);

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 45: // q45
                if(expression[i] == 'U') { // -> q46
                    estado_atual = 46;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 46: // q46
                if(expression[i] == 'N') { // -> q47
                    estado_atual = 47;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }
            
                break;

            case 47: // q47
                if(expression[i] == 'C') { // -> q48
                    estado_atual = 48;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                }
            
                break;

            case 48: // q48 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (END FUNC))
                // reconhece oque foi lido anteriormente
                printf("Palavra reservada(END_FUNC)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, END_FUNC, NO_LITERAL);

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 49: // q49 (ESTADO DE ACEITAÇÃO DE PALABRA RESERVADA (>, <, !))
                if(expression[i] == '=') { // -> q50
                    estado_atual = 50;
                    strncat(token_value, &expression[i], 1);
                }
                else{
                    if(token_value[0] == '>'){
                        printf("Palavra reservada(Maior)\n");
                        adicionar_token(&Tokens, PALAVRA_RESERVADA, MAIOR, NO_LITERAL);
                    }
                    else if(token_value[0] == '<'){
                        printf("Palavra reservada(Menor)\n");
                        adicionar_token(&Tokens, PALAVRA_RESERVADA, MENOR, NO_LITERAL);
                    }
                    else if(token_value[0] == '!'){
                        printf("Palavra reservada(Negado)\n");
                        adicionar_token(&Tokens, PALAVRA_RESERVADA, NEGADO, NO_LITERAL);
                    }
                    
                    estado_atual = QSTART;
                    token_value[0] = '\0';
                    i--;
                }

                break;

            case 50: // q50 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (!=, ==))
                if(token_value[0] == '!'){
                    printf("Palavra reservada(Diferente)\n");
                    adicionar_token(&Tokens, PALAVRA_RESERVADA, DIF, NO_LITERAL);
                }
                else if(token_value[0] == '='){
                    printf("Palavra reservada(Equivalente)\n");
                    adicionar_token(&Tokens, PALAVRA_RESERVADA, EQU, NO_LITERAL);
                }

                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 51: // q51 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (IF))
                printf("Palavra reservada(IF)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, IF, NO_LITERAL);
                
                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 52: // q52
                if(expression[i] == 'F') { // -> q53
                    estado_atual = 53;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 53: // q53 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (END IF))
                printf("Palavra reservada(END IF)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, END_IF, NO_LITERAL);
                
                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 54: // q54
                if(expression[i] == 'H') { // -> q55
                    estado_atual = 55;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 55: // q55
                if(expression[i] == 'I') { // -> q56
                    estado_atual = 56;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 56: // q56
                if(expression[i] == 'L') { // -> q57
                    estado_atual = 57;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 57: // q57
                if(expression[i] == 'E') { // -> q58
                    estado_atual = 58;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 58: // q58 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (WHILE))
                printf("Palavra reservada(WHILE)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, WHILE, NO_LITERAL);
                
                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            case 59: // q59
                if(expression[i] == 'H') { // -> q60
                    estado_atual = 60;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 60: // q60
                if(expression[i] == 'I') { // -> q61
                    estado_atual = 61;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 61: // q61
                if(expression[i] == 'L') { // -> q62
                    estado_atual = 62;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 62: // q62
                if(expression[i] == 'E') { // -> q63
                    estado_atual = 63;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 63: // q63 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (END WHILE))
                printf("Palavra reservada(END WHILE)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, END_WHILE, NO_LITERAL);
                
                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;
            
            case 64: // q64
                if(expression[i] == 'S') { // -> q65
                    estado_atual = 65;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 65: // q65
                if(expression[i] == 'E') { // -> q66
                    estado_atual = 66;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // transição invalida
                    estado_atual = QREJECT;
                    i--; 
                } 
            
                break;

            case 66: // q66 (ESTADO DE ACEITAÇÃO DE PALAVRA RESERVADA (ELSE))
                printf("Palavra reservada(ELSE)\n");
                adicionar_token(&Tokens, PALAVRA_RESERVADA, ELSE, NO_LITERAL);
                
                estado_atual = QSTART;
                token_value[0] = '\0';
                i--;

                break;

            default: // QREJECT (ESTADO DE REJEIÇÃO)
                //system("cls");
                printf("%s", expression);
                printf("Erro Lexico \n");
                return NULL;
        }

        i++;
    }

    printf("\n");
    return Tokens;
}
