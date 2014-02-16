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
        virtual ~reference_counted();

    public:
        reference_counted();
        void grab() const;
        void drop() const;
        uint32_t get_ref_count() const;
    };

    template<class T>
    using reference_counted_type =
        typename std::enable_if<std::is_base_of<reference_counted, T>::value>::type;

    template<class T, class = reference_counted_type<T>>
    T* grab(T* o)
    {
        o->grab();
        return o;
    }

    template<class T, class = reference_counted_type<T>>
    const T* grab(const T* o)
    {
        o->grab();
        return o;
    }

    template<class T, class = reference_counted_type<T>>
    T* drop(T* o)
    {
        uint32_t refc = o->get_ref_count();
        o->drop();
        return (refc == 1) ? nullptr : o;
    }

    template<class T, class = reference_counted_type<T>>
    const T* drop(const T* o)
    {
        uint32_t refc = o->get_ref_count();
        o->drop();
        return (refc == 1) ? nullptr : o;
    }

    template<class T, bool auto_grab = false, class = reference_counted_type<T>>
    class grab_ptr
    {
        T* m_obj;

    public:
        grab_ptr() : m_obj(nullptr) {}
        grab_ptr(T* t) : m_obj(t) { if (auto_grab) m_obj->grab(); }
        grab_ptr(const grab_ptr& p) : m_obj(p.m_obj) { if (m_obj != nullptr) m_obj->grab(); }
        grab_ptr(grab_ptr&& p) : m_obj(p.m_obj) { p.m_obj = nullptr; }

        grab_ptr& operator= (T* t)
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
            T* tmp = m_obj;
            m_obj = p.m_obj;
            p.m_obj = tmp;
            return *this;
        }

        operator T*& () { return m_obj; }
        operator const T*& () const { return m_obj; }

        T& operator* () { return m_obj; }
        const T& operator* () const { return m_obj; }

        T* operator-> () { return m_obj; }
        const T* operator-> () const { return m_obj; }
    };

    template<class T, class = reference_counted_type<T>>
    grab_ptr<T, false> auto_drop(T* t)
    {
        return t;
    }

    typedef grab_ptr<const reference_counted, true> grab_guard;
};

#endif // GG_REFCOUNTED_HPP_INCLUDED
