#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_EXPRESSION 500
#define MAX_NUM_TOKEN_VALUE 100
//#define NUM_ACCEPT_STATES 6

void remove_spaces(char *s){
    char *d = s;
    do{
        while(*d == ' ') ++d;
    } while (*s++ = *d++);
}

int main(){
    printf("Lexer\n");
    
    char expression[MAX_NUM_EXPRESSION], token_value[MAX_NUM_TOKEN_VALUE] = "";
    int estado_atual = 0, i = 0; //estados_aceitacao[NUM_ACCEPT_STATES] = {1,2,3,4,5,8};

    printf("Digite a expressao:\n");
    gets(expression);
    remove_spaces(expression);
    strncat(expression, "#", 1);
    printf("\n");

    while(expression[i] != '\0'){
        //printf("\nlendo: %c", expression[i]);
        
        switch(estado_atual){
            case 0: // q0 (ESTADO INICIAL)
                //printf(" estado atual: q0");

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
                else estado_atual = -1; // transição invalida

                break;

            case 1: // q1 (ESTADO DE ACEITAÇÃO DE OPERADOR)
                //printf(" estado atual: q1");

                //printf("Operador(%s)\n", token_value);
                token_value[0] == '+' ? printf("Operador(Soma)\n") : 
                    token_value[0] == '-' ? printf("Operador(Subtracao)\n") :
                        token_value[0] == '*' ? printf("Operador(Multiplicacao)\n") :
                            printf("Operador(Divisao)\n"); // token_value[0] == '/' 

                estado_atual = 0;
                token_value[0] = '\0';
                i--;

                break;
            
            case 2: // q2 (ESTADO DE ACEITAÇÃO DE PONTUACAO)
                //printf(" estado atual: q2");

                //printf("Pontuacao(%s)\n", token_value);
                token_value[0] == '(' ? printf("Pontuacao(Abre Parenteses)\n") :
                    printf("Pontuacao(Fecha Parenteses)\n"); // token_value[0] == ')'
                
                estado_atual = 0;
                token_value[0] = '\0';
                i--;

                break;

            case 3: // q3 (ESTADO DE ACEITAÇÃO DE NUMERO)
                ///printf(" estado atual: q3");

                if(expression[i] >= '0' && expression[i] <= '9'){ //looping
                    strncat(token_value, &expression[i], 1);
                }
                else{
                    printf("Numero(%s)\n", token_value);
                    estado_atual = 0;
                    token_value[0] = '\0';
                    i--;
                }

                break;
            
            default: // q-1 (ESTADO DE REGEIÇÃO)
                system("cls");
                printf("Erro Lexico");
                return -1;
        }

        i++;
    }

    return 0;
}