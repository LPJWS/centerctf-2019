#include "antidebug.h"

void sigsegv_handler()
{
    exit(0);
}

void check_loop()
{
    while (1)
    {
        if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) 
            __asm__("jmp 0x0");
        usleep(100);
    }
}

void setup()
{
    signal(SIGSEGV, (__sighandler_t)sigsegv_handler);
    pthread_create(&(tid[0]), NULL, &check_loop, NULL);
}