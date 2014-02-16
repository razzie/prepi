#ifndef GG_UTIL_HPP_INCLUDED
#define GG_UTIL_HPP_INCLUDED

#include <assert.h>
#include <cstdint>
#include <locale>
#include <string>
#include <vector>
#include <functional>
#include <type_traits>
#include <stdexcept>

namespace gg
{
namespace util
{
    class scope_callback
    {
        std::function<void()> m_func;

    public:
        scope_callback();
        scope_callback(std::function<void()> func);
        scope_callback(const scope_callback&) = delete;
        scope_callback(scope_callback&&) = delete;
        ~scope_callback();
        scope_callback& operator= (std::function<void()> func);
        void reset();
    };


    bool is_big_endian();
    bool is_little_endian();

    template<class T, size_t N = sizeof(T)>
    T& swap_byte_order(T& t)
    {
        uint8_t buf[N];
        size_t i;
        for (i = 0; i < N; ++i) buf[i] = reinterpret_cast<uint8_t*>(&t)[i];
        for (i = N; i > 0; --i) reinterpret_cast<uint8_t*>(&t)[i] = buf[i];
        return t;
    }

    template<class T, size_t N = sizeof(T)>
    T& to_host_byte_order(T& t)
    {
        if (!is_big_endian())
            return swap_byte_order<T, N>(t);
        else
            return t;
    }

    template<class T, size_t N = sizeof(T)>
    T& to_network_byte_order(T& t)
    {
        if (!is_big_endian())
            return swap_byte_order<T, N>(t);
        else
            return t;
    }


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
