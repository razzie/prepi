#ifndef GG_FILESYSTEM_HPP_INCLUDED
#define GG_FILESYSTEM_HPP_INCLUDED

#include <iosfwd>
#include <string>
#include "gg/refcounted.hpp"
#include "gg/enumerator.hpp"

namespace gg
{
    class file : public reference_counted
    {
    protected:
        virtual ~file() {}

    public:
        virtual std::string get_name() const = 0;
        virtual std::string get_extension() const = 0;
        virtual std::string get_full_name() const = 0;
        virtual std::string get_path() const = 0;
        virtual std::fstream&& get_fstream() const = 0;
        virtual size_t get_size() const = 0;
    };

    class directory : public reference_counted
    {
    protected:
        virtual ~directory() {}

    public:
        static directory* open(std::string);
        virtual std::string get_name() const = 0;
        virtual std::string get_full_name() const = 0;
        virtual std::string get_path() const = 0;
        virtual enumerator<file*> get_files() const = 0;
        virtual enumerator<file*> get_files_recursive() const = 0;
        virtual enumerator<directory*> get_directories() const = 0;
        virtual enumerator<directory*> get_directories_recursive() const = 0;
    };
};

#endif // GG_FILESYSTEM_HPP_INCLUDED
