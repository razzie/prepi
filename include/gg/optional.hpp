#ifndef OPTIONAL_HPP_INCLUDED
#define OPTIONAL_HPP_INCLUDED

#include <utility>
#include <stdexcept>
#include "gg/var.hpp"

namespace gg
{
    template<class T>
    class optional
    {
        var m_val;
        bool m_valid;

    public:
        optional()
         : m_valid(false) {}

        template<class... Args>
        optional(Args... args)
         : m_valid(true) { m_val.construct<T>(std::forward<Args>(args)...); }

        optional(const optional& o)
         : m_val(o.m_val), m_valid(o.m_valid) {}

        optional(optional&& o)
         : m_val(std::move(o.m_val)), m_valid(o.m_valid) {}

        ~optional() {}

        optional& operator= (T t)
        {
            m_val = t;
            m_valid = true;
            return *this;
        }

        optional& operator= (const optional& o)
        {
            m_val = o.m_val;
            m_valid = o.m_valid;
            return *this;
        }

        optional& operator= (optional&& o)
        {
            m_val = std::move(o.m_val);
            m_valid = o.m_valid;
            return *this;
        }

        optional& reference(T& t)
        {
            m_val.reference(t);
            m_valid = true;
            return *this;
        }

        T& get()
        {
            if (!m_valid) throw std::runtime_error("getting value of invalid optional");
            return m_val.get<T>();
        }

        const T& get() const
        {
            if (!m_valid) throw std::runtime_error("getting value of invalid optional");
            return m_val.get<T>();
        }

        operator T&() { return get(); }
        operator const T&() const { return get(); }

        T& operator* () { return get(); }
        const T& operator* () const { return get(); }

        T* operator-> () { return &get(); }
        const T* operator-> () const { return &get(); }

        bool is_valid() const { return m_valid; }
        void invalidate() { m_val.clear(); m_valid = false; }

        friend std::ostream& operator<< (std::ostream& o, const optional& opt)
        {
            if (opt.is_valid()) return (o << opt->to_stream());
            else return (o << "(invalid)");
        }
    };

    extern template class optional<var>;
};

#endif // OPTIONAL_HPP_INCLUDED
