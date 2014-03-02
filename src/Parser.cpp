#include <limits>
#include <cstring>
#include "Parser.h"

struct delimiter_is_space : std::ctype<char>
{
    std::ctype_base::mask m_rc[table_size];

    delimiter_is_space(char delim)
     : std::ctype<char>(get_table(static_cast<unsigned char>(delim)))
    {
    }

    const std::ctype_base::mask* get_table(unsigned char delim)
    {
        memset(m_rc, 0, sizeof(std::ctype_base::mask) * table_size);
        m_rc[delim] = std::ctype_base::space;
        m_rc['\n'] = std::ctype_base::space;
        return &m_rc[0];
    }
};


Parser::Parser(std::istream& stream, char delimiter)
 : m_stream(&stream)
{
    m_origLoc = m_stream->getloc();
    m_stream->imbue(std::locale(m_origLoc, new delimiter_is_space(delimiter)));
}

Parser::Parser(std::string str, char delimiter)
 : m_stream(&m_string)
{
    m_string.str(str);

    m_origLoc = m_stream->getloc();
    m_stream->imbue(std::locale(m_origLoc, new delimiter_is_space(delimiter)));
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
