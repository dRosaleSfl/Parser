#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 16

typedef struct nodo{
    char *nombre;
    int current;
    struct nodo *elementos[100];
} * Nodo;

Nodo programa(void);
Nodo lista_declaracion(void);
Nodo seleccion(void);
Nodo iteracion(void);
Nodo repeticion(void);
Nodo sent_read(void);
Nodo sent_write(void);
Nodo b_expresion(void);

char tkn [MAXCHAR];
FILE * ftkn, * parsed;
int tknum(void);
void error(char errM [100]);
int chk = 1;
int lvl = 0;
char ext[1000000];

int tknum(void) {
    int temp = 0;
    if (strcmp(tkn, "PROGRAM\n") == 0) temp = 1;
    else if ( strcmp(tkn, "COMMENTMUL\n") == 0) temp = 2;
    else if ( strcmp(tkn, "COMMENT\n") == 0) temp = 3;
    else if ( strcmp(tkn, "IDENTIFICADOR\n") == 0) temp = 4;
    else if ( strcmp(tkn, "INT\n") == 0) temp = 5;
    else if ( strcmp(tkn, "FLOAT\n") == 0) temp = 6;
    else if ( strcmp(tkn, "BOOL\n") == 0) temp = 7;
    else if ( strcmp(tkn, "IF\n") == 0) temp = 8;
    else if ( strcmp(tkn, "ELSE\n") == 0) temp = 9;
    else if ( strcmp(tkn, "FI\n") == 0) temp = 10;
    else if ( strcmp(tkn, "DO\n") == 0) temp = 11;
    else if ( strcmp(tkn, "UNTIL\n") == 0) temp = 12;
    else if ( strcmp(tkn, "WHILE\n") == 0) temp = 13;
    else if ( strcmp(tkn, "FLOAT_RES\n") == 0) temp = 14;
    else if ( strcmp(tkn, "INT_RES\n") == 0 ) temp = 15;
    else if ( strcmp(tkn, "BOOL_RES\n") == 0) temp = 16;
    else if ( strcmp(tkn, "NOT\n") == 0) temp = 17;
    else if ( strcmp(tkn, "AND\n") == 0) temp = 18;
    else if ( strcmp(tkn, "OR\n") == 0) temp = 19;
    else if ( strcmp(tkn, "TRUE\n") == 0) temp = 20;
    else if ( strcmp(tkn, "FALSE\n") == 0) temp = 21;
    else if ( strcmp(tkn, "OPERADOR_MAS\n") == 0) temp = 22;                                // +
    else if ( strcmp(tkn, "COMMENT_SYM\n") == 0) temp = 23;
    else if ( strcmp(tkn, "END\n") == 0) temp = 24;                                         // ;
    else if ( strcmp(tkn, "COMMA\n") == 0) temp = 25;                                       // ,
    else if ( strcmp(tkn, "COMPARACION\n") == 0) temp = 26;
    else if ( strcmp(tkn, "READ_RES\n") == 0) temp = 27;
    else if ( strcmp(tkn, "WRITE_RES\n") == 0) temp = 28;
    else if ( strcmp(tkn, "PAROP\n") == 0) temp = 29;                                       // (
    else if ( strcmp(tkn, "PARCL\n") == 0) temp = 30;                                       // )
    else if ( strcmp(tkn, "OPEN\n") == 0) temp = 31;                                        // {
    else if ( strcmp(tkn, "CLOSE\n") == 0) temp = 32;                                       // }
    else if ( strcmp(tkn, "OPERADOR_MENOS\n") == 0 ) temp = 33;       
    else if ( strcmp(tkn, "THEN\n") == 0 ) temp = 34;                                       // -
    else if ( strcmp(tkn, "SYMASIG\n") == 0 ) temp = 35;                                    // =
    else if ( strcmp(tkn, "OPERADOR_MUL\n") == 0 ) temp = 36;                               // *
    else if ( strcmp(tkn, "OPERADOR_DIV\n") == 0 ) temp = 37;                               // /
    else{
        char err[100] = "\ntoken no identificado: ";
        strcat(err, tkn);
        error(err);
    } 
    return temp; 
}

void verArbol(Nodo arbol)  //recorrido en postorden
{
    int indx = 0;
    for (int i=0; i<lvl; i++) {
        strcat(ext, "----");
    }
    strcat(ext, ">");
    strcat(ext, arbol->nombre);
    strcat(ext, "\n");
    while(arbol->elementos[indx] != NULL) {
        lvl++;
        verArbol(arbol->elementos[indx]);
        lvl--;
        indx++;
    } 

}

void error(char errM [100])
{
    strcat(ext, "Error sintactico: ");
    strcat(ext, errM);
    parsed = fopen ("parsed.txt", "w");
    fprintf (parsed, "%s", ext);
    fclose(parsed);
    exit(1);
}

void igualar( char tokenActual [MAXCHAR] )
{
    if ( strcmp(tokenActual, tkn) == 0) {
        printf("igualar: %s", tkn);
        fgets(tkn, MAXCHAR, ftkn);
    }
    else {
        char err [100] = "No se esparaba el token ";
        strcat( err, tkn);
        strcat( err, "token Actual: ");
        strcat ( err, tokenActual);
        error( err );
    }
    return;
}

Nodo creaNodo(char s[])
{
    Nodo t = (Nodo) malloc(sizeof(struct nodo));
	t->nombre = s;
    t->current = 0;
    int i ;
    for (i = 0; i < 100; i++) t->elementos[i] = NULL;
	return t;
}

void check4comments(void) {
    if ( tknum() == 2 ){
        igualar("COMMENTMUL\n");
    }
    else if ( tknum() == 3) {
        igualar("COMMENT\n");
    }
}

int main(void)
{
    ftkn = fopen("rsl.txt", "r");
    if (ftkn == NULL){
        error("problemas con el archivo :(");
    }
    fgets(tkn, MAXCHAR, ftkn);
    Nodo raiz = NULL; //aterrizo raiz 
    raiz = programa();
    fclose(ftkn);
    verArbol(raiz);
    printf("\n%s", ext);
    parsed = fopen ("parsed.txt", "w");
    fprintf (parsed, "%s", ext);
    fclose(parsed);
    return 0;
}

Nodo programa (void) {
    Nodo temp, nuevo, temp2;
    int whledo = 0;
    if ( tknum() == 1 ) { // if token = "PROGRAM"
        igualar("PROGRAM\n");
        check4comments();
        nuevo = creaNodo("PROGRAM");
    }
    else {
        switch ( tknum() ) {

            case 2:
                igualar("COMMENTMUL\n");
            break;

            case 3:
                igualar("COMMENT\n");
            break;

            case 9: // else, osea if
                igualar("ELSE\n");
                check4comments();
                nuevo = creaNodo("ELSE");
            break;

            case 11: //do, osea do
                whledo = 1;
            break;

            case 31: // {, osea while o bloque
                whledo = 1;
            break;

            case 34: // then, osea if
                igualar("THEN\n");
                check4comments();
                nuevo = creaNodo("THEN");
            break;
        }
    }
    igualar("OPEN\n"); // Por el momento esta esta bien...
    temp = creaNodo("{");
    while ( strcmp(tkn, "CLOSE\n") ) {
        switch ( tknum() ) {    // { program ”{” lista-declaración lista-sentencias ”}”

            case 2:
                igualar("COMMENTMUL\n");
            break;

            case 3:
                igualar("COMMENT\n");
            break;

            // lista-declaración 
            case 14: // FLOAT_RES
                temp2 = lista_declaracion();
                temp->elementos[temp->current] = temp2;
                temp->current++;
            break;

            case 15: // INT_RES
                temp2 = lista_declaracion();
                temp->elementos[temp->current] = temp2;
                temp->current++;
            break;

            case 16: // BOOL_RES
                temp2 = lista_declaracion();
                temp->elementos[temp->current] = temp2;
                temp->current++;
            break;

            //lista-sentenias
            case 8: // if
                temp->elementos[temp->current] = seleccion();
                temp->current++;
                temp->elementos[temp->current] = creaNodo("FI");
                temp->current++;
            break;

            case 13: //while
                temp->elementos[temp->current] = iteracion();
                temp->current++;
            break;

            case 11: //do
                temp->elementos[temp->current] = repeticion();
                temp->current++;
            break;

            case 27: // read
                temp->elementos[temp->current] = sent_read();
                temp->current++;
            break;

            case 28: // write ::= write b-expresión ”;”
                temp->elementos[temp->current] = sent_write();
                temp->current++;
            break;

            case 31: // { bloque
                temp->elementos[temp->current] = programa();
                temp->current++;
                igualar("CLOSE\n");
                check4comments();
                temp->elementos[temp->current] = creaNodo("}");
                temp->current++;
            break;

            case 4: // identificador
                ;
                temp2 = creaNodo("ASIGNACION");
                igualar("IDENTIFICADOR\n");
                check4comments();
                temp2->elementos[temp2->current] = creaNodo("IDENT");
                temp2->current++;
                igualar("SYMASIG\n");
                check4comments();
                temp2->elementos[temp2->current] = creaNodo("=");
                temp2->current++;
                Nodo temp3 = b_expresion();
                for (int i = 0; i<temp3->current; i++) {
                    temp2->elementos[temp2->current] = temp3->elementos[i];
                    temp2->current++;
                }
                igualar("END\n");
                check4comments();
                temp2->elementos[temp2->current] = creaNodo(";");
                temp2->current++;
                temp->elementos[temp->current] = temp2;
                temp->current++; 
            break;

            case 26: // <= | < | > | >= | == | !=
                igualar("COMPARACION\n");
                check4comments();
                temp->elementos[temp->current] = creaNodo("COMPARACION");
                temp->current++;
            break;

            default:
                //aqui deberia haber error
                printf("token: %s", tkn);
                fgets(tkn, MAXCHAR, ftkn);
            break;
        }
    }
    if ( strcmp(tkn, "CLOSE\n") == 0 ) {
        if ( whledo ) { // if whledo == 1, osea si fue while
           nuevo = temp;
        }else { // if whledo == 0, osea no fue while
            nuevo->elementos[nuevo->current] = temp;
            nuevo->current++;
            igualar("CLOSE\n");
            check4comments();
            nuevo->elementos[nuevo->current] = creaNodo("}");
            nuevo->current++;
        }
    }
    else {
        error("Corchete no cierra");
    }
    return nuevo;
}

Nodo lista_declaracion(void) {
    Nodo temp, nuevo;
    int tick = 0; // 0 = ident
                  // 1 = ,
    int bool = 1;
    nuevo = creaNodo("Declaracion");
    switch ( tknum() ) {

        case 2:
            igualar("COMMENTMUL\n");
        break;

        case 3:
            igualar("COMMENT\n");
        break;

        case 14: // float
            igualar("FLOAT_RES\n");
            check4comments();
            nuevo->elementos[nuevo->current] = creaNodo("FLOAT");
            nuevo->current++;
        break;

        case 15: // int
            igualar("INT_RES\n");
            check4comments();
            nuevo->elementos[nuevo->current] = creaNodo("INT");
            nuevo->current++;
        break;

        case 16: // bool
            igualar("BOOL_RES\n");
            check4comments();
            nuevo->elementos[nuevo->current] = creaNodo("BOOL");
            nuevo->current++;
        break;  
    }
    if ( strcmp(tkn, "IDENTIFICADOR\n") == 0 ) {
        igualar("IDENTIFICADOR\n");
        check4comments();
        nuevo->elementos[nuevo->current] = creaNodo("IDENT");
        nuevo->current++;
        while (bool) {
            switch( tknum() ) {
                case 2:
                    igualar("COMMENTMUL\n");
                break;

                case 3:
                    igualar("COMMENT\n");
                break;

                case 25: // ,
                    if (tick == 1) {
                        error ("Dos comas una tras otra");
                    }
                    else {
                        igualar("COMMA\n");
                        check4comments();
                        nuevo->elementos[nuevo->current] = creaNodo(",");
                        nuevo->current++;
                        tick = 1;
                    }
                break;

                case 4: // ident
                    if (tick == 0) {
                        error ("Dos identificadores sin coma de por medio");
                    }
                    else {
                        igualar("IDENTIFICADOR\n");
                        check4comments();
                        nuevo->elementos[nuevo->current] = creaNodo("IDENT");
                        nuevo->current++;
                        tick = 0;
                    }
                break;

                case 24: // ;
                    igualar("END\n");
                    check4comments();
                    nuevo->elementos[nuevo->current] = creaNodo(";");
                    nuevo->current ++;
                    bool = 0;
                break;

                default:
                    error( "no se esperaba el elemento" );
                break;
            }
        }
    }
    else {
        error ("Despues de un tipo debe de haber un identificador");
    }
    return nuevo;
}

Nodo seleccion(void) { // if ”(” b-expresión ”)” then bloque [ else bloque ] fi
    Nodo temp, nuevo;
    igualar("IF\n");
    check4comments();
    nuevo = creaNodo("IF");
    if ( tknum() == 29 ) { // (
        igualar("PAROP\n");
        check4comments();
        nuevo->elementos[nuevo->current] = b_expresion();
        nuevo->current++;
    }
    else {
        error("if sin '('\n");
    }
    igualar("PARCL\n");
    check4comments();
    nuevo->elementos[nuevo->current] = creaNodo(")");
    nuevo->current++;
    if ( tknum() == 34 ) { // tkn = THEN
        nuevo->elementos[nuevo->current] = programa();
        nuevo->current++;
    }
    else {
        error("Falta token 'then' despues de 'if'");
    }
    // ELSE
    if (tknum() == 9) { // tkn == else
        nuevo->elementos[nuevo->current] = programa();
        nuevo->current++;
    }
    // FI
    if (tknum() != 10) { // tkn != fi
        error("Sentencia if necesita terminar con fi\n");
    }
    igualar("FI\n");
    return nuevo;
}

Nodo iteracion (void) { // while ”(” b-expresión ”)” bloque
    Nodo nuevo, temp;
    igualar("WHILE\n");
    check4comments();
    nuevo = creaNodo("WHILE");
    if ( tknum() == 29 ) { // (
        igualar("PAROP\n");
        check4comments();
        nuevo->elementos[nuevo->current] = b_expresion();
        nuevo->current++;
    }
    else {
        error("while sin '('\n");
    }
    igualar("PARCL\n");
    check4comments();
    nuevo->elementos[nuevo->current] = creaNodo(")");
    nuevo->current++;
    if ( tknum() == 31 ) {
        nuevo->elementos[nuevo->current] = programa();
        nuevo->current++; 
        igualar("CLOSE\n");
        check4comments();
        nuevo->elementos[nuevo->current] = creaNodo("}");
        nuevo->current++; 
    }
    else {
        error("while sin { ");
    }
    return nuevo;
}

Nodo repeticion (void) { // do bloque until ”(” b-expresión ”)” ”;”
    Nodo nuevo, temp;
    igualar("DO\n");
    check4comments();
    nuevo = creaNodo("DO");
    if ( tknum() == 31 ) { // tkn == {
        nuevo->elementos[nuevo->current] = programa();
        nuevo->current++;
        igualar("CLOSE\n");
        check4comments();
        nuevo->elementos[nuevo->current] = creaNodo("}");
        nuevo->current++; 
    }
    else {
        error("While sin {\n");
    }
    if ( tknum() == 12 ) { // tkn == until
        igualar("UNTIL\n");
        check4comments();
        nuevo->elementos[nuevo->current] = creaNodo("UNTIL");
        igualar("PAROP\n");
        check4comments();
        temp = b_expresion();
        nuevo->elementos[nuevo->current]->elementos[0] = temp;
        igualar("PARCL\n");
        check4comments();
        nuevo->elementos[nuevo->current]->elementos[1] = creaNodo(")"); 
        nuevo->current++;
    }
    else {
        error("do sin until\n");
    } 
    if ( tknum() == 24 ) { // tkn == ;
        igualar("END\n");
        check4comments();
        nuevo->elementos[nuevo->current] = creaNodo(";");
        nuevo->current++;
    }
    else {
        error("Falta el ; al final\n");
    }
    return nuevo;
}

Nodo sent_read(void) { // read identificador ”;”
    Nodo nuevo;
    igualar("READ_RES\n");
    check4comments();
    nuevo = creaNodo("READ");
    igualar("IDENTIFICADOR\n");
    check4comments();
    nuevo->elementos[nuevo->current] = creaNodo("IDENT");
    nuevo->current++;
    igualar("END\n");
    check4comments();
    nuevo->elementos[nuevo->current] = creaNodo(";");
    nuevo->current++;
    return nuevo;
}

Nodo sent_write(void) { // write b-expresión ”;”
    Nodo nuevo;
    nuevo = b_expresion();
    igualar("END\n");
    check4comments();
    nuevo->elementos[nuevo->current] = creaNodo(";");
    nuevo->current++;
    return nuevo;
}

Nodo b_expresion(void) { 
    Nodo temp, nuevo;
    char errMs[20] = "Elemento: ";
    int wrte = 0;
    nuevo = creaNodo("(");
    if ( tknum() == 28 ) {
        igualar("WRITE_RES\n");
        check4comments();
        nuevo = creaNodo("WRITE");
    }
    if ( tknum() == 35 ) { // tkn == =
        igualar("SYMASIG\n");
        check4comments();
        nuevo = creaNodo("=");
    }
    while ( strcmp(tkn, "PARCL\n") && strcmp(tkn, "END\n") ) {
        switch ( tknum() ) {
            case 17: //NOT
                igualar("NOT\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("NOT");
                nuevo->current++;
            break;

            case 20: //true
                igualar("TRUE\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("TRUE");
                nuevo->current++;
                if (tknum() == 18 /* token == AND */) { //not-factor { AND not-factor })
                    igualar("AND\n");
                    check4comments();
                    nuevo->elementos[nuevo->current] = creaNodo("AND");
                    nuevo->current++;
                }
            break;

            case 21: //false
                igualar("FALSE\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("FALSE");
                nuevo->current++;
                if (tknum() == 18 /* token == AND */) { //not-factor { AND not-factor })
                    igualar("AND\n");
                    check4comments();
                    nuevo->elementos[nuevo->current] = creaNodo("AND");
                    nuevo->current++;
                }
            break;

            case 22: //operador + 
                igualar("OPERADOR_MAS\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("+");
                nuevo->current++;
            break;

            case 29: // (
                igualar("PAROP\n");
                check4comments();
                nuevo->elementos[nuevo->current] = b_expresion();
                nuevo->current++;
                igualar("PARCL\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo(")");
                nuevo->current++;
            break;

            case 33: //operador -
                igualar("OPERADOR_MENOS\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("-");
                nuevo->current++;
            break;

            case 36: //operador *
                igualar("OPERADOR_MUL\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("*");
                nuevo->current++;
            break;

            case 37: //operador /
                igualar("OPERADOR_DIV\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("/");
                nuevo->current++;
            break;

            case 4: // IDENTIFICADOR
                igualar("IDENTIFICADOR\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("IDENT");
                nuevo->current++;
                if (tknum() == 18 /* token == AND */) { //not-factor { AND not-factor })
                    igualar("AND\n");
                    check4comments();
                    nuevo->elementos[nuevo->current] = creaNodo("AND");
                    nuevo->current++;
                }
            break;

            case 5: // INT
                igualar("INT\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("INT");
                nuevo->current++;
                if (tknum() == 18 /* token == AND */) { //not-factor { AND not-factor })
                    igualar("AND\n");
                    check4comments();
                    nuevo->elementos[nuevo->current] = creaNodo("AND");
                    nuevo->current++;
                }
            break;

            case 6: // FLOAT
                igualar("FLOAT\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("FLOAT");
                nuevo->current++;
                if (tknum() == 18 /* token == AND */) { //not-factor { AND not-factor })
                    igualar("AND\n");
                    check4comments();
                    nuevo->elementos[nuevo->current] = creaNodo("AND");
                    nuevo->current++;
                }
            break;

            case 26: // <= | < | > | >= | == | !=
                igualar("COMPARACION\n");
                check4comments();
                nuevo->elementos[nuevo->current] = creaNodo("COMPARACION");
                nuevo->current++;
            break;

            case 2:
                igualar("COMMENTMUL\n");
            break;

            case 3:
                igualar("COMMENT\n");
            break;

            default:
                printf("token actual: %s", tkn);
                error("Error en el b-expresion");
            break;
        }
    }
    return nuevo;
}