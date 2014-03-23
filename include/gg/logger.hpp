#ifndef GG_LOGGER_HPP_INCLUDED
#define GG_LOGGER_HPP_INCLUDED

#include <iosfwd>
#include <string>

namespace gg
{
    class console;

    class logger : public virtual std::ostream
    {
    protected:
        virtual ~logger() {}

    public:
        class scoped_hook
        {
        public:
            scoped_hook(std::ostream&);
            scoped_hook(const scoped_hook&) = delete;
            scoped_hook(scoped_hook&&) = delete;
            ~scoped_hook();
        };

        virtual void enable_timestamp() = 0;
        virtual void disable_timestamp() = 0;
        virtual void log_to_stream(std::ostream&) = 0;
        virtual void log_to_file(std::string) = 0;
        virtual void log_to_console(console*) = 0;
        virtual std::ostream& get_nullstream() const = 0;
    };
};

#endif // GG_LOGGER_HPP_INCLUDED
