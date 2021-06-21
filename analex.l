digito [0-9]
id [a-z|A-Z][a-z|A-Z]*
cml [/][\*]([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*[\*][/]
cm [/]+.*
program program|PROGRAM
if if|IF
then then|THEN
else else|ELSE
fi fi|FI
do do|DO
until until|UNTIL
while while|WHILE
read read|READ
write write|WRITE
float float|FLOAT
int int|INT
bool bool|BOOL
not not|NOT
and and|AND
or or|OR
true true|TRUE
false false|FALSE
simas [+]
simen [-]
simul [*]
simdiv [/]
simcmn [\*]|[\/]|[\^]
parop \(
parcl \)
simopn \{
closed \}
simcomp <|<=|>|>=|==|!= 
simasig =
simend ;
simcom ,


%{
    #include <stdio.h>
    enum yytokentype {
        PROGRAM = 10,
        CM = 20,
        MLCM = 30,
        IDENT = 40,
        RES_IF = 50,
        RES_THEN = 51,
        RES_ELSE = 60,
        RES_FI = 70,
        RES_DO = 80,
        RES_UNTIL = 90,
        RES_WHILE = 100,
        RES_READ = 110,
        RES_WRITE = 120,
        RES_FLOAT = 130,
        RES_INT = 140,
        RES_BOOL = 150,
        RES_NOT = 160,
        SYMAS = 190,
        SYMEN = 189,
        SYMCN = 191,
        SYMCOMP = 192,
        SYMEND = 193,
        SYMCOM = 194,
        SYMOPN = 195,
        SYMCLS = 196,
        SYMASIG = 197,
        PAROP = 198,
        PARCL = 199,
        RES_OR = 200,
        SYMUL = 201,
        SYMDIV = 202,
        INT = 220,
        FLOAT = 230,
        BOOLV = 235,
        BOOLF = 236,
        RES_AND = 240,
        EOL = 999
    };
    int yyval;
%}

%option yylineno

%%
{if} {return RES_IF;}
{then} {return RES_THEN;}
{else} {return RES_ELSE;}
{fi} {return RES_FI;}
{do} {return RES_DO;}
{until} {return RES_UNTIL;}
{while} {return RES_WHILE;}
{read} {return RES_READ;}
{write} {return RES_WRITE;}
{float} {return RES_FLOAT;}
{int} {return RES_INT;}
{bool} {return RES_BOOL;}
{not} {return RES_NOT;}
{and} {return RES_AND;}
{or} {return RES_OR;}
{program} {return PROGRAM;}
{simul} {return SYMUL;}
{simdiv} {return SYMDIV;}
{cml} {return MLCM;}
{cm} {return CM;}
{digito}+ {return INT;}
{digito}+"."{digito}+ {return FLOAT;}
{true} {return BOOLV;}
{false} {return BOOLF;}
{id} {return IDENT;}
{simas} {return SYMAS;} // + 
{simen} {return SYMEN;} // -
{simcmn} {return SYMCN;} // // /* */
{simend} {return SYMEND;} // ;
{simcom} {return SYMCOM;} // ,
{simcomp} {return SYMCOMP;} // < = >
{simasig} {return SYMASIG;}
{closed} {return SYMCLS;}
{simopn} {return SYMOPN;}
{parop} {return PAROP;}
{parcl} {return PARCL;}
\n {}
%%

FILE * fp;

int yywrap() {
    return 1;
} 

int main (int argc, char **argv) {
    ++argv; --argc;
    if(argc > 0){
        yyin = fopen( argv[0], "r");
        if (yyin == NULL){
            printf("problemas con el archivo :(");
            return 1;
        }
    }
    else {
        yyin = stdin;
    }

    fp = fopen ("rsl.txt", "w");
    

    printf("Analisis Lexico programa %s \n", argv[0]);
    int tok;
    char *tkn;

    while( tok = yylex() ) {
        switch(tok) {
            case(PROGRAM):
                tkn = "PROGRAM";
            break;

            case(MLCM):
                tkn = "COMMENTMUL";
            break;

            case(CM):
                tkn = "COMMENT";
            break;

            case(IDENT):
                tkn = "IDENTIFICADOR";
            break;

            case(INT):
                tkn = "INT";
            break;

            case(FLOAT):
                tkn = "FLOAT";
            break;

            case(BOOLV):
                tkn = "TRUE";
            break;

            case(BOOLF):
                tkn = "FALSE";
            break;

            case(RES_IF):
                tkn = "IF";
            break;

            case(RES_THEN):
                tkn = "THEN";
            break;

            case(RES_ELSE):
                tkn = "ELSE";
            break;

            case(RES_FI):
                tkn = "FI";
            break;

            case(RES_DO):
                tkn = "DO";
            break;

            case(RES_UNTIL):
                tkn = "UNTIL";
            break;

            case(RES_WHILE):
                tkn = "WHILE";
            break;

            case(RES_FLOAT):
                tkn = "FLOAT_RES";
            break;

            case(RES_READ): 
                tkn = "READ_RES";
            break;

            case(RES_WRITE):
                tkn = "WRITE_RES";
            break;

            case(RES_INT):
                tkn = "INT_RES";
            break;

            case(RES_BOOL):
                tkn = "BOOL_RES";
            break;

            case(RES_NOT):
                tkn = "NOT";
            break;

            case(RES_AND):
                tkn = "AND";
            break;

            case(RES_OR):
                tkn = "OR";
            break;

            case(SYMAS):
                tkn = "OPERADOR_MAS";
            break;

            case(SYMEN):
                tkn = "OPERADOR_MENOS";
            break;

            case(SYMCN):
                tkn = "COMMENT_SYM";
            break;

            case(SYMEND):
                tkn = "END";
            break;

            case(SYMCOM):
                tkn= "COMMA";
            break;

            case(SYMCOMP):
                tkn = "COMPARACION";
            break;

            case(SYMASIG):
                tkn = "SYMASIG";
            break;

            case(SYMOPN):
                tkn = "OPEN";
            break;

            case(SYMCLS):
                tkn = "CLOSE";
            break;

            case(PAROP):
                tkn = "PAROP";
            break;

            case(PARCL):
                tkn = "PARCL";
            break;

            case(SYMUL):
                tkn = "OPERADOR_MUL";
            break;

            case(SYMDIV):
                tkn = "OPERADOR_DIV";
            break;

            case(EOL):
                tkn= "\n";
            break;
        }
        fprintf (fp, "%s\n", tkn);
    }
    fclose(fp);
    return 0;
}

    
