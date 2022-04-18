typedef enum TOKEN_TYPE {
    INT, FLOAT, OPERADOR, PONTUACAO, PALAVRA_RESERVADA
} token_type;

typedef enum TOKEN_VALUE {
    NUMERIC,                    // INT e FLOAT
    SOMA, SUBT, MULT, DIV,      // OPERADOR
    ABRE_P, FECHA_P,            // PONTUACAO
    START, END,                 // PALAVRA RESERVADA
    NOTHING                     // NADA
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