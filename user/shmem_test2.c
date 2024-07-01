#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char **argv)
{
    char str[12];
    int parent_pid = getpid();
    printf("my pid=%d\n", parent_pid);
    printf("str_ptr=%p\n", str);
    uint64 s;
    if (fork() == 0)
    {
        printf("size before map: %d\n", memsize());
        uint64 str_offset = map_shared_pages((void *)str, 4096, parent_pid, &s);
        char *fml = (char *)str_offset;
        memcpy(fml, "Hello daddy", 12);
        printf("The string of my parent is: %s\n", fml);
        printf("size after map: %l\n", s);
        if (unmap_shared_pages((void *)str_offset, 4096, &s) == 0)
        {
            printf("Child unmapped succ..\n");
            printf("size after unmap: %l\n", s);
            void *p = malloc(2048);
            printf("size after malloc %d", memsize());
            free(p);
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
        // printf("str=%s\n",str);
        exit(0);
    }
}