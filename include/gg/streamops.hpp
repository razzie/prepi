#ifndef GG_STREAMOPS_HPP_INCLUDED
#define GG_STREAMOPS_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <type_traits>
#include <stdexcept>

namespace gg
{
    template<class T>
    class ios_manipulator
    {
    public:
        typedef std::ios&(*manipulator)(std::ios&, T);

        ios_manipulator(manipulator m, T v)
         : m_manip(m), m_value(v) {}
        ~ios_manipulator() {}

        friend std::ios& operator<< (std::ios& io, const ios_manipulator<T>& m)
        {
            return m.m_manip(io, m.m_value);
        }

    private:
        manipulator m_manip;
        T m_value;
    };

    template<class T>
    class istream_manipulator
    {
    public:
        typedef std::istream&(*manipulator)(std::istream&, T);

        istream_manipulator(manipulator m, T v)
         : m_manip(m), m_value(v) {}
        ~istream_manipulator() {}

        friend std::istream& operator<< (std::istream& io, const istream_manipulator<T>& m)
        {
            return m.m_manip(io, m.m_value);
        }

    private:
        manipulator m_manip;
        T m_value;
    };

    template<class T>
    class ostream_manipulator
    {
    public:
        typedef std::ostream&(*manipulator)(std::ostream&, T);

        ostream_manipulator(manipulator m, T v)
         : m_manip(m), m_value(v) {}
        ~ostream_manipulator() {}

        friend std::ostream& operator<< (std::ostream& io, const ostream_manipulator<T>& m)
        {
            return m.m_manip(io, m.m_value);
        }

    private:
        manipulator m_manip;
        T m_value;
    };


    namespace meta
    {
        namespace sfinae
        {
            class yes { char c[1]; };
            class no  { char c[2]; };
        };

        template<class T> T& lvalue_of_type();
        template<class T> T  rvalue_of_type();

        template<class T>
        struct has_insert_op
        {
            template<class U>
            static sfinae::yes test(char(*)[sizeof(
                lvalue_of_type<std::ostream>() << rvalue_of_type<U>()
            )]);

            template<class U>
            static sfinae::no test(...);

            enum { value = ( sizeof(sfinae::yes) == sizeof(test<T>(0)) ) };
            typedef std::integral_constant<bool, value> type;
        };

        template<class T>
        struct has_extract_op
        {
            template<class U>
            static sfinae::yes test(char(*)[sizeof(
                lvalue_of_type<std::istream>() >> lvalue_of_type<U>()
            )]);

            template<class U>
            static sfinae::no test(...);

            enum { value = ( sizeof(sfinae::yes) == sizeof(test<T>(0)) ) };
            typedef std::integral_constant<bool, value> type;
        };
    };


    template<class T>
    void ostream_insert(std::ostream& o, const T& t,
        typename std::enable_if<meta::has_insert_op<T>::value>::type* = 0)
    {
        o << t;
    }

    template<class T>
    void ostream_insert(std::ostream& o, const T& t,
        typename std::enable_if<!meta::has_insert_op<T>::value>::type* = 0)
    {
        o << "???";
    }

    template<class T>
    bool istream_extract(std::istream& o, T& t,
        typename std::enable_if<meta::has_extract_op<T>::value>::type* = 0)
    {
        return (o >> t);
    }

    template<class T>
    bool istream_extract(std::istream& o, T& t,
        typename std::enable_if<!meta::has_extract_op<T>::value>::type* = 0)
    {
        return false;
    }
};

#endif // GG_STREAMOPS_HPP_INCLUDED
