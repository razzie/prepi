#ifndef GG_SERIALIZER_HPP_INCLUDED
#define GG_SERIALIZER_HPP_INCLUDED

#include <cstdint>
#include <cstring>
#include <functional>
#include "gg/var.hpp"
#include "gg/refcounted.hpp"
#include "gg/optional.hpp"
#include "gg/buffer.hpp"
#include "gg/typeinfo.hpp"

namespace gg
{
    class application;

    class serializer
    {
    protected:
        virtual ~serializer() {}

    public:
        typedef std::function<bool(const var&, buffer*, const serializer*)> serializer_func_ex;
        typedef std::function<bool(const var&, buffer*)> serializer_func;
        typedef std::function<optional<var>(buffer*, const serializer*)> deserializer_func_ex;
        typedef std::function<optional<var>(buffer*)> deserializer_func;

        virtual application* get_app() const = 0;
        virtual void add_rule_ex(typeinfo, serializer_func_ex, deserializer_func_ex) = 0;
        virtual void add_rule(typeinfo, serializer_func, deserializer_func) = 0;
        virtual void remove_rule(typeinfo) = 0;
        virtual bool serialize(const var&, buffer*) const = 0;
        virtual optional<var> deserialize(buffer*) const = 0;
        virtual varlist deserialize_all(buffer*) const = 0;

        template<class T>
        bool serialize(const T& t, buffer* buf) const
        {
            var data;
            data.const_reference(t);
            return this->serialize(data, buf);
        }

        template<class T>
        optional<T> deserialize(buffer* buf) const
        {
            optional<var> data = this->deserialize(buf);

            if (!data.is_valid() || data->get_type() != typeid(T))
                return {};
            return
                std::move(data->get<T>());
        }

        template<class T>
        void add_rule_ex(serializer_func_ex s, deserializer_func_ex d)
        {
            this->add_rule_ex(typeid(T), s, d);
        }

        template<class T>
        void add_rule(serializer_func s, deserializer_func d)
        {
            this->add_rule(typeid(T), s, d);
        }

        template<class T>
        void add_trivial_rule()
        {
            serializer_func s = [](const var& v, buffer* buf)->bool
            {
                if (buf == nullptr || v.get_type() != typeid(T))
                    return false;

                buf->push(reinterpret_cast<const uint8_t*>(v.get_ptr<T>()), sizeof(T));
                return true;
            };

            deserializer_func d = [](buffer* buf)->optional<var>
            {
                if (buf == nullptr || (buf->available() < sizeof(T)))
                    return {};

                T t;
                buf->pop(reinterpret_cast<uint8_t*>(&t), sizeof(T));
                return std::move(t);
            };

            this->add_rule(typeid(T), s, d);
        }
    };

    bool serialize_varlist(const var& v, buffer* buf, const serializer* s);
    optional<var> deserialize_varlist(buffer* buf, const serializer* s);

    bool serialize_string(const var& v, buffer* buf);
    optional<var> deserialize_string(buffer* buf);

    bool serialize_float(const var& v, buffer* buf);
    optional<var> deserialize_float(buffer* buf);

    bool serialize_double(const var& v, buffer* buf);
    optional<var> deserialize_double(buffer* buf);
};

#endif // GG_SERIALIZER_HPP_INCLUDED
