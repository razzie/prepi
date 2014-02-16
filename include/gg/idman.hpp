#ifndef GG_IDMAN_HPP_INCLUDED
#define GG_IDMAN_HPP_INCLUDED

#include <cstdint>
#include <string>

namespace gg
{
    class application;

    class id
    {
        uint32_t m_id;

    public:
        id(uint32_t);
        id(std::string); // init from hex
        id(const id&);
        ~id();
        id& operator= (const id&);
        bool operator== (const id&) const;
        bool operator!= (const id&) const;
        operator uint32_t() const;
        std::string get_hex() const;

        struct comparator
        {
            comparator() = default;
            ~comparator() = default;
            bool operator() (const id&, const id&) const;
        };

        enum : uint32_t
        {
            any = 0xffffffff,
            invalid = 0x00000000
        };
    };

    class id_manager
    {
    protected:
        virtual ~id_manager() {}

    public:
        virtual application* get_app() const = 0;
        virtual id get_random_id() const = 0;
        virtual id get_unique_id() = 0;
        virtual bool reserve_id(id) = 0;
        virtual void release_id(id) = 0;
        virtual bool is_unique(id) const = 0;
    };
};

#endif // GG_IDMAN_HPP_INCLUDED
