#ifndef GG_TASKMGR_HPP_INCLUDED
#define GG_TASKMGR_HPP_INCLUDED

#include <cstdint>
#include <string>
#include <list>
#include <functional>
#include "gg/refcounted.hpp"
#include "gg/enumerator.hpp"

namespace gg
{
    class application;

    class mutex : public reference_counted
    {
    protected:
        virtual ~mutex() {}

    public:
        class guard
        {
            mutex* m_mutex;

        public:
            guard(mutex*);
            guard(guard&&);
            ~guard();
        };

        virtual void lock() = 0;
        virtual void unlock() = 0;
        guard&& get_guard();
    };

    class condition : public reference_counted
    {
    protected:
        virtual ~condition() {}

    public:
        virtual void wait() = 0;
        virtual void wait(uint32_t timeout_ms) = 0;
        virtual void trigger() = 0;
    };

    class task : public reference_counted
    {
        std::string m_name;
        std::list<grab_ptr<task*, true>> m_children;

    protected:
        virtual ~task() {}

    public:
        task();
        task(std::string name);
        void rename(std::string name);
        void add_child(task* t);
        enumerator<task*> get_children();
        virtual std::string get_name() const;
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
        virtual mutex* create_mutex() const = 0;
        virtual mutex* create_recursive_mutex() const = 0;
        virtual condition* create_condition() const = 0;
    };
};

#endif // GG_TASKMGR_HPP_INCLUDED
