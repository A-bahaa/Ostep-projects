#include "thread_helper.h"
#include <fcntl.h>     // open, O_* constants
#include <stdio.h>     // fwrite, fprintf
#include <stdlib.h>    // exit, malloc
#include <sys/fcntl.h>
#include <sys/mman.h> // mmap, munmap  "in unix systems"
#include <sys/stat.h> // fstat, mode constants
#include <sys/types.h>
#include <unistd.h> // sysconf, close



typedef struct result {  // compressed output 
  struct result *next;   //pointer to the struct 
  int count;             
  char character;        
 
} Result;


typedef struct work {  //array of chunks (not compressed)
  long long chunk_size;  //size of each chunk 
  char *addr;   //mapping address (file num + page num)
  Result *results;
} Work;


typedef struct files {
  int fd;     // file descriptor, it shall not be shared with any other process
  off_t size;   //This is a signed integer type used to represent file sizes.
} Files;

static long long use_ptr = 0, fill_ptr = 0, chunks = 0;
//chunks -> number of buffers in the array
//chunks = file size / page size
static sem_t mutex, empty, full;
//empty and full indicates when a buffer entry has been empited or filled

//The <semaphore.h> header shall define the sem_t type, used in performing semaphore operations
//Semaphores are integer variables that are used to solve the critical section problem
//by using two atomic operations, wait and signal that are used for process synchronization

//mutex is to avoid deadlocks and race conditions
//without mutex, the data in the buffer may be overwritten 

static Result *create_result(int count, char character) {
  Result *result = malloc(sizeof(Result));  //result point to free space in heap in size of Result struct
  if (result == NULL)                       //if the size of the struct is zero
    handle_error("malloc");                 //error msg
  result->count = count;                    
  result->character = character;
  result->next = NULL;
  return result;
}


static void *compress(void *arg) {
  Work *works = (Work *)arg;

  while (1) {

    Sem_wait(&full);   //if full>=1 , sen_wait returns right away and decriment it
                       //else it will suspend execution waiting for a subsequent post 
    Sem_wait(&mutex);

    // between wait and post there is a critical section

    // get buffer
    Work *current_work = &works[use_ptr];  
    use_ptr = (use_ptr + 1) % chunks;  //circular queue
    
    // do work (compress code)
    Result *head = NULL;
    Result *previous_result = NULL;
    char previous_character = '\0';
    int previous_count = 0;

    for (long long i = 0; i < current_work->chunk_size; i++) { //loop on each chunk size
      char character = current_work->addr[i];                  
      if (character == previous_character) {   
        previous_count++;
      } 
      else {
        if (previous_count != 0) {
          Result *last_result = create_result(previous_count, previous_character);  //go create the result and point to it with last result
          
          if (previous_result != NULL) 
            previous_result->next = last_result;
          previous_result = last_result;
          if (head == NULL)
            head = previous_result;
        }
        previous_count = 1;  //if prev_count == 0 then the char appears only once
        previous_character = character;

      }
    }

    if (head == NULL) {
      // same characters
      current_work->results = create_result(previous_count, previous_character);
    } else {
      // list of results
      current_work->results = head;
      previous_result->next = create_result(previous_count, previous_character);
    }

    //end of the critical section
    Sem_post(&mutex); 
    Sem_post(&empty); 
  }
}


static void writeFile(int character_count, char *oldBuff) {

  fwrite(&character_count, sizeof(int), 1, stdout);
          //(pointer to the array of elements to be written, size in bytes of each element, number of elements, pointer to a file of the output stream)
  fwrite(oldBuff, sizeof(char), 1, stdout);

}
          //argc == arg count -> no. of args passed to the program (files) + 1 (for program name)
          //argv[] == arg vector -> pointer array that point to each argument passed to the program
int main(int argc, char *argv[]) {
  
  /* time testing
  double time_spent = 0.0;
  clock_t begin = clock();
  */
  
  long page_size = sysconf(_SC_PAGE_SIZE);  //get page size of specific address  (4096)

  if (argc <= 1) {  //no files are passed
    fprintf(stdout, "pzip: file1 [file2 ...]\n");
    exit(EXIT_FAILURE);
  }

  // get_nprocs is GNU extension
  long np = sysconf(_SC_NPROCESSORS_ONLN);  //get the number of threads
  pthread_t *threads = malloc(sizeof(pthread_t) * (unsigned long)np); //reserve space in heap for all threads
  if (threads == NULL) //if there is no threads
    handle_error("malloc");

  Files *files = malloc(sizeof(Files) * (unsigned long)(argc - 1));  //reserve space in heap for all files
  if (files == NULL)  //if there is no files
    handle_error("malloc");


  // producer
  // count chunks number
  for (int i = 1; i < argc; i++) {
    int fd = open(argv[i], O_RDONLY); //open for reading only
    struct stat sb; //struct that is defined to store information about files 
    if (fd == -1)
      handle_error("open");

    if (fstat(fd, &sb) == -1) //at success it returns 0 else -1
      handle_error("stat");

    //update files struct with the values of this file
    files[i - 1].fd = fd;
    files[i - 1].size = sb.st_size; 

    chunks += (sb.st_size +page_size -1) / page_size ;  //st_size gives the size of the file
  }

  // initializing semaphores
  Sem_init(&mutex, 0, 1);
  // set empty to 1 to prevent main thread cancel workers before they do the work.
  // 0 arg in the middle indicates that the semaphore is shared between threads in the same process
  Sem_init(&empty, 0, 1);
  Sem_init(&full, 0, 0);


  Work *works = malloc(sizeof(Work) * (unsigned long)chunks);  //reserve space in the heap for the buffers
  if (works == NULL)
    handle_error("malloc");

  // create worker threads (consumer)
  for (long i = 0; i < np; i++) //loop on each thread to create work object for each 
    Pthread_create(&threads[i], NULL, compress, works);
                  //pointer to pthread, args (default == null), the function, args will be passed to the function)

  // create jobs 
  //put data into the memory
  //(producer)
  for (int i = 0; i < argc - 1; i++) { //loop on files
    long long offset = 0;     
    //offest must be multible of page size to make it fast
    while (offset < files[i].size) { //loop inside each file to give it bunch of pages in memory
      
      Sem_wait(&empty);
      Sem_wait(&mutex);
      //critical section
      works[fill_ptr].chunk_size = page_size;  //update the size of the buffer with the page size when it is full
      if (offset + page_size > files[i].size)
        works[fill_ptr].chunk_size = files[i].size - offset;  //update the size of the buffer with rest size of the file

      char *addr = mmap(NULL, (size_t)works[fill_ptr].chunk_size, PROT_READ, MAP_PRIVATE, files[i].fd, offset);    //mmap maps files into memory
                        //*addr == NULL atrt address in VA space "indicates to page aligned address which to create the mapping"
                        //number of bytes to be mapped 
                        //prot: page may be read (desired memory protection of the mapping)
                        //flag: create a private copy on write mapping (updates to the mapping is not visible to other processes mapping the same file)
                        //the file to be mapped
                        //offset -> the contents of file mapping are intitialized using length bytes starting at offset 

      if (addr == MAP_FAILED)   //hadle the error if mapping failed
        handle_error("mmap");

      works[fill_ptr].addr = addr; //update the address of the buffer after mapping
      //works[fill_ptr].results = NULL; //no results because we are updating the memory not the output
      offset += page_size;
      fill_ptr = (fill_ptr + 1) % chunks; 

      Sem_post(&mutex);
      Sem_post(&full);
    }
    close(files[i].fd);
  }

  // check jobs are done
  Sem_wait(&empty);
  Sem_wait(&mutex);
  // critical section
  // kill and wait workers
  for (long i = 0; i < np; i++) {  //np : number of threads
    Pthread_cancel(threads[i]);
    Pthread_join(threads[i], NULL); //wait the thread to cancel
    
  }

  // final compress
  // take the final result and write it to file
  // consumer
  int last_count = 0;
  char last_character = '\0';
  for (long long i = 0; i < chunks; i++) {
    Result *result;
    result = works[i].results; 
    while (result != NULL) {   //first chunk : works[0].results == [2a,1b]->null  / sec chunk : works[1].results == [1b,2a]->null
      if (result == works[i].results && result->next != NULL) { // first but not last result
        if (result->character == last_character) { 
          writeFile(result->count + last_count, &result->character);
                    // (count, buffer)
        } else { // if the next element is not equal to the element before
          if (last_count > 0)
            writeFile(last_count, &last_character); 

          writeFile(result->count, &result->character);
        }
      } else if (result->next == NULL) {  // last result
        if (result != works[i].results) { // not first result in chunk
          if (i == chunks - 1) {          // last chunk
            writeFile(result->count, &result->character);
          } else { // not last chunk
            last_character = result->character;
            last_count = result->count;
          }
        } else {                                     // first result
          if (result->character == last_character) { // same
            if (i != chunks - 1) {                   // not last chunk
              last_count += result->count;
            } else {
              writeFile(result->count + last_count, &result->character);
            }
          } else { // not same
            if (last_count > 0) 
              writeFile(last_count, &last_character);
              
            if (i != chunks - 1) {  //not the last chunk
              last_character = result->character;
              last_count = result->count;
            } else {
              writeFile(result->count, &result->character);
            }
          }
        }
      } else { 
        writeFile(result->count, &result->character);
      }

      Result *tmp = result;
      result = result->next; //go to the next result
      free(tmp);  //after writing to the file , free it from the result struct
    }
    if (munmap(works[i].addr, (size_t)works[i].chunk_size) != 0) //unmapping
      handle_error("munmap");
  }
  Sem_post(&mutex);

//free the heap and destroy the semaphores
  free(threads);
  free(files);
  free(works);
  Sem_destroy(&mutex);
  Sem_destroy(&full);
  Sem_destroy(&empty);
  
  
  /* time testing
  clock_t end = clock();
  time_spent += (double)(end-begin) / CLOCKS_PER_SEC;
  printf("time is %f seconds", time_spent);
  */

  return 0;
}
