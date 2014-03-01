#ifndef GG_REFCOUNTED_HPP_INCLUDED
#define GG_REFCOUNTED_HPP_INCLUDED

#include <cstdint>
#include <type_traits>
#include "gg/atomic.hpp"

namespace gg
{
    class reference_counted
    {
        mutable atomic<uint32_t> m_ref_count;

    protected:
        virtual ~reference_counted() {}

    public:
        reference_counted();
        void grab() const;
        void drop() const;
        uint32_t get_ref_count() const;
    };

    template<class P>
    using reference_counted_type =
        typename std::enable_if<std::is_convertible<P, const reference_counted*>::value>::type;

    template<class P, class = reference_counted_type<P>>
    P grab(P o)
    {
        o->grab();
        return o;
    }

    template<class P, class = reference_counted_type<P>>
    P drop(P o)
    {
        uint32_t refc = o->get_ref_count();
        o->drop();
        return (refc == 1) ? nullptr : o;
    }

    template<class P, bool auto_grab = false, class = reference_counted_type<P>>
    class grab_ptr
    {
        P m_obj;

    public:
        typedef typename std::remove_pointer<P>::type type;

        grab_ptr() : m_obj(nullptr) {}
        grab_ptr(P t) : m_obj(t) { if (auto_grab) m_obj->grab(); }
        grab_ptr(const grab_ptr& p) : m_obj(p.m_obj) { if (m_obj != nullptr) m_obj->grab(); }
        grab_ptr(grab_ptr&& p) : m_obj(p.m_obj) { p.m_obj = nullptr; }

        grab_ptr& operator= (P t)
        {
            if (m_obj != nullptr) m_obj->drop();
            m_obj = t;
            if (auto_grab) m_obj->grab();
            return *this;
        }
        grab_ptr& operator= (const grab_ptr& p)
        {
            if (m_obj != nullptr) m_obj->drop();
            m_obj = p.m_obj;
            if (m_obj != nullptr) m_obj->grab();
            return *this;
        }
        grab_ptr& operator= (grab_ptr&& p)
        {
            //std::swap(m_obj, p.m_pbj);
            P tmp = m_obj;
            m_obj = p.m_obj;
            p.m_obj = tmp;
            return *this;
        }

        operator P& () { return m_obj; }
        operator const P& () const { return m_obj; }

        type& operator* () { return *m_obj; }
        const type& operator* () const { return *m_obj; }

        P operator-> () { return m_obj; }
        const P operator-> () const { return m_obj; }
    };

    template<class P, class = reference_counted_type<P>>
    grab_ptr<P, false> auto_drop(P t)
    {
        return t;
    }

    typedef grab_ptr<const reference_counted*, true> grab_guard;
};

#endif // GG_REFCOUNTED_HPP_INCLUDED
