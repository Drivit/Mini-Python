#ifndef SINTACTICO_H
#define SINTACTICO_H

#include <vector>
#include <fstream>
#include "token.h"

class Sintactico
{
    public:
        Sintactico(std::vector<Token> tokenList);
        ~Sintactico();

        void analizar();

    private:
        unsigned int tokenActual_;
        std::vector<Token> tokenList_;

        // Funciones de la gramatica

        void PROGRAMA();
        void SENTENCIA();
        void PRINT();
        void SELECCION();
        void ELSE();
        void WHILE();
        void ASIGNACION_SENTENCIA();
        void SUITE();
        void EXPRESION();
        void COMPARACION();
        void ADICION_SENTENCIA();
        void MULTIPLICACION_SENTENCIA();
        void ATOMO();

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
};

#endif
