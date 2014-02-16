#ifndef GG_TASKMGR_HPP_INCLUDED
#define GG_TASKMGR_HPP_INCLUDED

#include <string>
#include <list>
#include <functional>
#include "gg/refcounted.hpp"

namespace gg
{
    class application;

    class task : public reference_counted
    {
        std::string m_name;
        std::list<task*> m_children;

    protected:
        virtual ~task() {}

    public:
        task() : m_name("unknown") {}
        task(std::string name) : m_name(name) {}

        void rename(std::string name) { m_name = name; }
        void add_child(task* t) { t->grab(); m_children.push_back(t); }
        const std::list<task*>& get_children() const { return m_children; }

        virtual std::string get_name() const { return std::string("unknown"); }
        virtual bool run(uint32_t elapsed) = 0; // returns true if task is finished
    };

    class thread
    {
    protected:
        virtual ~thread() {}

    public:
        virtual std::string get_name() const = 0;
        virtual void add_task(task*) = 0;
        virtual void add_task(std::function<void()> func) = 0;
        virtual void add_delayed_task(task*, uint32_t delay_ms) = 0;
        virtual void add_delayed_task(std::function<void()> func, uint32_t delay_ms) = 0;
        virtual void add_persistent_task(std::function<bool(uint32_t)> func) = 0;
        virtual void add_delayed_persistent_task(std::function<bool(uint32_t)> func, uint32_t delay_ms) = 0;
        virtual void suspend() = 0;
        virtual void resume() = 0;
    };

    class task_manager
    {
    protected:
        virtual ~task_manager() {}

    public:
        static thread* get_current_thread();

        virtual application* get_app() const = 0;
        virtual thread* create_thread(std::string name) = 0;
        virtual thread* get_thread(std::string name) = 0;
        virtual void async_invoke(std::function<void()> func) const = 0;
        virtual task* create_task(std::function<void()> func) const = 0;
        virtual task* create_wait_task(uint32_t wait_ms) const = 0;
        virtual task* create_persistent_task(std::function<bool(uint32_t)> func) const = 0;
    };
};

#endif // GG_TASKMGR_HPP_INCLUDED
