%{
    #include "ast.h"
    #include "parser.h"

    #define SAVE_TOKEN yylval.string = strndup(yytext, yyleng)
    #define SAVE_NUMBER yylval.number = atof(yytext)
    #define TOKEN(t) (yylval.token = t)
%}

%option noyywrap

ASC     [\x00-\x7f]
ASCN    [\x00-\t\v-\x7f]
U       [\x80-\xbf]
U2      [\xc2-\xdf]
U3      [\xe0-\xef]
U4      [\xf0-\xf4]
UANY    {ASC}|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}
UANYN   {ASCN}|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}
UONLY   {U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}

%%

[ \t\n]                 ;
[a-z_]*                 SAVE_TOKEN; return TIDENTIFIER;
[0-9]*                  SAVE_NUMBER; return TNUMBER;

"*"                     return TOKEN(TMUL);
"/"                     return TOKEN(TDIV);
"+"                     return TOKEN(TPLUS);
"-"                     return TOKEN(TMIN);
"<<"                    return TOKEN(TBITLEFT);
">>"                    return TOKEN(TBITRIGHT);
"\\"                    return TOKEN(TLAMBDA);
"->"                    return TOKEN(TARROW);
","                     return TOKEN(TCOMMA);
"("                     return TOKEN(TOP);
")"                     return TOKEN(TCP);

.                       printf("Unknown token!\n"); yyterminate();

%%

