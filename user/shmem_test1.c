
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char **argv)
{
    char str[] = "Hello child";
    int parent_pid = getpid();
    printf("my pid=%d\n", parent_pid);
    printf("str_ptr=%p\n", str);
    uint64 size;
    if (fork() == 0)
    {
        uint64 str_offset = map_shared_pages((void *)str, 4096 / 2, parent_pid, &size);
        char *fml = (char *)str_offset;
        printf("The string of my parent is: %s\n", fml);
        // printf("size of the memory %l\n", size);
        //  fml[1]='Z';
        exit(0);
    }
    else
    {
        wait(0);
        printf("Parent here: child finished.. exiting\n");
        // str[1] = 'F';
        printf("str=%s\n", str);
        exit(0);
    }
}