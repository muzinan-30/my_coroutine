//
// Created by 路仕泽 on 2024/1/18.
//
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>


#include <iostream>
//_STRUCT_UCONTEXT
//{
//	  int                     uc_onstack;
//	  __darwin_sigset_t       uc_sigmask;     /* 上下文使用的信号掩码*/
//    _STRUCT_SIGALTSTACK     uc_stack;       /* 上下文使用的栈空间 */
//    _STRUCT_UCONTEXT        *uc_link;       /* pointer to resuming context */
//    __darwin_size_t         uc_mcsize;      /* size of the machine context passed in */
//    _STRUCT_MCONTEXT        *uc_mcontext;   /* pointer to machine specific context保存的上下文的特定机器表示，包括调用线程的特定寄存器等 */
//    #ifdef _XOPEN_SOURCE
//    _STRUCT_MCONTEXT        __mcontext_data;
//    #endif  /* _XOPEN_SOURCE */
//}
void func1(void* argv){
    puts("1");
    puts("11");
    puts("111");
    puts("1111");
}
void context_test(){
    char stack[1024*128];
    ucontext_t child,main;
    getcontext(&child); //获取当前上下文
    child.uc_stack.ss_sp = stack;//指定栈空间
    child.uc_stack.ss_size = sizeof(stack);//指定栈空间大小
    child.uc_stack.ss_flags = 0;
    child.uc_link = &main;//设置后继上下文
    makecontext(&child,(void(*)(void))func1,0);//修改上下文指向func1函数
    swapcontext(&main,&child);//切换到child上下文，保存当前上下文到main
    puts("main");//如果设置了后继上下文，func1函数指向完后会返回此处
}
int main(int argc, const char **argv) {
    context_test();
    return 0;
}