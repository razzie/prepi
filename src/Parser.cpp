#include <limits>
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
    m_origLoc = m_stream->getloc();
    m_stream->imbue(std::locale(m_origLoc, new colon_is_space));
}

Parser::Parser(std::string str)
 : m_stream(&m_string)
{
    m_string.str(str);

    m_origLoc = m_stream->getloc();
    m_stream->imbue(std::locale(m_origLoc, new colon_is_space));
}

Parser::~Parser()
{
    m_stream->imbue(m_origLoc); // reverting to original locale
}

bool Parser::nextLine()
{
    m_stream->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return *m_stream;
}
