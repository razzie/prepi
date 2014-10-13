#include <iostream>
#include <sstream>
#include <locale>
#include <tuple>
#include <stdexcept>

class Parser
{
    std::istream* m_stream;
    std::stringstream m_string;
    std::locale m_origLoc;
    char m_delimiter;

public:
    Parser(std::istream&, char delimiter);
    Parser(std::string, char delimiter);
    Parser(const Parser&) = delete;
    ~Parser();

    // jump to next line
    bool nextLine();

    // has next arg in this line
    bool hasNextArg() const;

    // special getter for " enclosed strings
    std::string getString();

    // get one argument
    template<class T>
    typename std::enable_if<!std::is_same<T, std::string>::value, T>::type
        getArg()
    {
        T t;
        if ( !(*m_stream >> t) ) throw std::runtime_error("out of args");
        return t;
    }

    template<class T>
    typename std::enable_if<std::is_same<T, std::string>::value, T>::type
        getArg()
    {
        return getString();
    }

    // get multiple arguments (recursive template)
    template<class Arg>
    std::tuple<Arg> getArgs()
    {
        return std::make_tuple(getArg<Arg>());
    }

    template<class Arg1, class Arg2, class... Args>
    std::tuple<Arg1, Arg2, Args...> getArgs()
    {
        auto a = getArgs<Arg1>();
        auto b = getArgs<Arg2, Args...>();
        return std::tuple_cat(a,b);
    }
};
