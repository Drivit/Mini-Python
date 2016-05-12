#include "lexico.h"

enum {
    Q0,
    Q1,
    Q2,
    Q3,
    Q4,
    Q5,
    Q6,
    Q7,
    Q8,
    Q9,
    Q10,
    Q11,
    Q12,
    Q13,
    Q14,
    Q15,
    Q16,
    Q17,
    Q18,
    Q19,
    Q20,
    Q21,
    Q22,
    Q23,
    Q24,
    K
};

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

int transiciones[25][18] = {
    { Q1,  Q3,  Q9, Q10,  Q2,  Q6, Q15, Q16, Q17,  Q7,  Q8, Q22, Q11,   K, Q20, Q21, Q23, Q24},
    { Q1,  Q1,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,  Q3,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,  Q4,   K,   K,   K,   K},
    {  K,  Q5,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,  Q5,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,  Q19,  K,   K,   K},
    {  K,   K,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,  K,    K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q13, Q12, Q12, Q12,   K,   K},
    {Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q12, Q13, Q12, Q12, Q12,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K, Q18,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K, Q18,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K, Q19,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,  Q19,  K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K}
};


Lexico::Lexico(std::string filename)
{
    std::fstream archivo(filename.c_str(), std::ios::in);
    std::string buffer_ = "";
    Token temp_;
    char caracter_;   
    int estado_ = Q0;
    int nuevoEdo_ = Q0;
    int tipo_;
    int nivelTab_ = 0;

    if(!archivo.is_open())
    {
        std::cerr << "ERROR: No se pudo abrir el archivo" << std::endl;
        setResult(0);
        exit(0);
    }
    std::getline(archivo, buffer_, '\0');
    archivo.close();
    
    for(unsigned int i = 0; i <= buffer_.length(); i++)
    {
        caracter_ = buffer_[i];
        tipo_ = returnType(caracter_);

        if(tipo_ == -1)
        {
            std::cerr << "ERROR LEXICO: Token invalido en: " << i << " -- Entrada: " << caracter_ << std::endl;
            setResult(0);
            exit(0);
        }

        nuevoEdo_ = transiciones[nuevoEdo_][tipo_];
        if(nuevoEdo_ == K)
        {
            if(returnTokenType(estado_) != -1)
            {
                if(returnTokenType(estado_) != TAB)
                {
                    temp_.setType(returnTokenType(estado_));
                    tokensList_.push_back(temp_);
                }

                if(caracter_ != ' ')
                    i--;

                if(returnTokenType(estado_) == NUEVA_LINEA || i == buffer_.length() - 1)
                {
                    i++;
                    int nivelActualTab = 0;
                    temp_.setContent("\t");
                    temp_.setType(TAB);
                    while (true)
                    {
                        // Eliminar saltos entre identaciones
                        if(buffer_[i] == '\n')
                            i++;
                        else
                            break;
                    }

                    while (buffer_[i] == '\t')
                    {
                        nivelActualTab++;
                        if(nivelActualTab > nivelTab_)
                        {
                            nivelTab_ = nivelActualTab;
                            temp_.setType(TAB);
                            temp_.setContent("\t");
                            while (buffer_[i] == '\t')
                            {
                                tokensList_.push_back(temp_);
                                nivelTab_++;
                                i++;
                                if(buffer_[i] != '\t')
                                {
                                    i--;
                                    nivelTab_--;
                                    break;
                                }
                            }
                        }
                        i++;
                        if(buffer_[i] != '\t')
                        {
                            i--;
                            break;
                        }
                    }
                    temp_.setContent("");
                    temp_.setType(UNTAB);
                    if(nivelTab_ > nivelActualTab)
                    {
                        while (nivelTab_ > nivelActualTab)
                        {
                            tokensList_.push_back(temp_);
                            nivelTab_--;
                        }
                        temp_.setType(NUEVA_LINEA);
                        temp_.setContent("\n");
                        tokensList_.push_back(temp_);
                    }
                    i--;
                }
            }

            estado_ = Q0;
            nuevoEdo_ = Q0;
            temp_.setContent("");
        }
        else
        {
            estado_ = nuevoEdo_;
            temp_.addContent(caracter_);
        }
    }    
    setResult(1);
}

int Lexico::returnType(const char caracter) const
{
    int type = -1;

    if((caracter >= 'a' && caracter <= 'z') || (caracter >= 'A' && caracter <= 'Z') || caracter == '_')
        type = 0;
    else if(caracter >= '0' && caracter <= '9')
        type = 1;
    else if(caracter == '+' || caracter == '-')
        type = 2;
    else if(caracter == '*' || caracter == '/' || caracter == '%')
        type = 3;
    else if(caracter == ')' || caracter == '(')
        type = 4;
    else if(caracter == ':')
        type = 5;
    else if(caracter == '&')
        type = 6;
    else if(caracter == '|')
        type = 7;
    else if(caracter == '!')
        type = 8;
    else if(caracter == '<' || caracter == '>')
        type = 9;
    else if(caracter == '\n')
        type = 10;
    else if(caracter == '\t')
        type = 11;
    else if(caracter == '"')
        type = 12;
    else if(caracter == '.')
        type = 13;
    else if(caracter == '=')
        type = 14;
    else if(caracter == '\0')
        type = 15;
    else if(caracter == ',')
        type = 16;
    else if(caracter == ' ')
        type = 17;

    return type;
}

int Lexico::returnTokenType(const int state) const
{
    int tokenType = -1;

    if(state == Q1)
        tokenType = IDENTIF;
    else if(state == Q2)
        tokenType = PARENTESIS;
    else if(state == Q3)
        tokenType = ENTERO;
    else if(state == Q5)
        tokenType = REAL;
    else if(state == Q6)
        tokenType = DOS_PUNTOS;
    else if(state == Q7 || state == Q19)
        tokenType = OP_REL;
    else if(state == Q8)
        tokenType = NUEVA_LINEA;
    else if(state == Q9)
        tokenType = ADICION;
    else if(state == Q10)
        tokenType = MULTIPLICACION;
    else if(state == Q13)
        tokenType = CADENA;
    else if(state == Q14)
        tokenType = COMA;
    else if(state == Q20)
        tokenType = ASIGNACION;
    else if(state == Q22)
        tokenType = TAB;

    return tokenType;
}

void Lexico::showTokens()
{
    if(!tokensList_.empty())
        for(unsigned int i = 0; i < tokensList_.size(); i++)
        {
            std::cout << i << ": ";
            std::cout << strType(tokensList_[i].getType()) << ": " << tokensList_[i].getContent() << std::endl;
        }
    else
        std::cerr << "No hay tokens que mostrar" << std::endl;
}

std::vector<Token> Lexico::getTokensList()
{
    return tokensList_;
}

void Lexico::setResult(const int result)
{
    std::fstream salida("salida.txt", std::ios::out);

    if(salida.is_open())
        salida << result;
    
    salida.close();
}

const std::string Lexico::strType(const int type)
{
    std::string strRet = "";

    if(type == IDENTIF)
        strRet = "IDENTIF";
    else if(type == DIGITO)
        strRet = "DIGITO";
    else if(type == ENTERO)
        strRet = "ENTERO";
    else if(type == REAL)
        strRet = "REAL";
    else if(type == CADENA)
        strRet = "CADENA";
    else if(type == ADICION)
        strRet = "ADICION";
    else if(type == MULTIPLICACION)
        strRet = "MULTIPLICACION";
    else if(type == PARENTESIS)
        strRet = "PARENTESIS";
    else if(type == DOS_PUNTOS)
        strRet = "DOS_PUNTOS";
    else if(type == COMA)
        strRet = "COMA";
    else if(type == OP_LOG)
        strRet = "OP_LOG";
    else if(type == OP_REL)
        strRet = "OP_REL";
    else if(type == ASIGNACION)
        strRet = "ASIGNACION";
    else if(type == NUEVA_LINEA)
        strRet = "NUEVA_LINEA";
    else if(type == TAB)
        strRet = "TAB";
    else if(type == UNTAB)
        strRet = "UNTAB";

    return strRet;
}
