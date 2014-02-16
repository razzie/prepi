#ifndef GG_SCRIPTENG_HPP_INCLUDED
#define GG_SCRIPTENG_HPP_INCLUDED

#include <type_traits>
#include "gg/function.hpp"
#include "gg/optional.hpp"
#include "gg/console.hpp"

namespace gg
{
    class application;

    class script_engine
    {
    protected:
        virtual ~script_engine() {}

        template<class T>
        static const char* get_arg()
        {
            if (std::is_same<T, varlist>::value) return ",( )";
            else if (std::is_arithmetic<T>::value) return ",0";
            else return ",\"\"";
        }

        template<class... Args>
        static std::string get_args()
        {
            std::string args;

            struct { void operator() (...) {} } expand;
            expand( & args.insert(0, get_arg<Args>())... );

            if (!args.empty()) args.erase(args.begin());
            args.insert(args.begin(), '(');
            args.insert(args.end(), ')');

            return args;
        }

        template<class>
        struct get_nested_args_impl;

        template<class R, class... Args>
        struct get_nested_args_impl<R(Args...)>
        {
            std::string operator()() const { return get_args<Args...>(); }
        };

        template<class F>
        static std::string get_nested_args()
        {
            get_nested_args_impl<meta::get_signature<F>> args;
            return args();
        }

    public:
        virtual application* get_app() const = 0;
        virtual void enable_remote_access() = 0;
        virtual void disable_remote_access() = 0;
        virtual void add_function(std::string fn, dynamic_function func, std::string args, bool hidden = false) = 0;
        virtual void remove_function(std::string fn) = 0;
        virtual optional<var> exec(std::string fn, varlist vl, std::ostream& output = std::cout) const = 0;
        virtual optional<var> parse_and_exec(std::string expr, std::ostream& output = std::cout) const = 0;
        virtual console::controller* create_console_controller() const = 0;

        template<class R, class... Args>
        void add_function(std::string fn, std::function<R(Args...)> func, bool hidden = false)
        {
            this->add_function(fn, func, get_args<Args...>(), hidden);
        }

        template<class R, class... Args>
        void add_function(std::string fn, R(*func)(Args...), bool hidden = false)
        {
            this->add_function(fn, func, get_args<Args...>(), hidden);
        }

        template<class F>
        void add_function(std::string fn, F func, bool hidden = false)
        {
            this->add_function(fn, func, get_nested_args<F>(), hidden);
        }
    };
};

#endif // GG_SCRIPTENG_HPP_INCLUDED
