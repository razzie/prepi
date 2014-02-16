#ifndef GG_BUFFER_HPP_INCLUDED
#define GG_BUFFER_HPP_INCLUDED

#include <iostream>
#include <cstdint>
#include <vector>
#include "gg/refcounted.hpp"
#include "gg/optional.hpp"

namespace gg
{
    class buffer : public reference_counted
    {
    protected:
        virtual ~buffer() {}

    public:
        typedef std::vector<uint8_t> byte_array;

        static buffer* create();

        virtual size_t available() const = 0;
        virtual void advance(size_t len) = 0;
        virtual void clear() = 0;

        virtual byte_array peek(size_t len) const = 0;
        virtual byte_array peek(size_t start_pos, size_t len) const = 0;
        virtual size_t peek(uint8_t* buf, size_t len) const = 0;
        virtual size_t peek(size_t start_pos, uint8_t* buf, size_t len) const = 0;

        virtual void push(uint8_t byte) = 0;
        virtual void push(const uint8_t* buf, size_t len) = 0;
        virtual void push(const byte_array& buf) = 0;
        virtual void push(const buffer* buf) = 0;
        virtual void merge(buffer* buf) = 0;

        virtual optional<uint8_t> pop() = 0;
        virtual byte_array pop(size_t len) = 0;
        virtual size_t pop(uint8_t* buf, size_t len) = 0;
    };

    std::ostream& operator<< (std::ostream&, const buffer&);
    std::ostream& operator<< (std::ostream&, const buffer*);
};

#endif // GG_BUFFER_HPP_INCLUDED
