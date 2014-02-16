#ifndef GG_TYPEINFO_HPP_INCLUDED
#define GG_TYPEINFO_HPP_INCLUDED

#include <typeinfo>
#include <string>

namespace gg
{
    class typeinfo
    {
        const std::type_info* m_type;

    public:
        typeinfo(const std::type_info&);
        typeinfo(const typeinfo&);
        typeinfo& operator= (const typeinfo&);
        ~typeinfo();

        bool operator== (const typeinfo&) const;
        bool operator!= (const typeinfo&) const;
        bool operator<  (const typeinfo&) const;
        bool operator<= (const typeinfo&) const;
        bool operator>  (const typeinfo&) const;
        bool operator>= (const typeinfo&) const;

        std::string get_name() const;
        const std::type_info& get_type() const;
        size_t get_hash() const noexcept;
        operator const std::type_info& () const;

        template<class T>
        static std::string name() { return name(typeid(T)); }
        static std::string name(const std::type_info&);
    };
};

#endif // GG_TYPEINFO_HPP_INCLUDED
