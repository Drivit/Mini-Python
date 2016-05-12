#include "nodos.h"

int numeroEtiqueta = 0;
int numeroLoop = 0;
std::map<std::string, char> tablaSim;

void Nodo::calcularTipo(){}


void Nodo::imprime(std::fstream &archivo){
    archivo << contenido_;
}

void Nodo::generaCodigo(std::fstream &archivo)
{}


Nodo::Nodo(){
    this->contenido_ = "";
}


void Nodo::errorRes()
{
    std::fstream salida("salida.txt", std::ios::out);

    if(salida.is_open())
    {
        salida << 0;
    }

    salida.close();
    exit(0);
}


Expresion::Expresion() {
    this->sigExp_ = NULL;
    this->antExp_ = NULL;
}


Identificador::Identificador(std::string contenido) {
    this->contenido_ = contenido;
    this->sigExp_ = NULL;
    this->antExp_ = NULL;
}


void Identificador::imprime(std::fstream &archivo)
{
    archivo << "<ID>";
    Nodo::imprime(archivo);
    archivo << "</ID>\n";
}


void Identificador::calcularTipo()
{
    std::map<std::string, char>::iterator pos;
    pos = tablaSim.find(this->contenido_);
    if(pos != tablaSim.end())
    {
        this->tipo = tablaSim.find(this->contenido_)->second;
    }
    else
    {
        std::cout << "Error Semantico: Variable: " << this->contenido_ << " no declarada aun" << std::endl;
        errorRes();
    }
}

void Identificador::generaCodigo(std::fstream &archivo)
{
    archivo << "push " << this->contenido_ + "_" << std::endl;
}


Entero::Entero(std::string contenido) {
    this->contenido_ = contenido;
    this->sigExp_ = NULL;
    this->antExp_ = NULL;
}


void Entero::imprime(std::fstream &archivo)
{
    archivo << "<ENTERO>";
    Nodo::imprime(archivo);
    archivo << "</ENTERO>\n";
}


void Entero::calcularTipo()
{
    this->tipo = 'i';
}

void Entero::generaCodigo(std::fstream &archivo)
{
    archivo << "push " << this->contenido_ << std::endl;
}


Flotante::Flotante(std::string contenido) {
    this->contenido_ = contenido;
    this->sigExp_ = NULL;
    this->antExp_ = NULL;
}


void Flotante::imprime(std::fstream &archivo)
{
    archivo << "<REAL>";
    Nodo::imprime(archivo);
    archivo << "</REAL>\n";
}


void Flotante::calcularTipo()
{
    this->tipo = 'r';
}

void Flotante::generaCodigo(std::fstream &archivo)
{
    archivo << "push " << this->contenido_ << std::endl;
}


Aritmetica::Aritmetica(std::string operador, Nodo *antExp, Nodo *sigExp) {
    this->contenido_ = operador;
    this->antExp_ = antExp;
    this->sigExp_ = sigExp;
}


void Aritmetica::imprime(std::fstream &archivo)
{
    if(this->contenido_ =="+" || this->contenido_ == "-")
    {
        archivo << "<SUMA value=\"" << this->contenido_ << "\">\n";
        if(this->antExp_)
            this->antExp_->imprime(archivo);
        if(this->sigExp_)
            this->sigExp_->imprime(archivo);
        archivo << "</SUMA>\n";
    }
    else
    {
        archivo << "<MULT value=\"" << this->contenido_ << "\">\n";
        if(this->antExp_)
            this->antExp_->imprime(archivo);
        if(this->sigExp_)
            this->sigExp_->imprime(archivo);
        archivo << "</MULT>\n";
    }
}


void Aritmetica::calcularTipo()
{
    this->antExp_->calcularTipo();
    this->sigExp_->calcularTipo();

    if(this->sigExp_->tipo == this->antExp_->tipo)
        this->tipo = this->sigExp_->tipo;
    else
    {
        std::cout << "Error Semantico: operacion aritmetica con diferentes tipos de datos" << std::endl;
        errorRes();
    }
}

void Aritmetica::generaCodigo(std::fstream &archivo)
{
    this->antExp_->generaCodigo(archivo);
    this->sigExp_->generaCodigo(archivo);

    archivo << "pop ebx" << std::endl;
    archivo << "pop eax" << std::endl;

    if(this->contenido_ == "+")
    {
        archivo << "add eax, ebx" << std::endl;
        archivo << "push eax" << std::endl;
    }
    else if(this->contenido_ == "-")
    {
        archivo << "sub eax, ebx" << std::endl;
        archivo << "push eax" << std::endl;
    }
    else if(this->contenido_ == "*")
    {
        archivo << "imul eax, ebx" << std::endl;
        archivo << "push eax" << std::endl;
    }
    else if(this->contenido_ == "/")
    {
        archivo << "xor edx, edx" << std::endl;
        archivo << "idiv ebx" << std::endl;
        archivo << "push eax" << std::endl;
    }
    else if(this->contenido_ == "%")
    {
        archivo << "xor edx, edx" << std::endl;
        archivo << "idiv ebx" << std::endl;
        archivo << "push edx" << std::endl;
    }
}


Relacional::Relacional(std::string operador, Nodo *antExp, Nodo *sigExp) {
    this->contenido_ = operador;
    this->antExp_ = antExp;
    this->sigExp_ = sigExp;
}


void Relacional::imprime(std::fstream &archivo)
{
    std::string simbolo = "";
    if(this->contenido_ =="<")
        simbolo = "&lt;";
    else if(this->contenido_ == ">")
        simbolo = "&gt;";
    else if(this->contenido_ == "<=")
        simbolo = "&lt;=";
    else if(this->contenido_ == ">=")
        simbolo = "&gt;=";
    else
        simbolo = this->contenido_;

    archivo << "<EXPRESION value=\"" << simbolo << "\">\n";
    if(this->antExp_)
        this->antExp_->imprime(archivo);
    if(this->sigExp_)
        this->sigExp_->imprime(archivo);
    archivo << "</EXPRESION>\n";
}


void Relacional::calcularTipo()
{
    this->antExp_->calcularTipo();
    this->sigExp_->calcularTipo();

    if(this->sigExp_->tipo == this->antExp_->tipo)
        this->tipo = this->sigExp_->tipo;
    else
    {
        std::cout << "Error Semantico: operacion relacional con diferentes tipos de datos" << std::endl;
        errorRes();
    }
}

void Relacional::generaCodigo(std::fstream &archivo)
{
    this->antExp_->generaCodigo(archivo);
    this->sigExp_->generaCodigo(archivo);

    int etiqueta = numeroEtiqueta;
    int fin = ++numeroEtiqueta;

    archivo << "pop eax" << std::endl;
    archivo << "pop ebx" << std::endl;

    archivo << "cmp ebx, eax" << std::endl;

    if(this->contenido_ == ">")
        archivo << "jle ";
    else if(this->contenido_ == "<")
        archivo << "jge ";
    else if(this->contenido_ == ">=")
        archivo << "jl ";
    else if(this->contenido_ == "<=")
        archivo << "jg ";
    else if(this->contenido_ == "==")
        archivo << "jne ";
    else if(this->contenido_ == "!=")
        archivo << "je ";

    archivo << "FALSE_" << etiqueta << std::endl;

    archivo << "push 1" << std::endl;
    archivo << "jmp FIN_" << fin << std::endl;
    archivo << "FALSE_" << etiqueta << ":" << std::endl;
    archivo << "push 0" << std::endl;
    archivo << "FIN_" << fin << ":" << std::endl;

    numeroEtiqueta++;
}


Signo::Signo(std::string operador, Nodo *sigExp) {
    this->contenido_ = operador;
    this->sigExp_ = sigExp;
}


void Signo::imprime(std::fstream &archivo)
{
    archivo << "<SIGNO value=\"" << this->contenido_ << "\">\n";
    if(this->sigExp_)
        this->sigExp_->imprime(archivo);
    archivo << "</SIGNO>\n";
}


void Signo::calcularTipo()
{
    this->sigExp_->calcularTipo();
    this->tipo = this->sigExp_->tipo;
}

void Signo::generaCodigo(std::fstream &archivo)
{
    this->sigExp_->generaCodigo(archivo);

    archivo << "pop eax" << std::endl;
    archivo << "neg eax" << std::endl;
    archivo << "push eax" << std::endl;
}


Sentencia::Sentencia() {
    this->sigSentencia_ = NULL;
}


Asignacion::Asignacion(Nodo *id, Nodo *exp, Nodo *sig) {
    this->id_ = id;
    this->exp_ = exp;
    this->sigSentencia_ = sig;
}


void Asignacion::imprime(std::fstream &archivo)
{
    archivo << "<ASIGNACION>\n";
    this->id_->imprime(archivo);
    if(this->exp_)
        this->exp_->imprime(archivo);
    archivo << "</ASIGNACION>\n";

    if(this->sigSentencia_)
        this->sigSentencia_->imprime(archivo);
}


void Asignacion::calcularTipo()
{
    this->exp_->calcularTipo();

    std::map<std::string, char>::iterator pos;

    pos = tablaSim.find(this->id_->contenido_);

    if(pos == tablaSim.end())
    {
        tablaSim.insert(std::pair<std::string, char>(this->id_->contenido_, this->exp_->tipo));
        this->tipo = 'v';
    }

    if(this->sigSentencia_)
        this->sigSentencia_->calcularTipo();
}

void Asignacion::generaCodigo(std::fstream &archivo)
{
    this->exp_->generaCodigo(archivo);

    archivo << "pop eax" << std::endl;
    archivo << "mov " << this->id_->contenido_ + "_" << ", eax" << std::endl;

    if(this->sigSentencia_)
        sigSentencia_->generaCodigo(archivo);
}


Iteracion::Iteracion(Nodo *exp, Nodo *cuerpoSentencias, Nodo *sig) {
    this->exp_ = exp;
    this->cuerpoSentencias_ = cuerpoSentencias;
    this->sigSentencia_ = sig;
}


void Iteracion::imprime(std::fstream &archivo)
{
    archivo << "<MIENTRAS>\n";
    this->exp_->imprime(archivo);

    archivo << "<BLOQUE>\n";
    this->cuerpoSentencias_->imprime(archivo);
    archivo << "</BLOQUE>\n";

    archivo << "</MIENTRAS>\n";

    if(this->sigSentencia_)
        this->sigSentencia_->imprime(archivo);
}


void Iteracion::calcularTipo()
{
    this->exp_->calcularTipo();
    this->cuerpoSentencias_->calcularTipo();

    if(this->sigSentencia_)
        sigSentencia_->calcularTipo();
}

void Iteracion::generaCodigo(std::fstream &archivo)
{
    int etiqueta = numeroEtiqueta;
    int fin = ++numeroEtiqueta;
    numeroEtiqueta++;

    // Ecribir la etiqueta de brinco para la exp relacional
    archivo << "LOOP_" << etiqueta << ":" << std::endl;

    this->exp_->generaCodigo(archivo);

    archivo << "pop eax" << std::endl;
    archivo << "cmp eax, 0" << std::endl;
    archivo << "je FALSE_" << fin << std::endl;

    this->cuerpoSentencias_->generaCodigo(archivo);
    archivo << "jmp LOOP_" << etiqueta << std::endl;

    archivo << "FALSE_" << fin << ":" << std::endl;

    if(this->sigSentencia_)
        sigSentencia_->generaCodigo(archivo);
}


Seleccion::Seleccion(std::string tipo, Nodo *exp, Nodo *cuerpoSentencias, Nodo *otraSentencia, Nodo *sig) {
    this->contenido_ = tipo;
    this->exp_ = exp;
    this->cuerpoSentencias_ = cuerpoSentencias;
    this->otraSentencia_ = otraSentencia;
    this->sigSentencia_ = sig;
}


void Seleccion::imprime(std::fstream &archivo)
{
    if(this->contenido_ == "if")
    {
        archivo << "<SI>\n";
        if(this->exp_)
            this->exp_->imprime(archivo);

        archivo << "<BLOQUE>\n";
        this->cuerpoSentencias_->imprime(archivo);
        archivo << "</BLOQUE>\n";

        if(otraSentencia_)
            otraSentencia_->imprime(archivo);

        archivo << "</SI>\n";

        if(this->sigSentencia_)
            this->sigSentencia_->imprime(archivo);
    }
    else
    {
        archivo << "<OTRO>\n";

        this->cuerpoSentencias_->imprime(archivo);

        archivo << "</OTRO>\n";
    }
}


void Seleccion::calcularTipo()
{
    if(this->exp_)
        this->exp_->calcularTipo();

    this->cuerpoSentencias_->calcularTipo();

    if(this->sigSentencia_)
        this->sigSentencia_->calcularTipo();
}

void Seleccion::generaCodigo(std::fstream &archivo)
{
    int etiqueta = numeroEtiqueta;
    int fin  = ++numeroEtiqueta;
    numeroEtiqueta++;

    if(this->contenido_ == "if")
    {
        this->exp_->generaCodigo(archivo);

        archivo << "pop eax" << std::endl;
        archivo << "cmp eax, 0" << std::endl;
        archivo << "je FALSE_" << etiqueta << std::endl;

        this->cuerpoSentencias_->generaCodigo(archivo);
        archivo << "jmp FIN_" << fin << std::endl;

        // TODO: Terminar esta parte...
        archivo << "FALSE_" << etiqueta << ":" << std::endl;

        if(this->otraSentencia_)
            this->otraSentencia_->generaCodigo(archivo);

        archivo << "FIN_" << fin << ":" << std::endl;

        if(this->sigSentencia_)
            sigSentencia_->generaCodigo(archivo);
    }
    else
        this->cuerpoSentencias_->generaCodigo(archivo);
}


Imprimir::Imprimir(Nodo *exp, Nodo *sig) {
    this->exp_ = exp;
    this->sigSentencia_ = sig;
}


void Imprimir::imprime(std::fstream &archivo)
{
    archivo << "<IMPRIME>\n";

    archivo << "<EXPRESION>\n";
    if(this->exp_)
        this->exp_->imprime(archivo);
    archivo << "</EXPRESION>\n";

    archivo << "</IMPRIME>\n";

    if(this->sigSentencia_)
        this->sigSentencia_->imprime(archivo);
}


void Imprimir::calcularTipo()
{
    this->exp_->calcularTipo();

    if(this->sigSentencia_)
        this->sigSentencia_->calcularTipo();
}

void Imprimir::generaCodigo(std::fstream &archivo)
{
    this->exp_->generaCodigo(archivo);

    archivo << "pop eax" << std::endl;
    archivo << "print str$(eax), 10;" << std::endl;

    if(this->sigSentencia_)
        sigSentencia_->generaCodigo(archivo);
}


void generarTablaSASM(std::fstream &archivo)
{
    if(!tablaSim.empty())
    {
        std::map<std::string, char>::iterator iter;
        for(iter = tablaSim.begin(); iter != tablaSim.end(); ++iter)
        {
            archivo << iter->first + "_" << " dword 0" << std::endl;
        }
    }
}
