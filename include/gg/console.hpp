#ifndef GG_CONSOLE_HPP_INCLUDED
#define GG_CONSOLE_HPP_INCLUDED

#include <iostream>
#include <cstdint>
#include <string>
#include <functional>
#include "gg/refcounted.hpp"

namespace gg
{
    class application;

    class console : public reference_counted
    {
    protected:
        virtual ~console() {};

    public:
        static console* get_invoker_console();

        class output : public reference_counted, public virtual std::ostream
        {
        protected:
            virtual ~output() {}

        public:
            struct color
            {
                uint8_t R, G, B;
            };

            virtual console& get_console() const = 0;
            virtual void show() = 0;
            virtual void hide() = 0;
            virtual void set_color(color) = 0;
            virtual color get_color() const = 0;
            virtual void align_left() = 0;
            virtual void align_center() = 0;
            virtual void align_right() = 0;
            virtual void print(std::string) = 0;
            virtual bool is_empty() const = 0;
            virtual std::string to_string() const = 0;
        };

        class controller : public reference_counted
        {
        protected:
            virtual ~controller() {}

        public:
            enum class exec_result
            {
                EXEC_SUCCESS,
                EXEC_FAIL,
                NO_EXEC
            };

            virtual exec_result exec(std::string& cmd, output&) = 0;
            virtual void complete(std::string& cmd, output&) = 0;
        };

        virtual application* get_app() const = 0;
        virtual std::string get_name() const = 0;
        virtual void set_name(std::string name) = 0;
        virtual controller* get_controller() const = 0;
        virtual void set_controller(controller* ctrl) = 0;
        virtual void enable_input() = 0;
        virtual void disable_input() = 0;
        virtual void enable_argument_fill_helper() = 0;
        virtual void disable_argument_fill_helper() = 0;
        virtual void open() = 0;
        virtual void close() = 0;
        virtual bool is_opened() const = 0;
        virtual void on_close(std::function<void()> callback) = 0;
        virtual output* create_output() = 0;
        virtual void clear() = 0;
    };
};

#endif // GG_CONSOLE_HPP_INCLUDED
