#ifndef PRIO_ORDER_STRIC_HPP
#define PRIO_ORDER_STRIC_HPP

#include <threadpool/task_adaptors.hpp>

static bool strict_ordering;
inline void set_ordering(bool strict) {

    strict_ordering = strict;
}

class prio_strict_task_func {
public:
    unsigned int m_priority;
    boost::threadpool::task_func m_function;

public:
    typedef void result_type;

public:
    prio_strict_task_func(unsigned int const priority, boost::threadpool::task_func const & function)
        : m_priority(priority), m_function(function)
        {}

    void operator() (void) const {

        if (m_function)
            m_function();
    }

    bool operator< (const prio_strict_task_func& rhs) const {
        return m_priority > rhs.m_priority;
    }
};

template <typename Task = boost::threadpool::prio_task_func>
class prio_scheduler_strict : public boost::threadpool::prio_scheduler<Task> {
public:
    prio_scheduler_strict() : current(1) {}

    void pop() {

        // update where we are in the ordered output
        if (strict_ordering)
            current = boost::threadpool::prio_scheduler<Task>::top().m_priority + 1;

        boost::threadpool::prio_scheduler<Task>::pop();
    }

    bool empty() const {

        return boost::threadpool::prio_scheduler<Task>::empty() ||
               (strict_ordering && boost::threadpool::prio_scheduler<Task>::top().m_priority != current);
    }

private:
    unsigned int current;
};

typedef boost::threadpool::thread_pool<prio_strict_task_func, prio_scheduler_strict,
        boost::threadpool::static_size, boost::threadpool::resize_controller, boost::threadpool::wait_for_all_tasks> prio_strict_pool;

#endif
