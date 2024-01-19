//
// Created by 路仕泽 on 2024/1/19.
//

#ifndef MY_COROUTINE_FIBER_H
#define MY_COROUTINE_FIBER_H

#include <iostream>
#include <ucontext.h>
#include <util.h>
class Fiber {
public:
    typedef std::shared_ptr<Fiber> ptr;
    enum State {
        READY,
        RUNNING,
        TERM
    };
private:
    Fiber(); //只能通过GetThis方法调用
public:
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool run_in_schedular = true);

    ~Fiber();

    void reset(std::function<void()> cd);

    void resume();

    void yieId();

    uint64_t getId() const { return m_id; };

    State getState() const { return m_state; }

public:
    static void setThis(Fiber *f);

    static Fiber::ptr GetThis() {}

    static uint64_t getFibers();

    static void MainFunc();

    static uint64_t GetFiberId();

private:
    u_int64_t m_id = 0;
    uint32_t m_stacksize = 0;
    State m_state = READY;
    ucontext_t m_ctx;
    void *m_stack = nullptr;
    std::function<void()> m_cb;
    bool m_runInScheduler;
};


#endif //MY_COROUTINE_FIBER_H
