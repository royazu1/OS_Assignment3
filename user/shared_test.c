
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main (int argc, char ** argv) {
    char str[4]= "fml";
    int parent_pid= getpid();
    printf("my pid=%d\n",parent_pid);
    printf("str_ptr=%p\n",str);

    if (fork() == 0) { 
        uint64 str_offset=map_shared_pages((void*)str, 4096, parent_pid);
        char* fml=(char*) str_offset;
//        fml++;
        //str_offset++;
        //str_offset+=5;
        printf("The string of my parent is: %s\n",fml);
        //fml[1]='Z';
        exit(0);
    }
    else {
        wait(0);
        printf("Parent here: child finished.. exiting\n");
        str[1]='F';
        printf("str=%s\n",str);
        exit(0);
    }
}