//
// Created by 路仕泽 on 2024/1/19.
//

#include "Fiber.h"
#include <ucontext.h>

void Fiber::setThis(Fiber *f) {

}

Fiber::Fiber() {
    setThis(this);
    m_state = RUNNING;
    if (getcontext(&m_ctx)) {
        SYlAR_ASSERT2(false, "getcontext");
    }
    ++s_fiber_count;
    m_id = s_fiber_id + 1;
    SYLAR_LOG_DEBUG(g_logger) << "Fiber::Fiber() main id" << m_id;
}

Fiber::Fiber(std::function<void()> cb, size_t stacksize) : m_id(s_fiber_id++), m_cb(cb) {
    ++s_fiber_count;
    m_stacksize = stacksize ? stacksize : g_fiber_stack_size->getValue();
    m_stack = StackAllocator::Alloc(m_stacksize);
    if (getcontext(&m_ctx)) {
        SYlAR_ASSERT2(false, "getcontext");
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;
    makecontext(&m_ctx, &Filber::MainFunc, 0);
    SYLAR_LOG_DEBUG(g_logger) << "Fiber::Fiber()id" << m_id;
}

Fiber::ptr Fiber::GetThis() {

}

Fiber::~Fiber() {

}

//执行环境位于线程主协程中，去激活子协程
void Fiber::resume() {
    SYlAR_ASSERT(m_state != TERM && m_state != RUNNING);
    setThis(this);
    m_state = RUNNING;
    if (swapcontext(&(t_thread_fiber->m_ctx), &m_ctx) {
        SYlAR_ASSERT2(false, "swap context");
    }
}

void Fiber::yieId() {
    SYlAR_ASSERT(m_state == RUNNING || m_state == TERM);
    setThis(t_thread_fiber.get());
    m_state = READY;
    if (swapcontext(&m_ctx, &(t_thread_fiber->m_ctx))) {
        SYlAR_ASSERT2(false, "swap context");
    }
}

void Fiber::MainFunc() {
    Fiber::ptr cur = GetThis();
    SYlAR_ASSERT(cur);
    cur->m_cb();
    cur->m_cb = nullptr;
    cur->m_state = TERM;
    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->yieId(); //协层结束，进行yieId
}

void Fiber::reset(std::function<void()> cd) {
    SYlAR_ASSERT(m_stack);
    SYlAR_ASSERT(m_state == TERM);
    m_cb = cb;
    if (getcontext(&m_ctx)) {
        SYlAR_ASSERT2(false, "get_context");
    }

    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    makecontext(&m_ctx, &Fiber::MainFunc, 0);
    m_state = READY;
}



