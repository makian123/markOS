#pragma once

#include "mutex.hpp"
#include <system/idt/idt.hpp>

#define KERNEL_PID 0
#define STACK_SIZE 2097152

typedef signed long pid_t;
typedef signed long tid_t;

struct process{
    pid_t pid;
    uint64_t pageMap;
};

struct thread{
    bool running;
    int errNo;
    tid_t tid;
    process *proc;
    interrupt_registers gprs;
    uintptr_t stackBottom;
    uintptr_t stackTop;
};

pid_t getPid();
tid_t getTid();
pid_t newProcess();
tid_t newThread(pid_t pid, void *(*start)(void*));

process *getProcessInfo(pid_t pid);
thread *getThreadInfo(pid_t pid, tid_t tid);