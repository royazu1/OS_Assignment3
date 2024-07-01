#include "kernel/types.h"
#include "user/user.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "kernel/fcntl.h"

#include "kernel/crypto.h"
#define SRV_PID 2

int main(void) {
  if(open("console", O_RDWR) < 0){
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  printf("crypto_srv: starting\n");

  if (getpid() != SRV_PID) {
    printf("Server started from user, exiting...\n");
    exit(0);
  }
  else {
    printf("srv with pid 2!\n");
  }

  struct crypto_op * op;
  uint64 size=0;

  // TODO: implement the cryptographic server here
    loop:
    while(1) {
        printf("srv_proc: take shd_req\n");
        if (take_shared_memory_request( (void**)&op, &size) == 0) { // operation removed from Q, it's addr is in op , its size in size.
          if (op->type > CRYPTO_OP_TYPE_DECRYPT || op->state != CRYPTO_OP_STATE_INIT) { //if type isn't dec/enc or state isn't the initial 
              op->state= CRYPTO_OP_STATE_ERROR;
              goto loop;
          }
           uint64 data_size= op->data_size;
           uint64 key_size= op->key_size;
           uchar * data= op->payload+ key_size;
           uchar * key= op->payload;
           for (int i=0; i < data_size;i++) {
              data[i]=data[i]^key[i % key_size]; //byte-wise XOR 
           }
           
          asm volatile ("fence rw,rw" : : : "memory"); // do the writes of the encrypt/decrypt before the STATE_DONE write


           op->state=CRYPTO_OP_STATE_DONE;//set the state to DONE

           if (remove_shared_memory_request(op,size) == 0) {
              printf("removed the op successfully!\n");
           }
           else {
              printf("couldn't remove the op shared map\n");
           }
        }
        else { //??

        }
        
    }

  exit(0);
}
