%{
     #include <stdio.h>
     #include "ast.h"
     #include "lexer.h"
     ast_node *root;
     void yyerror(const char *s) { printf("ERROR: %s\n", s); }
%}

%code provides {
    int parse(char *text, ast_node **node);
}

%union {
    char *string;
    double number;
    ast_node *node;
    int token;
}

%token <string> TIDENTIFIER
%token <number> TNUMBER
%type <node> expr ident number
%right '='
%left '-' '+'
%left '*' '/'
%left NEG
%start program

%%

program : /* empty */
        | expr { root = $1; };

        ident : TIDENTIFIER { $$ = ast_variable_create($1); };

        number : TNUMBER { $$ = ast_number_create($1); };

        expr : number
             | ident
             ;
%%

int parse(char *text, ast_node **node) {
    YY_BUFFER_STATE buffer = yy_scan_string(text);
    int rc = yyparse();
    yy_delete_buffer(buffer);
    if(rc == 0) {
        *node = root;
        return 0;
    } else {
        return -1;
    }
}
