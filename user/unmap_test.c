#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main (int argc, char ** argv) {
    char * str= "fml";
    int parent_pid= getpid();
    printf("my pid=%d\n",parent_pid);
    printf("str_ptr=%p\n",str);

    if (fork() == 0) { 
        printf("size before map: %d\n", memsize());
        uint64 str_offset=map_shared_pages((void*)str, 4096, parent_pid);
        char* fml=(char*) str_offset;
        printf("The string of my parent is: %s\n",fml);
        printf("size after map: %d\n", memsize());
        if ( unmap_shared_pages((void*)str_offset,4096) == 0) {
            printf("Child unmapped succ..\n");
            printf("size after unmap: %d\n", memsize());
        }
        else {
            printf("unmap failed!!??\n");
        }
        exit(0);
    }
    else {
        sleep(10);
        printf("Parent here: child finished.. exiting\n");
        //printf("str=%s\n",str);
        exit(0);
    }
}