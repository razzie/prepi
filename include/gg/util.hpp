#ifndef GG_UTIL_HPP_INCLUDED
#define GG_UTIL_HPP_INCLUDED

#include <cassert>
#include <cstdint>
#include <iosfwd>
#include <locale>
#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include <type_traits>
#include <stdexcept>
#include "gg/optional.hpp"
#include "gg/streamops.hpp"

namespace gg
{
namespace util
{
    class scope_callback
    {
        std::function<void()> m_func;

    public:
        scope_callback();
        scope_callback(std::function<void()>);
        scope_callback(const scope_callback&) = delete;
        scope_callback(scope_callback&&) = delete;
        ~scope_callback();
        scope_callback& operator= (std::function<void()>);
        void reset();
    };


    ostream_manipulator<const char*> format(const char*);
    istream_manipulator<char> delimiter(char);
    istream_manipulator<char> next(char);
    std::istream& next_line(std::istream&);


    template<class Arg>
    std::tuple<Arg> parse(std::istream& i, optional<char> d = {})
    {
        Arg a;
        if (d) i << delimiter(*d);
        if (!istream_extract(i, a)) throw std::runtime_error("can't extract arg");
        return std::tuple<Arg> { a };
    }

    template<class Arg1, class Arg2, class... Args>
    std::tuple<Arg1, Arg2, Args...> parse(std::istream& i, optional<char> d = {})
    {
        if (d) i << delimiter(*d);
        auto a = parse<Arg1>(i);
        auto b = parse<Arg2, Args...>(i);
        return std::tuple_cat(a,b);
    }

    template<class... Args>
    std::tuple<Args...> parse(std::string str, optional<char> d = {})
    {
        std::stringstream ss(str);
        if (d) ss << delimiter(*d);
        return parse<Args...>(ss);
    }


    template<size_t I = 0, class... Args>
    typename std::enable_if<I == sizeof...(Args), void>::type
    tuple_to_varlist(const std::tuple<Args...>& tup, varlist& vl) {}

    template<size_t I = 0, class... Args>
    typename std::enable_if<I < sizeof...(Args), void>::type
    tuple_to_varlist(const std::tuple<Args...>& tup, varlist& vl)
    {
        vl.push_back(std::get<I>(tup));
        tuple_to_varlist<I+1, Args...>(tup, vl);
    }

    template<class... Args>
    varlist tuple_to_varlist(const std::tuple<Args...>& tup)
    {
        varlist vl;
        tuple_to_varlist(tup, vl);
        return std::move(vl);
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


    template<class T>
    int strcmpi(std::basic_string<T> s1, std::basic_string<T> s2,
                std::locale loc = std::locale())
    {
        if (s1.size() != s2.size()) return (s1.size() - s2.size());

        for (size_t i = 0; i < s1.size(); ++i)
        {
            int c1 = std::tolower(s1[i], loc);
            int c2 = std::tolower(s2[i], loc);
            if (c1 != c2) return (c1 - c2);
        }

        return 0;
    }

    extern template int strcmpi(std::basic_string<char>, std::basic_string<char>, std::locale);
    extern template int strcmpi(std::basic_string<wchar_t>, std::basic_string<wchar_t>, std::locale);

    template<class T>
    int strncmpi(std::basic_string<T> s1, std::basic_string<T> s2, size_t n,
                 std::locale loc = std::locale())
    {
        if (s1.size() < n || s2.size() < n) return (s1.size() - s2.size());

        for (size_t i = 0; i < n; ++i)
        {
            int c1 = std::tolower(s1[i], loc);
            int c2 = std::tolower(s2[i], loc);
            if (c1 != c2) return (c1 - c2);
        }

        return 0;
    }

    extern template int strncmpi(std::basic_string<char>, std::basic_string<char>, size_t, std::locale);
    extern template int strncmpi(std::basic_string<wchar_t>, std::basic_string<wchar_t>, size_t, std::locale);


    template<class T>
    std::basic_string<T> trim(std::basic_string<T> s,
                              std::locale loc = std::locale())
    {
        auto s_begin = s.begin(), s_end = s.end();
        auto it_first = s_end, it_last = s_end;

        for (auto it = s_begin; it != s_end; ++it)
        {
            if (!std::isspace(*it, loc))
            {
                if (it_first == s_end) it_first = it;
                it_last = it + 1;
            }
        }

        return std::basic_string<T>(it_first, it_last);
    }

    extern template std::basic_string<char> trim<char>(std::basic_string<char>, std::locale);
    extern template std::basic_string<wchar_t> trim<wchar_t>(std::basic_string<wchar_t>, std::locale);


    template<class FROM, class TO>
    std::basic_string<TO> convert_string(std::basic_string<FROM> s,
                                         std::locale loc = std::locale())
    {
        std::vector<TO> result(s.size() + 1);
        FROM const* fromNext;
        TO* toNext;
        mbstate_t state = {0};
        std::codecvt_base::result convResult
            = std::use_facet<std::codecvt<TO, FROM, std::mbstate_t> >(loc)
            .in(state,&s[0], &s[s.size()], fromNext,
                &result[0], &result[result.size()], toNext);

        assert(fromNext == &s[s.size()]);
        assert(toNext != &result[result.size()]);
        assert(convResult == std::codecvt_base::ok);
        *toNext = '\0';

        return &result[0];
    }

    extern template std::basic_string<char> convert_string<char>(std::basic_string<char>, std::locale);
    extern template std::basic_string<wchar_t> convert_string<wchar_t>(std::basic_string<wchar_t>, std::locale);


    template<class T>
    bool is_integer(std::basic_string<T> s,
                    std::locale loc = std::locale())
    {
        if (s.empty()) return false;

        auto it = s.begin(), end = s.end();

        if (*it == '-') ++it;

        for (; it != end; ++it)
        {
            if (!std::isdigit(*it, loc)) return false;
        }

        return true;
    }

    extern template bool is_integer<char>(std::basic_string<char>, std::locale);
    extern template bool is_integer<wchar_t>(std::basic_string<wchar_t>, std::locale);


    template<class T>
    bool is_float(std::basic_string<T> s,
                  std::locale loc = std::locale())
    {
        if (s.empty()) return false;

        auto it = s.begin(), end = s.end();
        bool point_used = false;

        if (*it == '-') ++it;

        for (; it != end; ++it)
        {
            if ((!std::isdigit(*it, loc) && *it != '.') ||
                (*it == '.' && point_used)) return false;

            if (*it == '.') point_used = true;
        }

        return true;
    }

    extern template bool is_float<char>(std::basic_string<char>, std::locale);
    extern template bool is_float<wchar_t>(std::basic_string<wchar_t>, std::locale);


    template<class T>
    bool is_numeric(std::basic_string<T> s,
                    std::locale loc = std::locale())
    {
        return (is_float(s, loc) || is_integer(s, loc));
    }

    extern template bool is_numeric<char>(std::basic_string<char>, std::locale);
    extern template bool is_numeric<wchar_t>(std::basic_string<wchar_t>, std::locale);


    template<class T>
    bool contains_space(std::basic_string<T> s,
                        std::locale loc = std::locale())
    {
        auto it = s.cbegin(), end = s.cend();
        bool found_char = false;
        bool found_space = false;

        for (; it != end; ++it)
        {
            if (!found_char && !std::isspace(*it, loc)) { found_char = true; continue; }
            if (found_char && !found_space && std::isspace(*it, loc)) { found_space = true; continue; }
            if (found_space && !std::isspace(*it, loc)) return true;
        }

        return false;
    }

    extern template bool contains_space<char>(std::basic_string<char>, std::locale);
    extern template bool contains_space<wchar_t>(std::basic_string<wchar_t>, std::locale);
};
};

#endif // GG_UTIL_HPP_INCLUDED
