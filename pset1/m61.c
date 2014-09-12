#define M61_DISABLE 1
#include "m61.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>


//static struct m61_statistics stats;
static struct m61_statistics gstats = {
    0,0,0,0,0,0,NULL,NULL
};

void* m61_malloc(size_t sz, const char* file, int line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.
    
    //void* result = malloc(sz + sizeof(m61_metadata));
    size_t big_size = (size_t) -1;
    big_size -= sizeof(m61_metadata);
    if (sz > big_size) {
        gstats.nfail++;
        gstats.fail_size += sz;
        return NULL;
    }
    m61_metadata *result = malloc(sz + sizeof(m61_metadata));
    if (result == NULL) {
        gstats.nfail++;
        gstats.fail_size += sz;
        return NULL;
    } else {
        
        
       
        gstats.total_size += sz;
        gstats.ntotal++;
        gstats.nactive++;
        gstats.active_size += sz;
        m61_metadata meta = { sz };
        //result[sz] = meta;
        memcpy(result,&meta,sizeof(m61_metadata));
        char *cresult = (char *) (result + sizeof(m61_metadata));
        char *cmresult = cresult + sz;
        //printf("%p res, %p cres \n\n\n\n\n\n", result, cresult);
        //return result + 1;
         if (gstats.heap_min == NULL && gstats.heap_max == NULL) { 
        //     printf("hi\n\n");
            gstats.heap_min = cresult;
            gstats.heap_max = cmresult;
      //      printf("%p m, %p ma, %p cre \n\n", gstats.heap_min, gstats.heap_max, cresult);
        } else if (gstats.heap_min > cresult) {
            gstats.heap_min = cresult;
        } else if (gstats.heap_max < cmresult) {
            gstats.heap_max = cmresult;
        } else {
    //        printf("y u no call");
        }
        
        return result + sizeof(m61_metadata);  //pointer arithmetic should return the payload
    }
    //if (sz % 4 == 0) {
       // gstats.nactive++;
        //gstats.active_size += sz;
        //return malloc(sz);
   // } else {
     //   gstats.nfail++;
       // gstats.fail_size += sz;
        //return NULL;
    //}
}

void m61_free(void *ptr, const char *file, int line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.
    
    //struct m61_metadata meta;
    //read(ptr,&meta,sizeof(struct m61_metadata));
    //struct m61_metadata *metaptr = (struct m61_metadata *)ptr;
    //struct m61_metadata *actualdata = metaptr + 1;
    //m61_metadata *payload = (m61_metadata *)ptr;
    //m61_metadata *meta = ptr - 1;
    gstats.nactive--;
    m61_metadata *itismeta = ptr;
    //m61_metadata *meta = ptr - 1;
    m61_metadata *meta = itismeta - sizeof(m61_metadata);
    //m61_metadata *meta = ptr - sizeof(m61_metadata);
    
    gstats.active_size -= meta->size;
    //void *actptr = meta;
    //gstats.active_size -= meta->size;
    ///gstats.active_size -= (*metaptr).size; //???
    //free(actualdata);
    free(meta);
    //free(ptr);
}

void* m61_realloc(void* ptr, size_t sz, const char* file, int line) {
    //void* new_ptr = NULL;
    m61_metadata* new_ptr = NULL;
    if (sz)
        new_ptr = m61_malloc(sz, file, line);
    if (ptr && new_ptr) {
        // Copy the data from `ptr` into `new_ptr`.
        // To do that, we must figure out the size of allocation `ptr`.
        // Your code here (to fix test012).
        m61_metadata *itismeta = ptr;
        m61_metadata *meta = itismeta - sizeof(m61_metadata);
        m61_metadata new_meta = { (meta->size) };
        new_ptr -= sizeof(m61_metadata);
        memcpy(new_ptr,&new_meta,sizeof(m61_metadata));
        new_ptr += sizeof(m61_metadata);
    }
    m61_free(ptr, file, line);
    return new_ptr;
}

void* m61_calloc(size_t nmemb, size_t sz, const char* file, int line) {
    // Your code here (to fix test014).
    void* ptr = m61_malloc(nmemb * sz, file, line);
    if (ptr)
        memset(ptr, 0, nmemb * sz);
    return ptr;
}

void m61_getstatistics(struct m61_statistics* stats) {
    // Stub: set all statistics to enormous numbers
    memset(stats, 255, sizeof(struct m61_statistics));
    // Your code here.
    //memset(stats, 255, gstats);
    /*
    stats->nactive = gstats.nactive;
    stats->ntotal = gstats.ntotal;
    stats->nfail = gstats.nfail;
    stats->total_size = gstats.total_size;
    stats->active_size = gstats.active_size;
    stats->fail_size = gstats.fail_size;
    stats->heap_min = gstats.heap_min;
    stats->heap_max = gstats.heap_max;
    */
    *stats = gstats;
}

void m61_printstatistics(void) {
    struct m61_statistics stats;
    m61_getstatistics(&stats);

    printf("malloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("malloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}

void m61_printleakreport(void) {
    // Your code here.
}
