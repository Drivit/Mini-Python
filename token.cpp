#include "token.h"

Token::Token()
{
    type_ = -1;
    content_ = "";
}

std::string Token::getContent() const
{
    return content_;
}

void Token::addContent(const char content)
{
    content_ += content;
}

void Token::setContent(const std::string content)
{
    content_ = content;
}

int Token::getType() const
{
    return type_;
}

void Token::setType(const int type)
{
    type_ = type;
}
