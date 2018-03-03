#include<stdio.h>
#include<unistd.h>

int main(void){
    int pid=0;

    printf("PID:");
    scanf("%d",&pid);

    syscall(338,pid);
    execv("/usr/bin/dmesg",NULL);

    return 0;
}
