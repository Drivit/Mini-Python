#ifndef LEXICO_H
#define LEXICO_H

#include <fstream>
#include <vector>
#include "token.h"

class Lexico
{
    public:
        Lexico(std::string filename);
        std::vector<Token> getTokensList();
        void showTokens();
        
    private:
        std::vector<Token> tokensList_;

        int returnType(const char caracter) const;
        int returnTokenType(const int state) const;
        const std::string strType(const int type);
        void setResult(const int result);
};

#endif