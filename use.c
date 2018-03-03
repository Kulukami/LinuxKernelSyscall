#include<stdio.h>
#include<unistd.h>

int main(void){
    int pid=0;                      // init int pid=0

    printf("PID:");                 // input banner PID:
    scanf("%d",&pid);               // input PID

    syscall(338,pid);               // use sys_my_syscall
    execv("/usr/bin/dmesg",NULL);   // run dmesg and see result

    return 0;
}
