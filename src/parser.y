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

%code top {
    void free_args(void **args, int count);
}

%union {
    char *string;
    double number;
    struct {
      char **args;
      int count;
    } args;
    ast_node *node;
    int token;
}

%token <string> TIDENTIFIER
%token <number> TNUMBER
%token <token> TEQUAL TPLUS TMIN TDIV TMUL TLAMBDA TBITLEFT TBITRIGHT TARROW TCOMMA TOP TCP
%type <node> expr ident number function prototype call_args
%type <args> args

%right '='
%left '-' '+'
%left '*' '/'
%left NEG

%start program

%%

program     : /* empty */
            | expr { root = $1; }
            | function { root = $1; }
            ;

ident       : TIDENTIFIER { $$ = ast_variable_create($1); };

number      : TNUMBER { $$ = ast_number_create($1); };

args        : /* empty */ { $$.count = 0; $$.args = NULL; }
            | TIDENTIFIER { $$.count = 1; $$.args = malloc(sizeof(char*)); $$.args[0] = strdup($1); }
            | args TCOMMA TIDENTIFIER { $1.count++; $1.args = realloc($1.args, sizeof(char*) * $1.count);
                                        $1.args[$1.count-1] = strdup($3); $$ = $1; }
            ;

prototype   : TIDENTIFIER TOP args TCP { $$ = ast_prototype_create("", $3.args, $3.count);
                                              free_args((void**)$3.args, $3.count); }
            | args { $$ = ast_prototype_create("", $1.args, $1.count); free_args((void**)$1.args, $1.count); }
            ;

call_args   : /*blank*/ { $$.count = 0; $$.args = NULL; }
            | expr { $$.count = 1; $$.args = malloc(sizeof(ast_node*)); $$.args[0] = $1; }
            | call_args expr { $1.count++; $1.args = realloc($1.args, sizeof(ast_node*) * $1.count); $1.args[$1.count - 1] = $2; $$ = $1; }
            ;

function    : TLAMBDA prototype TARROW expr { $$ = ast_function_create($2, $4); };

expr        : expr TMUL  expr { $$ = ast_binary_expr_create(BINOP_MUL, $1, $3); }
            | expr TDIV  expr { $$ = ast_binary_expr_create(BINOP_DIV, $1, $3); }
            | expr TMIN  expr { $$ = ast_binary_expr_create(BINOP_MIN, $1, $3); }
            | expr TPLUS expr { $$ = ast_binary_expr_create(BINOP_PLUS, $1, $3); }
            | number
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


void free_args(void **args, int count)
{
    int i;
    for(i=0; i<count; i++) {
        free(args[i]);
    }
    free(args);
}
