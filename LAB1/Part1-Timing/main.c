#include "utility.h"

// TODO: Uncomment the following lines and fill in the correct size
//#define L1_SIZE [TODO]
//#define L2_SIZE [TODO]
//#define L3_SIZE [TODO]
 
int main (int ac, char **av) {
    const int size = 20*1024*1024;
    char *c = (char *)malloc(size);
    for (int i = 0; i < 10; i++) {
	    for (int j = 0;j < size; j++) {
		    c[j] = i*j;
	    }
    }
    // create 4 arrays to store the latency numbers
    // the arrays are initialized to 0
    uint64_t dram_latency[SAMPLES] = {0};
    uint64_t l1_latency[SAMPLES] = {0};
    uint64_t l2_latency[SAMPLES] = {0};
    uint64_t l3_latency[SAMPLES] = {0};

    // A temporary variable we can use to load addresses
    // The volatile keyword tells the compiler to not put this variable into a
    // register- it should always try to load from memory/ cache.
    volatile char tmp;

    // Allocate a buffer of 64 Bytes
    // the size of an unsigned integer (uint64_t) is 8 Bytes
    // Therefore, we request 8 * 8 Bytes
    uint64_t *target_buffer = (uint64_t *)malloc(8*sizeof(uint64_t));
    //uint64_t *eviction_buffer = (uint64_t *)malloc(516*8*sizeof(uint64_t));
    //uint64_t *eviction_buffer = (uint64_t *)malloc(516*8*8*sizeof(uint64_t));
    //printf("l3_addr = %p \n", eviction_buffer);
    if (NULL == target_buffer) {
        perror("Unable to malloc");
        return EXIT_FAILURE;
    }

    // [1.4] TODO: Uncomment the following line to allocate a buffer of a size
    // of your chosing. This will help you measure the latencies at L2 and L3.
    uint64_t *eviction_buffer = (uint64_t *)malloc(516*8*8*sizeof(uint64_t));

    // ======
    // [1.4] TODO: Measure DRAM Latency, store results in dram_latency array
    for (int i = 0; i < SAMPLES; i++) {
	    clflush(target_buffer);

	    dram_latency[i] = measure_one_block_access_time((uint64_t)target_buffer);
	    //l1_latency[i] = measure_one_block_access_time((uint64_t)target_buffer);
    }
    clflush(target_buffer);
    /*int i, j;
    //one_block_access((uint64_t)eviction_buffer);
    clflush(eviction_buffer);
    printf("one block time = %d \n", measure_one_block_access_time((uint64_t)eviction_buffer));
    for (i = 0; i < SAMPLES; i++) {
	for (j = 0; j < 2; j++) {
		/*one_block_access((uint64_t)eviction_buffer+0x8000);
		one_block_access((uint64_t)eviction_buffer+0x18000);
		one_block_access((uint64_t)eviction_buffer+0x10000);
		one_block_access((uint64_t)eviction_buffer+0x30000);
		one_block_access((uint64_t)eviction_buffer+0x38000);
		one_block_access((uint64_t)eviction_buffer+0x28000);
		one_block_access((uint64_t)eviction_buffer+0x20000);
		one_block_access((uint64_t)eviction_buffer+0x40000);
                one_block_access((uint64_t)eviction_buffer+0x50000);
                one_block_access((uint64_t)eviction_buffer+0x58000);
                one_block_access((uint64_t)eviction_buffer+0x48000);
                one_block_access((uint64_t)eviction_buffer+0x68000);
                one_block_access((uint64_t)eviction_buffer+0x70000);
                one_block_access((uint64_t)eviction_buffer+0x60000);
                one_block_access((uint64_t)eviction_buffer+0x80000);
                one_block_access((uint64_t)eviction_buffer+0x78000);

	//l3_latency[i] = measure_one_block_access_time((uint64_t)eviction_buffer);
	        printf("Time 20 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x20000));
                printf("Time 38 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x38000));
                printf("Time 60 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x60000));
                printf("Time 18 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x180000));
                printf("Time 10 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x10000));
                printf("Time 80 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x80000));
                printf("Time 8 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x8000));
                //printf("Time 0 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x0000));
                printf("Time 40 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x40000));
                printf("Time 48 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x48000));
                printf("Time 70 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x70000));
                printf("Time 68 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x68000));
                printf("Time 50 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x50000));
                printf("Time 28 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x28000));
                printf("Time 30 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x30000));
                printf("Time 78 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x78000));
                printf("Time 58 = %lu \n", measure_one_block_access_time((uint64_t)eviction_buffer+0x58000));
		printf ("End of loop %d **********************************************************\n", i);
	}
	l3_latency[i] = measure_one_block_access_time((uint64_t)eviction_buffer);
    }*/
    // ======
    //
    one_block_access((uint64_t)&eviction_buffer[0]);
    //printf("eviction_buffer[0] = %p\n", &eviction_buffer[0]);
    int j, i;
    // [1.4] TODO: Measure L2 Latency, store results in l2_latency array
    for (i = 0; i < SAMPLES; i++) {
            //one_block_access((uint64_t)eviction_buffer);
            //printf ("Target = %p", eviction_buffer);
            for (j = 8; j < (2101*8*8); j+=8) {
            //one_block_access((uint64_t)eviction_buffer);
            one_block_access((uint64_t)&eviction_buffer[j]);
            //printf ("Addr = %p \n", &eviction_buffer[j]);

            }
            //printf("Measuring time for addr = %p\n", &eviction_buffer[0]);
            l3_latency[i] = measure_one_block_access_time((uint64_t)eviction_buffer);

    }



    // ======
    one_block_access((uint64_t)&eviction_buffer[0]);
    //printf("eviction_buffer[0] = %p\n", &eviction_buffer[0]);
    //int j, i;
    // [1.4] TODO: Measure L2 Latency, store results in l2_latency array
    for (i = 0; i < SAMPLES; i++) {
	    //one_block_access((uint64_t)eviction_buffer);
	    //printf ("Target = %p", eviction_buffer);
	    for (j = 8; j < (2101*8); j+=8) {
	    //one_block_access((uint64_t)eviction_buffer);
	    one_block_access((uint64_t)&eviction_buffer[j]);
	    //tmp = eviction_buffer[j];
	    //printf ("Addr = %p \n", &eviction_buffer[j]);
	    	    
	    }
	    //printf("Measuring time for addr = %p\n", &eviction_buffer[0]);
	    l2_latency[i] = measure_one_block_access_time((uint64_t)eviction_buffer);
	  	 
    }
    // ======
    //

    // ======
    // [1.4] TODO: Measure L3 Latency, store results in l3_latency array
    // ======
    //
    // Example: Measure L1 access latency, store results in l1_latency array
    for (int i=0; i<SAMPLES; i++){
        // Step 1: bring the target cache line into L1 by simply accessing the line
        //tmp = target_buffer[0];
	one_block_access((uint64_t)target_buffer);
        // Step 2: measure the access latency
        l1_latency[i] = measure_one_block_access_time((uint64_t)target_buffer);
    }


    // Print the results to the screen
    // [1.5] Change print_results to print_results_for_python so that your code will work
    // with the python plotter software
    print_results(dram_latency, l1_latency, l2_latency, l3_latency);

    free(target_buffer);

    // [1.4] TODO: Uncomment this line once you uncomment the eviction_buffer creation line
    free(eviction_buffer);
    return 0;
}
