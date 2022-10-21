
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>

// TODO: define your own buffer size
#define BUFF_SIZE (1<<21)
//#define BUFF_SIZE [TODO]

int main(int argc, char **argv)
{
	    const int size = 20*1024*1024;
    char *c = (char *)malloc(size);
    for (int i = 0; i < 1000; i++) {
            for (int j = 0;j < size; j++) {
                    c[j] = i*j;
            }
    }

  // Allocate a buffer using huge page
  // See the handout for details about hugepage management
  void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
  
  if (buf == (void*) - 1) {
     perror("mmap() error\n");
     exit(EXIT_FAILURE);
  }
  // The first access to a page triggers overhead associated with
  // page allocation, TLB insertion, etc.
  // Thus, we use a dummy write here to trigger page allocation
  // so later access will not suffer from such overhead.
  *((char *)buf) = 1; // dummy write to trigger page allocation


  // TODO:
  // Put your covert channel setup code here
  // Idea is to setup each bit 0-7 in sets 0-7 and then perform a write based on whether the bit is 1. If 1, a write to the set will evict one of the reciever lines that will then show up with the L3 delay during the probe stage.
  void* bit0 = buf;
  void* bit1 = buf+0x40;
  void* bit2 = buf+0x80;
  void* bit3 = buf+0xc0;
  void* bit4 = buf+0x100;
  void* bit5 = buf+0x140;
  void* bit6 = buf+0x180;
  void* bit7 = buf+0x1c0;

  printf("Please type a message.\n");
  int i = 10000000;
  bool sending = true;
  while (sending) {
      char text_buf[128];
      fgets(text_buf, sizeof(text_buf), stdin);
      while(true) {
      	if (string_to_int(text_buf)) {
	      	*((char *)buf) = 1;
		printf("Time = %d\n", measure_one_block_access_time((uint64_t)buf));
      	}
	sending = false;
      }
      // TODO:
      // Put your covert channel code here
  }

  printf("Sender finished.\n");
  return 0;
}


