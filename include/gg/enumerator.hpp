#ifndef GG_ENUMERATOR_HPP_INCLUDED
#define GG_ENUMERATOR_HPP_INCLUDED

#include <functional>
#include <iterator>
#include <type_traits>
#include <stdexcept>
#include "gg/optional.hpp"

namespace gg
{
    template<class T>
    class const_enumerator
    {
    protected:
        class enumerator_impl_base
        {
        public:
            virtual ~enumerator_impl_base() {}
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
            virtual enumerator_impl_base* clone() const = 0;
        };

        enumerator_impl_base* m_enum;


        template<class iterator> using compatible_iterator =
            typename std::enable_if<std::is_same<T, typename iterator::value_type>::value>::type;

        template<class iterator,
                 class = compatible_iterator<iterator>>
        class enumerator_impl : public enumerator_impl_base
        {
            iterator m_begin;
            iterator m_end;
            iterator m_current;

        public:
            enumerator_impl(iterator begin, iterator end) : m_begin(begin), m_end(end), m_current(begin) {}
            enumerator_impl(const enumerator_impl& e) : m_begin(e.m_begin), m_end(e.m_end), m_current(e.m_current) {}
            ~enumerator_impl() {}
            enumerator_impl_base* clone() const { return new enumerator_impl(*this); }
            void next() { std::advance(m_current, 1); }
            void advance(size_t n) { std::advance(m_current, n); }
            bool is_finished() const { return (m_current == m_end); }
            void reset() { m_current = m_begin; }
            void erase() { throw std::runtime_error("erase error"); }
            void insert(const T&) { throw std::runtime_error("insert error"); }
            void insert(T&&) { throw std::runtime_error("insert error"); }
            size_t count() const { return std::distance(m_begin, m_end); }
            optional<T> get() { if (!is_finished()) return *m_current; else return {}; }
            optional<T> get() const { if (!is_finished()) return *m_current; else return {}; }
        };

        template<class container> using compatible_container =
            typename std::enable_if<std::is_same<T, typename container::value_type>::value>::type;

        template<class container> using const_container =
            typename std::enable_if<std::is_const<container>::value>::type;

        template<class container> using non_const_container =
            typename std::enable_if<!std::is_const<container>::value>::type;

        template<class container, class iterator = typename container::iterator,
                 class = compatible_container<container>,
                 class = compatible_iterator<iterator>>
        class container_enumerator_impl : public enumerator_impl_base
        {
            container* m_cont;
            iterator m_current;
            iterator m_next;

        public:
            container_enumerator_impl(container& c) : m_cont(&c), m_current(m_cont->begin()), m_next(std::next(m_current, 1)) {}
            container_enumerator_impl(const container_enumerator_impl& c) : m_cont(c.m_cont), m_current(c.m_current), m_next(c.m_next) {}
            ~container_enumerator_impl() {}
            enumerator_impl_base* clone() const { return new container_enumerator_impl(*this); }
            void next() { if (!is_finished()) { m_current = m_next; std::advance(m_next, 1); } }
            void advance(size_t n) { std::advance(m_current, n); m_next = std::next(m_current, 1); }
            bool is_finished() const { return (m_current == m_cont->end()); }
            void reset() { m_current = m_cont->begin(); m_next = std::next(m_current, 1); }
            void erase() { if (!is_finished()) { m_current = m_next = m_cont->erase(m_current); } }
            void insert(const T& t) { m_next = m_cont->insert(m_next, t); }
            void insert(T&& t) { m_next = m_cont->insert(m_next, t); }
            size_t count() const { return m_cont->size(); }
            optional<T> get() { if (!is_finished() && m_current != m_next) return *m_current; else return {}; }
            optional<T> get() const { if (!is_finished() && m_current != m_next) return *m_current; else return {}; }
        };

        template<class container, class iterator = typename container::const_iterator,
                 class = compatible_container<container>,
                 class = compatible_iterator<iterator>>
        class const_container_enumerator_impl : public enumerator_impl_base
        {
            container* m_cont;
            iterator m_current;

        public:
            const_container_enumerator_impl(container& c) : m_cont(&c), m_current(m_cont->begin()) {}
            const_container_enumerator_impl(const const_container_enumerator_impl& c) : m_cont(c.m_cont), m_current(c.m_current) {}
            ~const_container_enumerator_impl() {}
            enumerator_impl_base* clone() const { return new const_container_enumerator_impl(*this); }
            void next() { if (!is_finished()) { std::advance(m_current, 1); } }
            void advance(size_t n) { std::advance(m_current, n); }
            bool is_finished() const { return (m_current == m_cont->end()); }
            void reset() { m_current = m_cont->begin(); }
            void erase() { throw std::runtime_error("erase error"); }
            void insert(const T&) { throw std::runtime_error("insert error"); }
            void insert(T&&) { throw std::runtime_error("insert error"); }
            size_t count() const { return m_cont->size(); }
            optional<T> get() { if (!is_finished()) return *m_current; else return {}; }
            optional<T> get() const { if (!is_finished()) return *m_current; else return {}; }
        };

        template<class container, class iterator = typename container::iterator>
        class container_holder_enumerator_impl : public container_enumerator_impl<container, iterator>
        {
            container m_cont_holder;

        public:
            container_holder_enumerator_impl(container&& c)
             : m_cont_holder(c), container_enumerator_impl<container, iterator>(m_cont_holder) {}
        };

        template<class U> using disable_reference =
            typename std::enable_if<!std::is_reference<U>::value>::type;

    public:
        template<class iterator>
        const_enumerator(iterator begin, iterator end) : m_enum(new enumerator_impl<iterator>(begin, end)) {}

        template<class container, class = disable_reference<container>>
        const_enumerator(container& cont, non_const_container<container>* = 0)
         : m_enum(new container_enumerator_impl<container>(cont)) {}

        template<class container, class = disable_reference<container>>
        const_enumerator(container& cont, const_container<container>* = 0)
         : m_enum(new const_container_enumerator_impl<container>(cont)) {}

        template<class container, class = disable_reference<container>>
        const_enumerator(container&& cont)
         : m_enum(new container_holder_enumerator_impl<container>(cont)) {}

        const_enumerator() : m_enum(nullptr) {}
        const_enumerator(const const_enumerator& e) { m_enum = ((e.m_enum == nullptr) ? nullptr : e.m_enum->clone()); }
        const_enumerator(const_enumerator&& e) { m_enum = e.m_enum; e.m_enum = nullptr; }
        ~const_enumerator() { if (m_enum != nullptr) delete m_enum; }

        const_enumerator& operator= (const const_enumerator& e)
        { if (m_enum != nullptr) delete m_enum; m_enum = (e.m_enum == nullptr) ? nullptr : e.m_enum->clone(); }
        const_enumerator& operator= (const_enumerator&& e)
        { if (m_enum != nullptr) delete m_enum; m_enum = e.m_enum; e.m_enum = nullptr; }

        void next() { if (m_enum != nullptr) m_enum->next(); else throw std::runtime_error("empty enumerator"); }
        void advance(size_t n) { if (m_enum != nullptr) m_enum->advance(n); else throw std::runtime_error("empty enumerator"); }
        bool is_finished() const { if (m_enum != nullptr) return m_enum->is_finished(); else throw std::runtime_error("empty enumerator"); }
        void reset() { if (m_enum != nullptr) m_enum->reset(); else throw std::runtime_error("empty enumerator"); }
        size_t count() const { if (m_enum != nullptr) return m_enum->count(); else return 0; }
        optional<T> get() { if (m_enum != nullptr) return m_enum->get(); else return {}; }
        optional<T> get() const { if (m_enum != nullptr) return m_enum->get(); else return {}; }
    };

    template<class T>
    class enumerator : public const_enumerator<T>
    {
    public:
        template<class iterator>
        enumerator(iterator begin, iterator end) : const_enumerator<T>(begin, end) {}

        template<class container, class = const_enumerator<T>::disable_reference<container>>
        enumerator(container& cont) : const_enumerator<T>(cont) {}

        template<class container, class = const_enumerator<T>::disable_reference<container>>
        enumerator(container&& cont) : const_enumerator<T>(cont) {}

        enumerator() {}
        enumerator(const enumerator& e) { this->m_enum = ((e.m_enum == nullptr) ? nullptr : e.m_enum->clone()); }
        enumerator(enumerator&& e) { this->m_enum = e.m_enum; e.m_enum = nullptr; }
        ~enumerator() { /* implemented in const_enumerator already */ }

        enumerator& operator= (const enumerator& e)
        { if (this->m_enum != nullptr) delete this->m_enum; this->m_enum = (e.m_enum == nullptr) ? nullptr : e.m_enum->clone(); }
        enumerator& operator= (enumerator&& e)
        { if (this->m_enum != nullptr) delete this->m_enum; this->m_enum = e.m_enum; e.m_enum = nullptr; }

        void erase() { if (this->m_enum != nullptr) this->m_enum->erase(); else throw std::runtime_error("empty enumerator"); }
        void insert(const T& t) { if (this->m_enum != nullptr) this->m_enum->insert(t); else throw std::runtime_error("empty enumerator"); }
        void insert(T&& t) { if (this->m_enum != nullptr) this->m_enum->insert(t); else throw std::runtime_error("empty enumerator"); }
    };

    template<class iterator, class T>
    class conversion_iterator : public std::iterator<std::forward_iterator_tag, T>
    {
    public:
        typedef T value_type;

    private:
        iterator m_iter;
        std::function<value_type&(typename iterator::value_type&)> m_conv;

    public:
        conversion_iterator(iterator it, std::function<value_type&(typename iterator::value_type&)> conv)
         : m_iter(it), m_conv(conv) {}
        conversion_iterator(const conversion_iterator& it)
         : m_iter(it.m_iter), m_conv(it.m_conv) {}
        ~conversion_iterator() {}

        conversion_iterator& operator= (const conversion_iterator& it)
        { m_iter = it.m_iter; m_conv = it.m_conv; return *this; }

        conversion_iterator& operator++ () { ++m_iter; return *this; }
        conversion_iterator& operator++ (int) { m_iter++; return *this; }

        bool operator== (const conversion_iterator& it) const { return (m_iter == it.m_iter); }
        bool operator!= (const conversion_iterator& it) const { return (m_iter != it.m_iter); }

        value_type& operator* () { return m_conv(*m_iter); }
        value_type* operator-> () { return &m_conv(*m_iter); }
        const value_type& operator* () const { return m_conv(*m_iter); }
        const value_type* operator-> () const { return &m_conv(*m_iter); }

        iterator get_internal_iterator() { return m_iter; }
    };

    template<class container, class T>
    class conversion_container
    {
    public:
        typedef T value_type;
        typedef conversion_iterator<typename container::iterator, value_type> iterator;

    private:
        container m_cont;
        std::function<value_type&(typename container::iterator::value_type&)> m_conv;

    public:
        conversion_container(container&& cont, std::function<value_type&(typename container::iterator::value_type&)> conv)
         : m_cont(cont), m_conv(conv) {}
        conversion_container(const conversion_container& cont)
         : m_cont(cont.m_cont), m_conv(cont.m_conv) {}
        conversion_container(conversion_container&& cont)
         : m_cont(std::move(cont.m_cont)), m_conv(std::move(cont.m_conv)) {}
        ~conversion_container() {}

        conversion_container& operator= (const conversion_container& cont)
        { m_cont.clear(); m_cont.insert(cont.m_cont.begin(), cont.m_cont.end()); m_conv = cont.m_conv; return *this; }
        conversion_container& operator= (conversion_container&& cont)
        { std::swap(m_cont, cont.m_cont); m_conv = std::move(cont.m_conv); return *this; }

        iterator begin() { return iterator(m_cont.begin(), m_conv); }
        iterator end() { return iterator(m_cont.end(), m_conv); }
        iterator insert(iterator it, value_type val) { return iterator(m_cont.insert(it.get_internal_iterator(), val), m_conv); }
        iterator erase(iterator it) { return iterator(m_cont.erase(it.get_internal_iterator()), m_conv); }
        size_t size() const { return m_cont.size(); }
        void clear() { m_cont.clear(); }
    };
};

#endif // GG_ENUMERATOR_HPP_INCLUDED
