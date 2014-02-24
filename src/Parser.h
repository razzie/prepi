#include <iostream>
#include <sstream>
#include <tuple>
#include <stdexcept>

class Parser
{
    std::istream* m_stream;
    std::stringstream m_line;

public:
    Parser(std::istream&);
    Parser(std::string);
    Parser(const Parser&) = delete;
    ~Parser();

    bool nextLine();

    template<class T>
    T getArg()
    {
        T t;
        if ( !(m_line >> t) ) throw std::runtime_error("out of args");
        return t;
    }

    template<class... Args>
    std::tuple<Args...> getArgs()
    {
        //return std::make_tuple( getArg<Args>()... );
        return std::tuple<Args...> { getArg<Args>()... };
    }
};
