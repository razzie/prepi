#ifndef GG_NETMGR_HPP_INCLUDED
#define GG_NETMGR_HPP_INCLUDED

#include <iosfwd>
#include <string>
#include <functional>
#include "gg/refcounted.hpp"
#include "gg/buffer.hpp"
#include "gg/enumerator.hpp"

namespace gg
{
    class application;
    class connection;

    class connection_handler : public reference_counted
    {
    protected:
        virtual ~connection_handler() {}

    public:
        virtual void handle_connection_open(connection*) = 0;
        virtual void handle_connection_close(connection*) = 0;
    };

    class listener : public reference_counted
    {
    protected:
        virtual ~listener() {}

    public:
        virtual uint16_t get_port() const = 0;
        virtual void set_connection_handler(connection_handler*) = 0;
        virtual void set_connection_handler(std::function<void(connection*, bool is_opened)>) = 0;
        virtual connection_handler* get_connection_handler() const = 0;
        virtual enumerator<connection*> get_connections() = 0;
        virtual void send_to_all(buffer*) = 0;
        virtual void send_to_all(uint8_t*, size_t) = 0;
        virtual bool is_opened() const = 0;
        virtual bool open() = 0;
        virtual void close() = 0;
        virtual void set_error_stream(std::ostream&) = 0;
    };

    class packet_handler : public reference_counted
    {
    protected:
        virtual ~packet_handler() {}

    public:
        virtual void handle_packet(connection*) = 0;
    };

    class connection : public reference_counted
    {
    protected:
        virtual ~connection() {}

    public:
        virtual listener* get_listener() = 0;
        virtual buffer* get_input_buffer() = 0;
        virtual buffer* get_output_buffer() = 0;
        virtual std::string get_address() const = 0;
        virtual uint16_t get_port() const = 0;
        virtual void set_packet_handler(packet_handler*) = 0;
        virtual void set_packet_handler(std::function<void(connection*)>) = 0;
        virtual packet_handler* get_packet_handler() const = 0;
        virtual void set_connection_handler(connection_handler*) = 0;
        virtual connection_handler* get_connection_handler() const = 0;
        virtual void send(buffer*) = 0;
        virtual void send(uint8_t*, size_t) = 0;
        virtual bool is_opened() const = 0;
        virtual bool open() = 0;
        virtual void close() = 0;
        virtual void set_error_stream(std::ostream&) = 0;
    };

    class network_manager
    {
    protected:
        virtual ~network_manager() {}

    public:
        virtual application* get_app() const = 0;
        virtual std::string get_hostname() const = 0;
        virtual bool is_big_endian() const = 0;
        virtual bool is_little_endian() const = 0;
        virtual listener* create_tcp_listener(uint16_t port) const = 0;
        virtual listener* create_udp_listener(uint16_t port) const = 0;
        virtual connection* create_tcp_connection(std::string address, uint16_t port) const = 0;
        virtual connection* create_udp_connection(std::string address, uint16_t port) const = 0;

        template<class T, size_t N = sizeof(T)>
        T& swap_byte_order(T& t) const
        {
            uint8_t buf[N];
            size_t i;
            for (i = 0; i < N; ++i) buf[i] = reinterpret_cast<uint8_t*>(&t)[i];
            for (i = N; i > 0; --i) reinterpret_cast<uint8_t*>(&t)[i] = buf[i];
            return t;
        }

        template<class T, size_t N = sizeof(T)>
        T& to_host_byte_order(T& t) const
        {
            if (!is_big_endian())
                return swap_byte_order<T, N>(t);
            else
                return t;
        }

        template<class T, size_t N = sizeof(T)>
        T& to_network_byte_order(T& t) const
        {
            if (!is_big_endian())
                return swap_byte_order<T, N>(t);
            else
                return t;
        }
    };
};

#endif // GG_NETMGR_HPP_INCLUDED
