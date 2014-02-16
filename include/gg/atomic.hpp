#ifndef GG_ATOMIC_HPP_INCLUDED
#define GG_ATOMIC_HPP_INCLUDED

namespace gg
{
    template<class T>
    class atomic
    {
        T m_val;

    public:
        atomic(const T& t) : m_val( t ) {}
        atomic(const atomic&) = delete;
        ~atomic() {}

        atomic& operator= (T val)
        {
            __sync_synchronize();
            m_val = val;
            __sync_synchronize();
            return *this;
        }

        operator T()
        {
            __sync_synchronize();
            T tmp = m_val;
            __sync_synchronize();
            return tmp;
        }

        T operator++( ) { return __sync_add_and_fetch(&m_val, (T)1); }
        T operator++ (int) { return __sync_fetch_and_add(&m_val, (T)1); }
        T operator+= (T val) { return __sync_add_and_fetch(&m_val, val); }

        T operator-- () { return __sync_sub_and_fetch(&m_val, (T)1); }
        T operator-- (int) { return __sync_fetch_and_sub(&m_val, (T)1); }
        T operator-= (T val) { return __sync_sub_and_fetch(&m_val, val); }

        T exchange(T old_val, T new_val) { return __sync_val_compare_and_swap(&m_val, old_val, new_val); }
    };
};

#endif // GG_ATOMIC_HPP_INCLUDED
