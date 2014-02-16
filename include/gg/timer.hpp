#ifndef GG_TIMER_HPP_INCLUDED
#define GG_TIMER_HPP_INCLUDED

#include "gg/refcounted.hpp"

namespace gg
{
    class timer : public reference_counted
    {
    protected:
        virtual ~timer() {};

    public:
        static timer* create();

        virtual uint32_t get_elapsed() = 0;
        virtual uint32_t peek_elapsed() const = 0;
        virtual void reset() = 0;
    };
};

#endif // GG_TIMER_HPP_INCLUDED
