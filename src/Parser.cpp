#include <locale>
#include "Parser.h"

struct colon_is_space : std::ctype<char>
{
    colon_is_space() : std::ctype<char>(get_table()) {}
    static mask const* get_table()
    {
        static mask rc[table_size];
        rc[','] = std::ctype_base::space;
        rc['\n'] = std::ctype_base::space;
        return &rc[0];
    }
};


Parser::Parser(std::istream& stream)
 : m_stream(&stream)
{
    m_line.imbue(std::locale(m_line.getloc(), new colon_is_space));
    nextLine();
}

Parser::Parser(std::string str)
 : m_stream(nullptr)
{
    m_line.imbue(std::locale(m_line.getloc(), new colon_is_space));
    m_line.str(str);
}

Parser::~Parser()
{
}

bool Parser::nextLine()
{
    if (m_stream == nullptr) return false;

    m_line.str(std::string());
    std::streambuf* buf = m_line.rdbuf();
    return m_stream->get(*buf);
}
