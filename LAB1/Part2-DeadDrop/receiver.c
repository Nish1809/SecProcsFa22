
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
#define BUFF_SIZE (1<<21)
int main(int argc, char **argv)
{
    const int size = 20*1024*1024;
    char *c = (char *)malloc(size);
    for (int i = 0; i < 1000; i++) {
            for (int j = 0;j < size; j++) {
                    c[j] = i*j;
            }
    }
    
	// Put your covert channel setup code here
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
  *((uint64_t *)buf) = 1; // dummy write to trigger page allocation
  //Reciever sets up 16 lines from set0-7 by changing bits 14-6 from 0 to 7, This way when the sender writes to a set, it will evict a line from a set n of L1 which in turn evicts a line from set n of L2 and pushes it to L3, the probe stage will access the 16 lines and hope to see a delay
  void* bit0 = buf;
  int i = 1000;
  while (i--) {
	*((char *)buf) = 1;
	*((char *)buf+0x10000) = 1;
	*((char *)buf+0x8000) = 1;
	*((char *)buf+0x28000) = 1;
	*((char *)buf+0x18000) = 1;
	*((char *)buf+0x40000) = 1;
	*((char *)buf+0x78000) = 1;
	*((char *)buf+0x80000) = 1;
        *((char *)buf+0x20000) = 1;
        *((char *)buf+0x48000) = 1;
        *((char *)buf+0x60000) = 1;
        *((char *)buf+0x88000) = 1;
        *((char *)buf+0x70000) = 1;
        *((char *)buf+0x50000) = 1;
        *((char *)buf+0x68000) = 1;
        *((char *)buf+0x58000) = 1;
  }

	i = 1000;

	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	bool listening = true;
	while (i--) {
		printf (" Time 68 = %d\n", measure_one_block_access_time((uint64_t)(buf+0x68000)));
		printf (" Time 20 = %d\n", measure_one_block_access_time((uint64_t)buf+0x20000));
		printf (" Time 50 = %d\n", measure_one_block_access_time((uint64_t)buf+0x50000));
		printf (" Time 58 = %d\n", measure_one_block_access_time((uint64_t)buf+0x58000));
		printf (" Time 48 = %d\n", measure_one_block_access_time((uint64_t)buf+0x48000));
		printf (" Time 88 = %d\n", measure_one_block_access_time((uint64_t)buf+0x88000));
		printf (" Time 60 = %d\n", measure_one_block_access_time((uint64_t)buf+0x60000));
		printf (" Time 70 = %d\n", measure_one_block_access_time((uint64_t)buf+0x70000));
		printf (" Time 80 = %d\n", measure_one_block_access_time((uint64_t)buf+0x80000));
		printf (" Time 78 = %d\n", measure_one_block_access_time((uint64_t)buf+0x78000));
		printf (" Time 8 = %d\n", measure_one_block_access_time((uint64_t)buf+0x8000));
		printf (" Time 0 = %d for %p\n", measure_one_block_access_time((uint64_t)buf+0x000), buf);
		printf (" Time 40 = %d\n", measure_one_block_access_time((uint64_t)buf+0x40000));
		printf (" Time 28 = %d\n", measure_one_block_access_time((uint64_t)buf+0x28000));
		printf (" Time 18 = %d\n", measure_one_block_access_time((uint64_t)buf+0x18000));
		printf (" Time 10 = %d\n", measure_one_block_access_time((uint64_t)buf+0x10000));
		// Put your covert channel cod\ne here

	}

	printf("Receiver finished.\n");

	return 0;
}


