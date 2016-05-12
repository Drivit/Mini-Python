#include "lexico.h"
#include "sintactico.h"
#include "arbol_sintactico.h"

int main(int argc, const char * argv[])
{   
    std::string file = (argc == 2) ? argv[1] : "entrada.txt";

    Lexico lex(file);
    lex.showTokens();

    Sintactico sintactico(lex.getTokensList());
    sintactico.analizar();

    Arbol_Sintactico arbol(lex.getTokensList());
    arbol.analizar();
    arbol.analisisSemantico();
    arbol.generarCodigoEnsamblador();
    return 0;
}
