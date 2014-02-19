#ifndef GG_ENUMERATOR_HPP_INCLUDED
#define GG_ENUMERATOR_HPP_INCLUDED

#include <functional>
#include <iterator>
#include <type_traits>
#include <stdexcept>
#include "gg/optional.hpp"

namespace gg
{
    template<class container, class T, bool is_reference = false,
             bool is_const = std::is_const<container>::value>
    class container_wrapper
    {
        template<class U>
        using make_ref = typename std::conditional<std::is_pointer<U>::value, U, U&>::type;

    public:
        typedef T value_type;
        typedef make_ref<value_type> value_type_ref;
        typedef typename container::value_type container_value_type;
        typedef make_ref<container_value_type> container_value_type_ref;
        typedef std::function<value_type_ref(container_value_type_ref)> extractor;

        typedef decltype(((container*)0)->begin()) container_iterator;
        typedef typename std::remove_reference<container>::type container_noref;
        typedef typename std::decay<container>::type container_decay;

        class iterator : public std::iterator<std::forward_iterator_tag, value_type>
        {
        private:
            container_iterator m_iter;
            extractor* m_extr;

            friend class container_wrapper;

            iterator(container_iterator it, extractor& extr)
             : m_iter(it), m_extr(&extr) {}

        public:
            iterator(const iterator& it) : m_iter(it.m_iter), m_extr(it.m_extr) {}
            ~iterator() {}

            iterator& operator= (const iterator& it) { m_iter = it.m_iter; m_extr = it.m_extr; return *this; }
            iterator& operator++ () { ++m_iter; return *this; }
            iterator& operator++ (int) { m_iter++; return *this; }

            bool operator== (const iterator& it) const { return (m_iter == it.m_iter); }
            bool operator!= (const iterator& it) const { return (m_iter != it.m_iter); }

            value_type_ref operator* () { return (*m_extr)(*m_iter); }
            const value_type_ref operator* () const { return (*m_extr)(*m_iter); }
            value_type* operator-> () { return &(*m_extr)(*m_iter); }
            const value_type* operator-> () const { return &(*m_extr)(*m_iter); }

            container_iterator get_internal_iterator() { return m_iter; }
        };

    private:
        container_noref* m_cont;
        extractor m_extr;

    public:
        container_wrapper(container_decay&& cont, extractor extr = {})
        {
            //static_assert(is_reference, "can't take reference of xvalue");
            //static_assert(is_const, "can't take xvalue of const");
            if (is_reference) throw std::runtime_error("can't take reference of xvalue");
            m_cont = new container_decay(cont);
            if (!m_extr) m_extr = [](container_value_type_ref it)->value_type_ref { return static_cast<value_type_ref>(it); };
        }

        container_wrapper(container_noref& cont, extractor extr = {})
        {
            if (is_reference) m_cont = &cont;
            else m_cont = new container_decay(cont.begin(), cont.end());
            if (!m_extr) m_extr = [](container_value_type_ref it)->value_type_ref { return static_cast<value_type_ref>(it); };
        }

        container_wrapper(const container_wrapper& cont)
        {
            if (is_reference) m_cont = cont.m_cont;
            else m_cont = new container_decay(cont.m_cont->begin(), cont.m_cont->end());
            m_extr = cont.m_extr; // can't move as the original would crash then
        }

        container_wrapper(container_wrapper&& cont)
        {
            if (is_reference) m_cont = cont.m_cont;
            else m_cont = new container_decay(std::move(*cont.m_cont));
            m_extr = cont.m_extr; // can't move as the original would crash then
        }

        ~container_wrapper()
        {
            if (!is_reference) delete m_cont;
        }

        size_t size() const { return m_cont->size(); }
        void clear() { m_cont->clear(); }

        iterator begin() { return iterator(m_cont->begin(), m_extr); }
        iterator end() { return iterator(m_cont->end(), m_extr); }

        iterator insert(iterator it, value_type val)
        {
            if (is_const) throw std::runtime_error("container is const");
            return iterator(m_cont->insert(it.get_internal_iterator(), val), m_extr);
        }

        iterator erase(iterator it)
        {
            if (is_const) throw std::runtime_error("container is const");
            return iterator(m_cont->erase(it.get_internal_iterator()), m_extr);
        }
    };


    template<class T>
    class enumerator
    {
        class enumerator_impl_base
        {
        public:
            virtual ~enumerator_impl_base() {}
            virtual enumerator_impl_base* clone() const = 0;
            virtual void next() = 0;
            virtual void advance(size_t) = 0;
            virtual bool is_finished() const = 0;
            virtual void reset() = 0;
            virtual void erase() = 0;
            virtual void insert(const T&) = 0;
            virtual void insert(T&&) = 0;
            virtual size_t count() const = 0;
            virtual optional<T> get() = 0;
            virtual optional<T> get() const = 0;
        };

        template<class container, bool is_reference, bool is_const>
        class enumerator_impl : public enumerator_impl_base
        {
        private:
            typedef container_wrapper<container, T, is_reference, is_const> container_type;
            typedef decltype(((container_type*)0)->begin()) container_iterator;

            mutable container_type m_cont;
            container_iterator m_current;
            container_iterator m_next;

        public:
            enumerator_impl(container_type cont) : m_cont(cont), m_current(m_cont.begin()), m_next(std::next(m_current, 1)) {}
            enumerator_impl(const enumerator_impl* e) : m_cont(e.m_cont), m_current(e.m_current), m_next(e.m_next) {}
            enumerator_impl_base* clone() const { return new enumerator_impl(*this); }
            void next() { if (!is_finished()) { m_current = m_next; std::advance(m_next, 1); } }
            void advance(size_t n) { std::advance(m_current, n); m_next = std::next(m_current, 1); }
            bool is_finished() const { return (m_current == m_cont.end()); }
            void reset() { m_current = m_cont.begin(); m_next = std::next(m_current, 1); }
            void erase() { if (!is_finished()) { m_current = m_next = m_cont.erase(m_current); } }
            void insert(const T& t) { m_next = m_cont.insert(m_next, t); }
            void insert(T&& t) { m_next = m_cont.insert(m_next, t); }
            size_t count() const { return m_cont.size(); }
            optional<T> get() { if (!is_finished() && m_current != m_next) return *m_current; else return {}; }
            optional<T> get() const { if (!is_finished() && m_current != m_next) return *m_current; else return {}; }
        };

        enumerator_impl_base* m_enum;

    public:
        template<class container, bool is_reference, bool is_const>
        enumerator(container_wrapper<container, T, is_reference, is_const> cont)
         : m_enum(new enumerator_impl<container, is_reference, is_const>(cont)) {}

        enumerator() : m_enum(nullptr) {}
        enumerator(const enumerator& e) { m_enum = ((e.m_enum == nullptr) ? nullptr : e.m_enum->clone()); }
        enumerator(enumerator&& e) { m_enum = e.m_enum; e.m_enum = nullptr; }
        ~enumerator() { if (m_enum != nullptr) delete m_enum; }

        enumerator& operator= (const enumerator& e)
        { if (m_enum != nullptr) delete m_enum; m_enum = (e.m_enum == nullptr) ? nullptr : e.m_enum->clone(); }
        enumerator& operator= (enumerator&& e)
        { if (m_enum != nullptr) delete m_enum; m_enum = e.m_enum; e.m_enum = nullptr; }

        void next() { if (m_enum != nullptr) m_enum->next(); else throw std::runtime_error("empty enumerator"); }
        void advance(size_t n) { if (m_enum != nullptr) m_enum->advance(n); else throw std::runtime_error("empty enumerator"); }
        bool is_finished() const { if (m_enum != nullptr) return m_enum->is_finished(); else throw std::runtime_error("empty enumerator"); }
        void reset() { if (m_enum != nullptr) m_enum->reset(); else throw std::runtime_error("empty enumerator"); }
        void erase() { if (this->m_enum != nullptr) this->m_enum->erase(); else throw std::runtime_error("empty enumerator"); }
        void insert(const T& t) { if (this->m_enum != nullptr) this->m_enum->insert(t); else throw std::runtime_error("empty enumerator"); }
        void insert(T&& t) { if (this->m_enum != nullptr) this->m_enum->insert(t); else throw std::runtime_error("empty enumerator"); }
        size_t count() const { if (m_enum != nullptr) return m_enum->count(); else return 0; }
        optional<T> get() { if (m_enum != nullptr) return m_enum->get(); else return {}; }
        optional<T> get() const { if (m_enum != nullptr) return m_enum->get(); else return {}; }
    };


    template<class T, class container>
    enumerator<T> make_enumerator(container cont, typename container_wrapper<container, T>::extractor extr = {})
    {
        container_wrapper<container, T, false> cw(cont, extr);
        return std::move(cw);
    }

    template<class T, class container>
    enumerator<T> make_ref_enumerator(container& cont, typename container_wrapper<container, T>::extractor extr = {})
    {
        static_assert(std::is_const<container>::value || !std::is_lvalue_reference<container>::value,
                      "container is const or not lvalue reference");

        container_wrapper<container, T, true> cw(cont, extr);
        return std::move(cw);
    }

    template<class T, class container>
    enumerator<T> make_const_enumerator(container cont, typename container_wrapper<container, T>::extractor extr = {})
    {
        container_wrapper<container, T, false, true> cw(cont, extr);
        return std::move(cw);
    }
};

#endif // GG_ENUMERATOR_HPP_INCLUDED
