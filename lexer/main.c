#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_EXPRESSION 500
#define MAX_NUM_TOKEN_VALUE 100

#define QINITIAL 0    // estado inicial do automato
#define QSTART -2    // usado para verificar se o primeiro numero é negativo
#define QREJECT -3  // estado de rejeição

void remove_spaces(char *s){
    char *d = s;
    do{
        while(*d == ' ') ++d;
    } while (*s++ = *d++);
}

int main(){
    system("cls");
    printf("Lexer\n");
    
    char expression[MAX_NUM_EXPRESSION], token_value[MAX_NUM_TOKEN_VALUE] = "";
    int estado_atual = QSTART, i = 0;

    printf("Digite a expressao:\n");
    gets(expression);
    remove_spaces(expression);
    strncat(expression, "#", 1);
    printf("\n");

    while(expression[i] != '\0'){
        //printf("\nlendo: %c no estado: q%d", expression[i], estado_atual);
        
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
                    printf("Operador(Subtracao)\n", token_value);
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
                else estado_atual = QREJECT; // transição invalida

                break;

            case 1: // q1 (ESTADO DE ACEITAÇÃO DE OPERADOR)
                //printf("Operador(%s)\n", token_value);
                // reconhece oque foi lido anteriormente
                token_value[0] == '+' ? printf("Operador(Soma)\n") : 
                    token_value[0] == '-' ? printf("Operador(Subtracao)\n") :
                        token_value[0] == '*' ? printf("Operador(Multiplicacao)\n") :
                            printf("Operador(Divisao)\n"); // token_value[0] == '/' 

                estado_atual = QINITIAL;
                token_value[0] = '\0';
                i--;

                break;
            
            case 2: // q2 (ESTADO DE ACEITAÇÃO DE PONTUACAO)
                //printf("Pontuacao(%s)\n", token_value);
                // reconhece oque foi lido anteriormente
                token_value[0] == '(' ? printf("Pontuacao(Abre Parenteses)\n") :
                    printf("Pontuacao(Fecha Parenteses)\n"); // token_value[0] == ')'
                
                estado_atual = QINITIAL;
                token_value[0] = '\0';
                i--;

                break;

            case 3: // q3 (ESTADO DE ACEITAÇÃO DE NUMERO)
                if(expression[i] >= '0' && expression[i] <= '9'){ //looping
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == '.'){ // -> q9
                    estado_atual = 9;
                    strncat(token_value, &expression[i], 1);
                }
                else if(expression[i] == 'e' || expression[i] == 'E'){ // -> q6
                    estado_atual = 6;
                    strncat(token_value, &expression[i], 1);
                }
                else{ // reconhece oque foi lido anteriormente
                    printf("Numero(%s)\n", token_value);
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
                    printf("Numero(%s)\n", token_value);
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
                printf("Numero(%s)\n", token_value);
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
                    printf("Numero(%s)\n", token_value);
                    estado_atual = QINITIAL;
                    token_value[0] = '\0';
                    i--;
                }

                break;

            default: // QREJECT (ESTADO DE REJEIÇÃO)
                system("cls");
                printf("Erro Lexico \n");
                return -1;
        }

        i++;
    }

    return 0;
}