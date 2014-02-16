#ifndef GG_FUNCTION_HPP_INCLUDED
#define GG_FUNCTION_HPP_INCLUDED

#include <functional>
#include <stdexcept>
#include "gg/var.hpp"

namespace gg
{
    namespace meta
    {
        template<class T>
        struct remove_class { };

        template<class C, class R, class... Args>
        struct remove_class<R(C::*)(Args...)> { using type = R(Args...); };

        template<class C, class R, class... Args>
        struct remove_class<R(C::*)(Args...) const> { using type = R(Args...); };

        template<class C, class R, class... Args>
        struct remove_class<R(C::*)(Args...) volatile> { using type = R(Args...); };

        template<class C, class R, class... Args>
        struct remove_class<R(C::*)(Args...) const volatile> { using type = R(Args...); };

        template<class T>
        struct get_signature_impl { using type = typename remove_class<
            decltype(&std::remove_reference<T>::type::operator())>::type; };

        template<class R, class... Args>
        struct get_signature_impl<R(Args...)> { using type = R(Args...); };

        template<class R, class... Args>
        struct get_signature_impl<R(&)(Args...)> { using type = R(Args...); };

        template<class R, class... Args>
        struct get_signature_impl<R(*)(Args...)> { using type = R(Args...); };

        template<class T>
        using get_signature = typename get_signature_impl<T>::type;
    };

    template<class>
    class function;

    template<class R, class... Args>
    class function<R(Args...)>
    {
        std::function<R(Args...)> m_func;

        template<class _R, class... _Args>
        static _R _invoke(std::function<_R(_Args...)> func, varlist vl);

        template<class _R>
        static _R _invoke(std::function<_R()> func, varlist vl)
        {
            if (vl.size() > 0)
                throw std::runtime_error("too long argument list");

            return func();
        }

        template<class _R, class _Arg0, class... _Args>
        static _R _invoke(std::function<_R(_Arg0, _Args...)> func, varlist vl)
        {
            if (vl.size() == 0)
                throw std::runtime_error("too short argument list");

            _Arg0 arg0 = vl[0].cast<_Arg0>();
            vl.erase(vl.begin());
            std::function<_R(_Args... args)> lambda =
                [=](_Args... args) -> _R { return func(arg0, args...); };

            return _invoke(lambda, vl);
        }

        template<class _R, class... _Args>
        static _R _invoke(_R(*func)(_Args...))
        {
            return _invoke(std::function<_R(_Args...)>(func));
        }

        template<class _F>
        static typename std::result_of<meta::get_signature<_F>>::type
        _invoke(_F func)
        {
            return _invoke(std::function<meta::get_signature<_F>>(func));
        }

    public:
        using type = R(Args...);
        using return_type = R;

        function() {}
        function(const gg::function<R(Args...)>& func) : m_func(func.m_func) {}
        function(gg::function<R(Args...)>&& func) : m_func(std::move(func.m_func)) {}
        ~function() {}

        template<class F>
        function(F func) : m_func(func) {}
        function(R(*func)(Args...)) : m_func(func) {}
        function(std::function<R(Args...)> func) : m_func(func) {}

        template<class F>
        function<R(Args...)>& operator= (F func) { m_func = func; return *this; }
        function<R(Args...)>& operator= (R(*func)(Args...)) { m_func = func; return *this; }
        function<R(Args...)>& operator= (std::function<R(Args...)> func) { m_func = func; return *this; }

        function& operator= (const gg::function<R(Args...)>& func) { m_func = func.m_func; return *this; }
        function& operator= (gg::function<R(Args...)>&& func) { m_func = std::move(func.m_func); return *this; }

        R operator() (Args... args) const { return m_func(std::forward<Args>(args)...); }
        R invoke(varlist vl) const { return _invoke(m_func, vl); }
        operator bool() const { return static_cast<bool>(m_func); }
        operator std::function<R(Args...)>() const { return m_func; }
    };

    class dynamic_function
    {
        gg::function<var(varlist)> m_func;

        template<class R, class... Args>
        static gg::function<var(varlist)> convert(gg::function<R(Args...)> func)
        {
            return ([=](varlist vl)->var { return func.invoke(vl); });
        }

        template<class... Args>
        static gg::function<var(varlist)> convert(gg::function<void(Args...)> func)
        {
            return ([=](varlist vl)->var { func.invoke(vl); return var(); });
        }

    public:
        dynamic_function();
        dynamic_function(const dynamic_function& func);
        dynamic_function(dynamic_function&& func);
        ~dynamic_function();

        template<class R, class... Args>
        dynamic_function(gg::function<R(Args...)> func)
         : m_func(convert(func)) {}

        template<class R, class... Args>
        dynamic_function(std::function<R(Args...)> func)
         : m_func(convert( gg::function<R(Args...)>(func) )) {}

        template<class R, class... Args>
        dynamic_function(R(*func)(Args...))
         : m_func(convert( gg::function<R(Args...)>(func) )) {}

        template<class F>
        dynamic_function(F func)
         : m_func(convert( gg::function<meta::get_signature<F>>(func) )) {}

        dynamic_function& operator= (const dynamic_function& func);
        dynamic_function& operator= (dynamic_function&& func);

        var operator() (varlist vl) const;
        operator bool() const;
        operator gg::function<var(varlist)>() const;
        operator std::function<var(varlist)>() const;
    };
};

#endif // GG_FUNCTION_HPP_INCLUDED
