#include <stdint.h>

// We want to jump between "tasks"

typedef struct {
    int stack_ptr;
    int pc;
    int lr;
}task_t;

uint8_t fa_stack[1024] = {0};
uint8_t fb_stack[1024] = {0};

task_t fb_task;
task_t fa_task;

uint8_t fa_running=1;

void scheduler() {
    if(fa_running) {
        fa_running=0;

    } else {
        fa_running=1;
    }
}

void fa() {
        int a=0;
        int b=1;
        int c=a+b;
        scheduler();
}

void fb() {
        int a=0;
        int b=1;
        int c=a+b;
        scheduler();
}

int main() {
    fa_task.lr=main;
    fa_task.pc=fa;
    fa_task.stack_ptr=fa_stack;

    fb_task.lr=main;
    fb_task.pc=fb;
    fb_task.stack_ptr=fb_stack;

    while(1) {
        int a=0;
        int b=1;
        int c=a+b;
        fa();
        fb();
    }

}


