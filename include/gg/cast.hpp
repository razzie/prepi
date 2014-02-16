#ifndef GG_CAST_HPP_INCLUDED
#define GG_CAST_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <type_traits>
#include <stdexcept>

namespace gg
{
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
    void istream_extract(std::istream& o, T& t,
        typename std::enable_if<meta::has_extract_op<T>::value>::type* = 0)
    {
        o >> t;
    }

    template<class T>
    void istream_extract(std::istream& o, T& t,
        typename std::enable_if<!meta::has_extract_op<T>::value>::type* = 0)
    {
    }

    template<class From, class To>
    typename std::enable_if<std::is_convertible<From, To>::value, To>::type
    cast(const From& from)
    {
        return To(from);
    }

    template<class From, class To>
    typename std::enable_if<!std::is_convertible<From, To>::value, To>::type
    cast(const From& from)
    {
        if (!meta::has_insert_op<From>::value
            || !meta::has_extract_op<To>::value)
        {
            throw std::runtime_error("unable to cast");
        }

        To result;
        std::stringstream ss;

        ostream_insert(ss, from);
        istream_extract(ss, result);

        return result;
    }
};

#endif // GG_CAST_HPP_INCLUDED
