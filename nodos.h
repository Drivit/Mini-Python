#ifndef NODOS_H
#define NODOS_H

#include <fstream>
#include <vector>
#include <map>
#include "token.h"

void generarTablaSASM(std::fstream & archivo);

class Nodo
{
public:
    char tipo;
    std::string contenido_;

    virtual void calcularTipo();
    virtual void imprime(std::fstream & archivo);
    virtual void generaCodigo(std::fstream & archivo);
    Nodo();

    void errorRes();
};

class Expresion : public Nodo
{
public:
    Nodo * sigExp_;
    Nodo * antExp_;

    Expresion();
};

class Identificador : public Expresion
{
public:
    Identificador(std::string contenido);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

class Entero : public Expresion
{
public:
    Entero(std::string contenido);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

class Flotante : public Expresion
{
public:
    Flotante(std::string contenido);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

class Aritmetica : public Expresion
{
public:
    Aritmetica(std::string operador, Nodo * antExp, Nodo * sigExp);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

class Relacional : public Expresion
{
public:
    Relacional(std::string operador, Nodo * antExp, Nodo * sigExp);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

class Signo : public Expresion
{
public:
    Signo(std::string operador, Nodo * sigExp);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

class Sentencia : public Nodo
{
public:
    Nodo * sigSentencia_;

    Sentencia();
};

class Asignacion : public Sentencia
{
public:
    Nodo * id_;
    Nodo * exp_;

    Asignacion(Nodo * id, Nodo * exp, Nodo * sig);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

class Iteracion : public Sentencia
{
public:
    Nodo * exp_;
    Nodo * cuerpoSentencias_;

    Iteracion(Nodo * exp, Nodo * cuerpoSentencias, Nodo * sig);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

class Seleccion : public Sentencia
{
public:
    Nodo * exp_;
    Nodo * cuerpoSentencias_;
    Nodo * otraSentencia_;

    Seleccion(std::string tipo, Nodo * exp, Nodo * cuerpoSentencias, Nodo * otraSentencia, Nodo * sig);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

class Imprimir : public Sentencia
{
public:
    Nodo * exp_;
    Imprimir(Nodo * exp, Nodo * sig);

    void imprime(std::fstream & archivo);

    void calcularTipo();

    void generaCodigo(std::fstream & archivo);
};

#endif // NODOS_H
