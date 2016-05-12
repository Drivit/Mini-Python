#ifndef ARBOL_SINTACTICO_H
#define ARBOL_SINTACTICO_H

#include "nodos.h"

class Arbol_Sintactico
{
public:
    Arbol_Sintactico(std::vector<Token> listaTokens);

    void analizar();
    void analisisSemantico();
    void generarCodigoEnsamblador();

private:
    unsigned int tokenActual_;
    std::vector<Token> tokenList_;

    Sentencia * raiz_;

    // Funciones de analisis

    Sentencia * PROGRAMA();
    Sentencia * SENTENCIA(int opcion);
    Sentencia * SENTENCIA_P();
    Sentencia * PRINT(int opcion);
    Sentencia * SELECCION(int opcion);
    Sentencia * ELSE();
    Sentencia * WHILE(int opcion);
    Sentencia * ASIGNACION_SENTENCIA(int opcion);
    Sentencia * SUITE();
    Sentencia * SUITE_P();
    Nodo * EXPRESION();
    Nodo * EXPRESION_REL();
    Nodo * COMPARACION();
    Nodo * ADICION_SENTENCIA(Nodo *ant);
    Nodo * MULTIPLICACION_SENTENCIA();
    Nodo * FACTOR();
    Nodo * ATOMO();

    // Funciones del analizador
    int tokenActual() const;
    std::string tokenActualContent() const;

    void sigToken();
    void comprueba(const int tipo);
    void comprueba(const std::string content);

    void ERROR(const std::string TKN);
    void ERROR(const std::string TIPO, std::string MSG);
    void ERROR(const int TKN);

    // Obtener tipo de token

    std::string tipoToken(const int TKN);

    // Comprobar fin de analisis
    void compruebaFin();

    // Dar salida al resultado
    void resultado(const int resultado);
    void guardarXML();
};
#endif // ARBOL_SINTACTICO_H
