typedef enum TOKEN_TYPE {
    INT, FLOAT, OPERADOR, PONTUACAO, PALAVRA_RESERVADA, INDENTIFICADOR
} token_type;

typedef enum TOKEN_VALUE {
    NUMERIC,                    // INT e FLOAT
    SOMA, SUBT, MULT, DIV,      // OPERADOR
    ABRE_P, FECHA_P, VIRGULA,   // PONTUACAO
    ASPAS, ATRIBUICAO,
    START, END, _INT, _FLOAT,   // PALAVRA RESERVADA
    STRING, RETURN, TRUE, 
    FALSE, END_FUNC, MAIOR,
    MENOR, MAIOR_IGUAL,
    MENOR_IGUAL, NEGADO,
    DIF, EQU, IF, END_IF,
    WHILE, END_WHILE, ELSE,
    NOME                        // INDENTIFICADOR
} token_value;

typedef struct no_token{
    token_type type;
    token_type value;
    int n;                  // literal numerico inteiro(se tiver, caso contrario: n = 0)
    struct no_token *prox;
} No_token;

No_token *lexer(void);
void imprimir_tokens(No_token *Tokens);
No_token *go_to_next_token(No_token *Tokens);