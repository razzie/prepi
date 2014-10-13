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
 , m_delimiter(delimiter)
{
    m_origLoc = m_stream->getloc();
    m_stream->imbue(std::locale(m_origLoc, new delimiter_is_space(delimiter)));
    *m_stream >> std::boolalpha;
}

Parser::Parser(std::string str, char delimiter)
 : m_stream(&m_string)
 , m_delimiter(delimiter)
{
    m_string.str(str);

    m_origLoc = m_stream->getloc();
    m_stream->imbue(std::locale(m_origLoc, new delimiter_is_space(delimiter)));
    *m_stream >> std::boolalpha;
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

bool Parser::hasNextArg() const
{
    std::streampos curr = m_stream->tellg();
    m_stream->seekg(-1, m_stream->end);

    std::streampos last = m_stream->tellg();
    m_stream->seekg(curr);

    return (*m_stream && (curr != last) && (m_stream->peek() != '\n'));
}

std::string Parser::getString()
{
    std::string s;

    if (m_stream->peek() == m_delimiter) m_stream->ignore();

    if (m_stream->peek() == '"')
    {
        m_stream->ignore(); // ignore string begin mark

        while (*m_stream)
        {
            if (m_stream->peek() == '"')
            {
                m_stream->ignore(); // ignore string end mark
                return s;
            }
            else if (m_stream->peek() == '\\')
            {
                m_stream->ignore();
                if (m_stream->peek() == 'n')
                {
                    m_stream->ignore();
                    s += '\n';
                }
                else
                {
                    s += '\\';
                }
            }
            else
            {
                s += m_stream->get();
            }
        }
    }

    throw std::runtime_error("string parse error");
}
