#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <cstdlib>
#include <cstring>

class Token
{
    private:
        int type_;
        std::string content_;

    public:
        Token();

        void addContent(const char content);
        void setContent(const std::string content);
        std::string getContent() const;
        void setType(const int type);
        int getType() const;
};

#endif
