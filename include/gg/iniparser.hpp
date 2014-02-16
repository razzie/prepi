#ifndef GG_INIPARSER_HPP_INCLUDED
#define GG_INIPARSER_HPP_INCLUDED

#include <iostream>
#include <list>
#include <string>
#include <memory>
#include "gg/refcounted.hpp"
#include "gg/optional.hpp"

namespace gg
{
    class ini_parser : public reference_counted
    {
    public:
        class entry;

        class section
        {
        protected:
            virtual ~section() {}

        public:
            virtual std::string get_name() const = 0;
            virtual void set_name(std::string) = 0;
            virtual entry& operator[] (std::string) = 0;
            virtual entry* get_entry(std::string) = 0;
            virtual const entry* get_entry(std::string) const = 0;
            virtual entry* add_entry(std::string, std::string) = 0;
            virtual void remove_entry(std::string) = 0;
            virtual void remove_entry(entry*) = 0;
            virtual std::list<entry*>& get_entries() = 0;
            virtual const std::list<entry*>& get_entries() const = 0;
        };

        class entry
        {
        protected:
            virtual ~entry() {}

        public:
            virtual section* get_section() = 0;
            virtual const section* get_section() const = 0;
            virtual std::string get_key() const = 0;
            virtual void set_key(std::string) = 0;
            virtual std::string get_value() const = 0;
            virtual void set_value(std::string) = 0;
        };

        static ini_parser* create(std::string file);
        static ini_parser* create(std::istream&);
        virtual ~ini_parser() {}

        virtual entry* get_entry(std::string, std::string) = 0;
        virtual const entry* get_entry(std::string, std::string) const = 0;
        virtual entry* add_entry(std::string section, std::string key, std::string value = {}) = 0;
        virtual void remove_entry(std::string, std::string) = 0;
        virtual void remove_entry(entry*) = 0;

        virtual section& operator[] (std::string) = 0;
        virtual section* get_section(std::string) = 0;
        virtual const section* get_section(std::string) const = 0;
        virtual section* create_section(std::string) = 0;
        virtual void remove_section(std::string) = 0;
        virtual void remove_section(section*) = 0;
        virtual std::list<section*>& get_sections() = 0;
        virtual const std::list<section*>& get_sections() const = 0;

        virtual void save(std::string file) = 0;
        virtual void save(std::ostream&) = 0;
    };
};

#endif // GG_INIPARSER_HPP_INCLUDED
