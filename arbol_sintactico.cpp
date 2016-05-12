#include "arbol_sintactico.h"

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

Arbol_Sintactico::Arbol_Sintactico(std::vector<Token> listaTokens)
{
    tokenList_ = listaTokens;
    tokenActual_ = 0;
    raiz_ = NULL;
}

void Arbol_Sintactico::analizar()
{
    raiz_ = PROGRAMA();
    compruebaFin();
    guardarXML();
}

void Arbol_Sintactico::analisisSemantico()
{
    if(raiz_)
        raiz_->calcularTipo();
    else
        resultado(0);

    resultado(1);
}

void Arbol_Sintactico::generarCodigoEnsamblador()
{
    std::fstream salida("salida.txt", std::ios::out);

    if(!salida.is_open())
        ERROR("Ensamblador", "No se crear el archivo SASM");

    salida << ".386" << std::endl << std::endl;

    salida << ".model flat, stdcall" << std::endl;
    salida << "option casemap:none ;labels are case-sensitive now" << std::endl << std::endl;

    salida << "include \\masm32\\macros\\macros.asm" << std::endl;
    salida << "include \\masm32\\include\\masm32.inc" << std::endl;
    salida << "include \\masm32\\include\\kernel32.inc" << std::endl << std::endl;


    salida << "includelib \\masm32\\lib\\masm32.lib" << std::endl;
    salida << "includelib \\masm32\\lib\\kernel32.lib" << std::endl << std::endl;

    salida << ".data" << std::endl << std::endl;
    generarTablaSASM(salida);
    salida << "\n.data?" << std::endl << std::endl;


    salida << ".code" << std::endl;
    if(raiz_)
    {
        salida << "INICIO:" << std::endl;
        raiz_->generaCodigo(salida);
        salida << "exit" << std::endl;
        salida << "end INICIO";

        salida.close();
    }
    else
        resultado(0);
}

Sentencia *Arbol_Sintactico::PROGRAMA()
{
    Sentencia * aux = SENTENCIA(0);

    return aux;
}

Sentencia * Arbol_Sintactico::SENTENCIA(int opcion)
{
    Sentencia * aux = NULL;

    if(tokenActual() == IDENTIF)
    {
        if(tokenActualContent() == "print")
            aux = PRINT(opcion);
        else if(tokenActualContent() == "while")
            aux = WHILE(opcion);
        else if(tokenActualContent() == "if")
            aux = SELECCION(opcion);
        else
            aux = ASIGNACION_SENTENCIA(opcion);
    }

    return aux;
}

Sentencia *Arbol_Sintactico::SENTENCIA_P()
{
    Sentencia * aux = NULL;

    if(tokenActual() == NUEVA_LINEA)
    {
        sigToken();
        aux = SENTENCIA(0);
    }

    return aux;
}

Sentencia *Arbol_Sintactico::PRINT(int opcion)
{
    Sentencia * aux = NULL;
    Nodo * exp = NULL;

    comprueba("print");
    comprueba("(");
    exp = EXPRESION();
    comprueba(")");

    if(opcion == 0)
        aux = new Imprimir(exp, SENTENCIA_P());
    else
        aux = new Imprimir(exp, SUITE_P());

    return aux;
}

Sentencia *Arbol_Sintactico::SELECCION(int opcion)
{
    Nodo * exp = NULL;
    Sentencia * aux = NULL;
    Sentencia * otro = NULL;
    Sentencia * cuerpo = NULL;

    comprueba("if");
    exp = EXPRESION_REL();
    comprueba(":");
    cuerpo = SUITE();
    otro = ELSE();

    if(opcion == 0)
        aux = new Seleccion("if", exp, cuerpo, otro, SENTENCIA_P());
    else
        aux = new Seleccion("if", exp, cuerpo, otro, SUITE_P());

    return aux;
}

Sentencia *Arbol_Sintactico::ELSE()
{
    Sentencia * aux = NULL;
    Sentencia * temp = NULL;

    // Revision; caso en el que el if no tiene else y es el fin del archivo
    if(tokenActual() == NUEVA_LINEA)
        comprueba(NUEVA_LINEA);
    else if(tokenActual() == UNTAB)
        sigToken();

    if(tokenActual_ < tokenList_.size())
    {
        if(tokenActualContent() == "else")
        {
            sigToken();
            comprueba(":");
            temp = SUITE();

            aux = new Seleccion("else", NULL, temp, NULL, NULL);
        }
        else
            tokenActual_--;
    }

    return aux;
}

Sentencia *Arbol_Sintactico::WHILE(int opcion)
{
    Nodo * exp = NULL;
    Sentencia * aux = NULL;
    Sentencia * cuerpo = NULL;

    comprueba("while");
    exp = EXPRESION_REL();
    comprueba(":");
    cuerpo = SUITE();

    if(opcion == 0)
        aux = new Iteracion(exp, cuerpo, SENTENCIA_P());
    else
        aux = new Iteracion(exp, cuerpo, SUITE_P());

    return aux;
}

Sentencia *Arbol_Sintactico::ASIGNACION_SENTENCIA(int opcion)
{
    Nodo * exp = NULL;
    Nodo * id = NULL;
    Sentencia * aux = NULL;

    id = new Identificador(tokenActualContent());
    comprueba(IDENTIF);
    comprueba("=");
    exp = EXPRESION();

    if(opcion == 0)
        aux = new Asignacion(id, exp, SENTENCIA_P());
    else
        aux = new Asignacion(id, exp, SUITE_P());

    return aux;
}

Sentencia *Arbol_Sintactico::SUITE()
{
    Sentencia * aux = NULL;

    if(tokenActual() == NUEVA_LINEA)
    {
        sigToken();
        comprueba(TAB);
        aux = SENTENCIA(1);
        comprueba(UNTAB);
    }
    else
    {
        if(tokenActualContent() == "if" || tokenActualContent() == "while" || tokenActualContent() == "print" || tokenActual() == IDENTIF)
            aux = SENTENCIA(0);
    }

    return aux;
}

Sentencia *Arbol_Sintactico::SUITE_P()
{
    Sentencia * aux = NULL;

    if(tokenActual() == NUEVA_LINEA)
    {
        sigToken();
        if(tokenActual() != UNTAB)
            aux = SENTENCIA(1);
    }

    return aux;
}

Nodo * Arbol_Sintactico::EXPRESION()
{
    Nodo * aux = NULL;
    std::string simbolo = "";

    aux = MULTIPLICACION_SENTENCIA();

    while(tokenActual() == ADICION)
    {
        simbolo = tokenActualContent();
        sigToken();
        aux = new Aritmetica(simbolo, aux, MULTIPLICACION_SENTENCIA());
    }

    return aux;
}

Nodo *Arbol_Sintactico::EXPRESION_REL()
{
    Nodo * aux = NULL;
    Nodo * temp = EXPRESION();
    std::string operador = "";

    if(tokenActual() == OP_REL)
    {
        operador = tokenActualContent();
        sigToken();
        aux = new Relacional(operador, temp, EXPRESION());
    }
    else
        ERROR("SINTAXIS", "Operador relacional esperado");

    return aux;
}

Nodo * Arbol_Sintactico::MULTIPLICACION_SENTENCIA()
{
    Nodo * aux = NULL;
    std::string operador = "";

    aux = FACTOR();

    while(tokenActual() == MULTIPLICACION)
    {
        operador = tokenActualContent();
        sigToken();
        aux = new Aritmetica(operador, aux, FACTOR());
    }

    return aux;
}

Nodo *Arbol_Sintactico::FACTOR()
{
    Nodo * aux = NULL;
    std::string simbolo = "";

    // Unario
    if(tokenActual() == ADICION)
    {
        simbolo = tokenActualContent();
        sigToken();
        aux = new Signo(simbolo, FACTOR());
    }
    else
    {
        aux = ATOMO();
    }

    return aux;
}

Nodo * Arbol_Sintactico::ATOMO()
{
    Nodo * aux = NULL;
    std::string operador = "";

    if(tokenActualContent() == "(")
    {
        sigToken();
        aux = EXPRESION();
        comprueba(")");
    }
    else if(tokenActual() == IDENTIF)
    {
        operador = tokenActualContent();
        sigToken();
        aux = new Identificador(operador);
    }
    else if(tokenActual() == REAL)
    {
        operador = tokenActualContent();
        sigToken();
        aux = new Flotante(operador);
    }
    else if(tokenActual() == ENTERO)
    {
        operador = tokenActualContent();
        sigToken();
        aux = new Entero(operador);
    }
    //else ERROR("SINTAXIS", "Hace falta una expresion"); // TODO: Revisar precedencia

    return aux;
}


int Arbol_Sintactico::tokenActual() const
{
    return tokenList_[tokenActual_].getType();
}


std::string Arbol_Sintactico::tokenActualContent() const
{
    return tokenList_[tokenActual_].getContent();
}


void Arbol_Sintactico::sigToken()
{
    tokenActual_++;
}


void Arbol_Sintactico::comprueba(const int tipo)
{
    if(tokenActual_ >= tokenList_.size())
        ERROR("FIN", "Se llego al final del archivo, ultimo analisis: ");

    if(tokenActual() == tipo)
        sigToken();
    else
        ERROR(tipo);
}


void Arbol_Sintactico::comprueba(const std::string content)
{
    if(tokenActual_ >= tokenList_.size())
        ERROR("FIN", "Se llego al final del archivo, ultimo analisis: ");

    if(tokenActualContent() == content)
        sigToken();
    else
        ERROR(content);
}

void Arbol_Sintactico::ERROR(const std::string TKN)
{
    std::cout << "ERROR ARBOL: Token requerido: " << TKN << " -- Token recibido: " << tipoToken(tokenActual()) << std::endl;
    std::cout << "Ultimo token analizado: " << tokenActual_ << std::endl;
    resultado(0);
    exit(0);
}

void Arbol_Sintactico::ERROR(const std::string TIPO, std::string MSG)
{
    if(TIPO == "FIN")
    {
        std::cout << "ERROR ARBOL: " << TIPO << "; " << MSG << tokenActual_ << std::endl;
    }
    else
    {
        std::cout << "ERROR ARBOL: " << TIPO << "; " << MSG << std::endl;
        std::cout << "Ultimo token analizado: " << tokenActual_ << std::endl;
    }

    resultado(0);
    exit(0);
}

void Arbol_Sintactico::ERROR(const int TKN)
{
    std::cout << "ERROR ARBOL: Token requerido: " << tipoToken(TKN) << " -- Token recibido: " << tipoToken(tokenActual()) << std::endl;
    std::cout << "Ultimo token analizado: " << tokenActual_ << std::endl;
    resultado(0);
    exit(0);
}

std::string Arbol_Sintactico::tipoToken(const int TKN)
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

void Arbol_Sintactico::compruebaFin()
{
    if(tokenActual_ == tokenList_.size())
    {
        std::cout << "Arbol completado con exito" << std::endl;
        resultado(1);
    }
    else
    {
        ERROR("FIN", "No se termino el analisis del archivo, ultimo token analizado: ");
    }
}

void Arbol_Sintactico::resultado(const int resultado)
{
    std::fstream salida("salida.txt", std::ios::out);

    if(!salida.is_open())
    {
        std::cout << "No se pudo crear el archivo" << std::endl;
        exit(0);
    }

    salida << resultado;
}

void Arbol_Sintactico::guardarXML()
{
    std::fstream salida("salida.txt", std::ios::out);

    if(!salida.is_open())
        resultado(0);

    salida << "<PROGRAMA>\n";
    if(raiz_)
        raiz_->imprime(salida);
    salida << "</PROGRAMA>\n";

    salida.close();
}


