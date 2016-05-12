#include "sintactico.h"

enum {
    IDENTIF,
    DIGITO,
    ENTERO,
    REAL,
    CADENA,
    ADICION,
    MULTIPLICACION,
    PARENTESIS,
    DOS_PUNTOS,
    COMA,
    OP_LOG,
    OP_REL,
    ASIGNACION,
    NUEVA_LINEA,
    TAB,
    UNTAB
};

Sintactico::Sintactico(std::vector<Token> tokenList)
{
    tokenList_ = tokenList;
    tokenActual_ = 0;
}

Sintactico::~Sintactico()
{

}


void Sintactico::analizar()
{
    PROGRAMA();
    compruebaFin();
}

void Sintactico::PROGRAMA()
{
    SENTENCIA();

    while (tokenActual() == NUEVA_LINEA)
    {
        sigToken();
        SENTENCIA();
    }
}

void Sintactico::SENTENCIA()
{
    if(tokenActual() == IDENTIF)
    {
        if(tokenActualContent() == "print")
            PRINT();
        else if(tokenActualContent() == "while")
            WHILE();
        else if(tokenActualContent() == "if")
            SELECCION();
        else
            ASIGNACION_SENTENCIA();
    }
}

void Sintactico::PRINT()
{
    comprueba("print");
    comprueba("(");
    EXPRESION();
    comprueba(")");
}

void Sintactico::SELECCION()
{
    comprueba("if");
    EXPRESION();
    comprueba(":");
    SUITE();
    ELSE();
}

void Sintactico::ELSE()
{
    // Revision; caso en el que el if no tiene else y es el fin del archivo
    if(tokenActual() == NUEVA_LINEA || tokenActual() == UNTAB)
        sigToken();

    if(tokenActual_ < tokenList_.size())
    {
        if(tokenActualContent() == "else")
        {
            sigToken();
            comprueba(":");
            SUITE();
        }
        else
            tokenActual_--;
    }
}

void Sintactico::WHILE()
{
    comprueba("while");
    EXPRESION();
    comprueba(":");
    SUITE();
}

void Sintactico::ASIGNACION_SENTENCIA()
{
    comprueba(IDENTIF);
    comprueba("=");
    EXPRESION();
}

void Sintactico::SUITE()
{

    if(tokenActual() == NUEVA_LINEA)
    {
        sigToken();
        comprueba(TAB);
        SENTENCIA();

        while (tokenActual() == NUEVA_LINEA)
        {
            sigToken();
            if(tokenActual() != UNTAB)
                SENTENCIA();
        }

        comprueba(UNTAB);
    }
    else
    {
        if(tokenActualContent() == "if" || tokenActualContent() == "while" || tokenActualContent() == "print" || tokenActual() == IDENTIF)
            SENTENCIA();
    }
}

void Sintactico::EXPRESION()
{
    ADICION_SENTENCIA();

    while (tokenActual() == OP_REL)
    {
        sigToken();
        ADICION_SENTENCIA();
    }
}

void Sintactico::ADICION_SENTENCIA()
{
    MULTIPLICACION_SENTENCIA();

    while (tokenActual() == ADICION)
    {
        sigToken();
        MULTIPLICACION_SENTENCIA();
    }
}

void Sintactico::MULTIPLICACION_SENTENCIA()
{
    ATOMO();

    // Operador Unario
    while (tokenActual() == ADICION)
    {
        sigToken();
        ATOMO();
    }

    while (tokenActual() == MULTIPLICACION)
    {
        sigToken();
        ATOMO();
    }
}

void Sintactico::ATOMO()
{
    if(tokenActualContent() == "(")
    {
        sigToken();
        EXPRESION();
        comprueba(")");
    }
    else if(tokenActual() == IDENTIF || tokenActual() == REAL || tokenActual() == ENTERO)
    {
        sigToken();
    }
}


int Sintactico::tokenActual() const
{    
    return tokenList_[tokenActual_].getType();
}


std::string Sintactico::tokenActualContent() const
{
    return tokenList_[tokenActual_].getContent();
}


void Sintactico::sigToken()
{
    tokenActual_++;
}


void Sintactico::comprueba(const int tipo)
{
    if(tokenActual_ >= tokenList_.size())
        ERROR("FIN", "Se llego al final del archivo, ultimo analisis: ");

    if(tokenActual() == tipo)
        sigToken();
    else
        ERROR(tipo);
}


void Sintactico::comprueba(const std::string content)
{
    if(tokenActual_ >= tokenList_.size())
        ERROR("FIN", "Se llego al final del archivo, ultimo analisis: ");

    if(tokenActualContent() == content)
        sigToken();
    else
        ERROR(content);
}

void Sintactico::ERROR(const std::string TKN)
{
    std::cout << "ERROR SINTACTICO: Token requerido: " << TKN << " -- Token recibido: " << tipoToken(tokenActual()) << std::endl;
    std::cout << "Ultimo token analizado: " << tokenActual_ << std::endl;
    resultado(0);
    exit(0);
}

void Sintactico::ERROR(const std::string TIPO, std::string MSG)
{
    if(TIPO == "FIN")
    {
        std::cout << "ERROR SINTACTICO: " << TIPO << "; " << MSG << tokenActual_ << std::endl;
    }
    else
    {
        std::cout << "ERROR SINTACTICO: " << TIPO << "; " << MSG << std::endl;
        std::cout << "Ultimo token analizado: " << tokenActual_ << std::endl;
    }

    resultado(0);
    exit(0);
}

void Sintactico::ERROR(const int TKN)
{
    std::cout << "ERROR SINTACTICO: Token requerido: " << tipoToken(TKN) << " -- Token recibido: " << tipoToken(tokenActual()) << std::endl;
    std::cout << "Ultimo token analizado: " << tokenActual_ << std::endl;
    resultado(0);
    exit(0);
}

std::string Sintactico::tipoToken(const int TKN)
{
    std::string result = "";

    if(TKN == IDENTIF)
        result = "IDENTIF";
    else if(TKN == DIGITO)
        result = "DIGITO";
    else if(TKN == ENTERO)
        result = "ENTERO";
    else if(TKN == REAL)
        result = "REAL";
    else if(TKN == CADENA)
        result = "CADENA";
    else if(TKN == ADICION)
        result = "ADICION";
    else if(TKN == MULTIPLICACION)
        result = "MULTIPLICACION";
    else if(TKN == PARENTESIS)
        result = "PARENTESIS";
    else if(TKN == DOS_PUNTOS)
        result = "DOS_PUNTOS";
    else if(TKN == COMA)
        result = "COMA";
    else if(TKN == OP_LOG)
        result = "OP_LOG";
    else if(TKN == OP_REL)
        result = "OP_REL";
    else if(TKN == ASIGNACION)
        result = "ASIGNACION";
    else if(TKN == NUEVA_LINEA)
        result = "NUEVA_LINEA";
    else if(TKN == TAB)
        result = "TAB";
    else if(TKN == UNTAB)
        result = "UNTAB";


    return result;
}

void Sintactico::compruebaFin()
{
    if(tokenActual_ == tokenList_.size())
    {
        std::cout << "Analisis sintactico completado con exito" << std::endl;
        resultado(1);
    }
    else
    {
        ERROR("FIN", "No se termino el analisis del archivo, ultimo token analizado: ");
    }
}

void Sintactico::resultado(const int resultado)
{
    std::fstream salida("salida.txt", std::ios::out);

    if(!salida.is_open())
    {
        std::cout << "No se pudo crear el archivo" << std::endl;
        exit(0);
    }

    salida << resultado;
}
