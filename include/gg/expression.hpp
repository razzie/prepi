#ifndef GG_EXPRESSION_HPP_INCLUDED
#define GG_EXPRESSION_HPP_INCLUDED

#include <string>
#include <functional>
#include "gg/refcounted.hpp"
#include "gg/enumerator.hpp"

namespace gg
{
    class expression : public reference_counted
    {
    public:
        static expression* create(std::string expr, bool auto_complete = false);

        virtual ~expression() {};
        virtual void set_name(std::string name) = 0;
        virtual std::string get_name() const = 0;
        virtual std::string get_expression() const = 0;
        virtual bool is_root() const = 0;
        virtual bool is_leaf() const = 0;
        virtual bool is_empty() const = 0;
        virtual enumerator<expression*> get_children() = 0;
        virtual enumerator<expression*> get_children() const = 0;
        virtual void for_each(std::function<void(expression&)>) = 0;
        virtual void for_each(std::function<void(const expression&)>) const = 0;

        friend std::ostream& operator<< (std::ostream& o, const expression& e);
    };

    class expression_error : public std::exception
    {
    public:
        virtual ~expression_error() noexcept {};
        virtual const char* what() const noexcept = 0;
    };
};

#endif // GG_EXPRESSION_HPP_INCLUDED
