#include <sys/ptrace.h>
#include <pthread.h>
#include <unistd.h>
#include "signal.h"

pthread_t tid[1];

void sigsegv_handler();
void check_loop();
void setup();