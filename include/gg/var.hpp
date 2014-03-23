#ifndef GG_VAR_HPP_INCLUDED
#define GG_VAR_HPP_INCLUDED

#include <iosfwd>
#include <string>
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <stdexcept>
#include "gg/streamops.hpp"

namespace gg
{
    class var
    {
        class var_impl_base
        {
        public:
            virtual ~var_impl_base() {}
            virtual var_impl_base* clone() const = 0;
            virtual void* get_ptr() = 0;
            virtual const void* get_ptr() const = 0;
            virtual const std::type_info& get_type() const = 0;
            virtual void extract_to(std::ostream&) const = 0;
        };

        template<class T>
        class var_impl : public var_impl_base
        {
            T m_var;
            const std::type_info* m_type;

        public:
            template<class... Args>
            var_impl(Args... args) : m_var(std::forward<Args>(args)...), m_type(&typeid(T)) {}
            ~var_impl() {}
            var_impl_base* clone() const { return new var_impl<T>(m_var); }
            void* get_ptr() { return static_cast<void*>(&m_var); }
            const void* get_ptr() const { return static_cast<const void*>(&m_var); }
            const std::type_info& get_type() const { return *m_type; }
            void extract_to(std::ostream& o) const { ostream_insert(o, m_var); }
        };

        template<class T>
        class var_ref_impl : public var_impl_base
        {
            T& m_var;
            const std::type_info* m_type;

        public:
            var_ref_impl(T& t) : m_var(t), m_type(&typeid(T)) {}
            ~var_ref_impl() {}
            var_impl_base* clone() const { return new var_ref_impl<T>(m_var); }
            void* get_ptr() { return static_cast<void*>(&m_var); }
            const void* get_ptr() const { return static_cast<const void*>(&m_var); }
            const std::type_info& get_type() const { return *m_type; }
            void extract_to(std::ostream& o) const { ostream_insert(o, m_var); }
        };

        template<class T>
        class var_const_ref_impl : public var_impl_base
        {
            const T& m_var;
            const std::type_info* m_type;

        public:
            var_const_ref_impl(const T& t) : m_var(t), m_type(&typeid(T)) {}
            ~var_const_ref_impl() {}
            var_impl_base* clone() const { return new var_const_ref_impl<T>(m_var); }
            void* get_ptr() { throw std::runtime_error("can't access const reference"); }
            const void* get_ptr() const { return static_cast<const void*>(&m_var); }
            const std::type_info& get_type() const { return *m_type; }
            void extract_to(std::ostream& o) const { ostream_insert(o, m_var); }
        };

        var_impl_base* m_var = nullptr;

    public:
        var();
        var(const var& v);
        var(var&& v);
        ~var();

        template<class T>
        var(T t) : m_var(new var_impl<T>(t)) {}

        template<class T, class... Args>
        var& construct(Args... args)
        {
            if (m_var != nullptr) delete m_var;
            m_var = new var_impl<T>(std::forward<Args>(args)...);
            return *this;
        }

        template<class T>
        var& reference(T& t)
        {
            if (m_var != nullptr) delete m_var;
            m_var = new var_ref_impl<T>(t);
            return *this;
        }

        template<class T>
        var& const_reference(const T& t)
        {
            if (m_var != nullptr) delete m_var;
            m_var = new var_const_ref_impl<T>(t);
            return *this;
        }

        template<class T>
        var& operator= (const T& t)
        {
            if (m_var != nullptr) delete m_var;
            m_var = new var_impl<T>(t);
            return *this;
        }

        var& operator= (const var& v);
        var& operator= (var&& v);
        const std::type_info& get_type() const;
        bool is_empty() const;
        void clear();

        template<class T>
        T* get_ptr()
        {
            if (m_var == nullptr)
                throw std::runtime_error("get() called on empty var");

            if (m_var->get_type() != typeid(T))
                throw std::runtime_error("var type mismatch");

            return static_cast<T*>(m_var->get_ptr());
        }

        template<class T>
        const T* get_ptr() const
        {
            if (m_var == nullptr)
                throw std::runtime_error("get() called on empty var");

            if (m_var->get_type() != typeid(T))
                throw std::runtime_error("var type mismatch");

            return static_cast<const T*>(m_var->get_ptr());
        }

        template<class T> T& get() { return *get_ptr<T>(); }
        template<class T> const T& get() const { return *get_ptr<T>(); }
        template<class T> operator T() const { return this->get<T>(); }

        template<class T, class = typename std::enable_if<std::is_same<T, var>::value>::type>
        var cast() const { return var(*this); }

        template<class T, class = typename std::enable_if<!std::is_same<T, var>::value>::type>
        T cast() const
        {
            if (m_var == nullptr)
                throw std::runtime_error("casting empty var");

            if (m_var->get_type() == typeid(T))
                return *static_cast<const T*>(m_var->get_ptr());

            if (!meta::has_extract_op<T>::value)
                throw std::runtime_error("unable to cast");

            T result;
            std::stringstream ss;

            m_var->extract_to(ss);
            istream_extract(ss, result);

            return result;
        }

        class view
        {
            friend std::ostream& operator<< (std::ostream& o, const gg::var::view& vw);
            const var& m_var;

        public:
            view(const var& var);
            view(const view& vw);
            ~view();
        };

        view to_stream() const;
        std::string to_string() const;
        friend std::ostream& operator<< (std::ostream& o, const gg::var::view& vw);
    };

    typedef std::vector<var> varlist;

    std::ostream& operator<< (std::ostream& o, const varlist& vl);
};

extern template class std::vector<gg::var>;

#endif // GG_VAR_HPP_INCLUDED
