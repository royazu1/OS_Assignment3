#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char **argv)
{
    char str[12];
    int parent_pid = getpid();
    //printf("my pid=%d\n", parent_pid);
    //printf("str_ptr=%p\n", str);
    uint64 s;
    if (fork() == 0)
    {
        printf("size before map: %l\n", (uint64)sbrk(0));
        uint64 str_offset = map_shared_pages((void *)str, 40, parent_pid, &s);
        char *fml = (char *)str_offset;
        memcpy(fml, "Hello daddy", 12);
        printf("The string of my parent is: %s\n", fml);
        printf("size after map: %l\n", (uint64)sbrk(0));
        if (unmap_shared_pages((void *)str_offset, 4096, &s) == 0)
        {
            printf("Child unmapped succ..\n");
            printf("size after unmap: %l\n", (uint64)sbrk(0));
            void *p = malloc(2048);
            printf("size after malloc() %l\n", (uint64)sbrk(0));
            free(p);
            printf("size after free() %l\n", (uint64)sbrk(0));
        }
        else
        {
            printf("unmap failed!!??\n");
        }
        exit(0);
    }
    else
    {
        sleep(20);
        printf("%s\n", str);
        printf("Parent here: child finished.. exiting\n");
        printf("the string should still be valid here: str=%s\n",str);
        exit(0);
    }
}