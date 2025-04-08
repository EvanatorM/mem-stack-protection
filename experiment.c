#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include<unistd.h>
#include <limits.h>
#include <signal.h>
#include <malloc.h>
#include<string.h>

#define handle_error(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)

static char *buffer;

/* 
Note: calling printf() from a signal handler is not safe
(and should not be done in production programs), since
printf() is not async-signal-safe; see signal-safety(7).
Nevertheless, we use printf() here as a simple way of
showing that the handler was called. 
*/
static void handler(int sig, siginfo_t *si, void *unused){
  printf("Got SIGSEGV at address: 0x%lx\n", (long) si->si_addr);
  exit(EXIT_FAILURE);
}
  
int main(int argc, char *argv[]){
  char *p, *buffer; // * means 'a pointer to'
	char c; //c is a char
  int pagesize;
	int i = 0, size;
  
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = handler;
  
  if(sigaction(SIGSEGV, &sa, NULL) == -1)
    handle_error("sigaction");

  // Initializing Pagesize, most systems its 4096 bytes
  pagesize = sysconf(_SC_PAGE_SIZE);
  
  if(pagesize == -1)
    handle_error("sysconf");
 
  //Allocate a buffer; it will have the default protection of PROT_READ|PROT_WRITE. 
  size = pagesize * 10;
  p = memalign(pagesize, size); //Allocating buffer of size of ten pages
  
  if (p == NULL)
    handle_error("memalign");

  memset(p, 0x00, size); // Zeros
  memset(p, 0x41, size); // 'A'
  
  for(i = 0; i < 10; i++){
    /*
    Printing all pages first bytes from first page. 
    The usage of %d format specifier causes compilation warnings.
    */	
	  printf("Address of %d Page: %lx\n", i + 1 , p + (i * pagesize));	
  }

  // TODO: Start writing code here and can define variables for functions above
  
  // Question 7
  
  // Page 9
  i = pagesize * 8;
  *(p + i) = 'E';
  *(p + (i + 1)) = 'v';
  *(p + (i + 2)) = 'a';
  *(p + (i + 3)) = 'n';

  printf("First 4 bytes of page 9 (%lx): ", p + (pagesize * 8));
  for (i = pagesize * 8; i < (pagesize * 8 + 4); i++) {
    printf("%c", *(p + i));
  }
  printf("\n");

  // Page 10
  i = pagesize * 9;
  *(p + i) = 'E';
  *(p + (i + 1)) = 'v';
  *(p + (i + 2)) = 'a';
  *(p + (i + 3)) = 'n';

  printf("First 4 bytes of page 10 (%lx): ", p + (pagesize * 9));
  for (i = pagesize * 9; i < (pagesize * 9 + 4); i++) {
    printf("%c", *(p + i));
  }
  printf("\n");


  // Question 8
  // Protect 7 and 8
  if (mprotect(p + pagesize * 6, pagesize, PROT_READ | PROT_WRITE) == -1){
    handle_error("mprotect");
  }
  if (mprotect(p + pagesize * 7, pagesize, PROT_READ | PROT_WRITE) == -1){
    handle_error("mprotect");
  }

  i = pagesize * 6;
  *(p + i) = 'M';
  *(p + (i + 1)) = 'i';
  *(p + (i + 2)) = 'l';
  *(p + (i + 3)) = 'l';
  *(p + (i + 4)) = 'e';
  *(p + (i + 5)) = 'r';

  i = pagesize * 7;
  *(p + i) = 'M';
  *(p + (i + 1)) = 'i';
  *(p + (i + 2)) = 'l';
  *(p + (i + 3)) = 'l';
  *(p + (i + 4)) = 'e';
  *(p + (i + 5)) = 'r';

  printf("First 6 bytes of page 7 (%lx): ", p + (pagesize * 6));
  for (i = pagesize * 6; i < (pagesize * 6 + 6); i++) {
    printf("%c", *(p + i));
  }
  printf("\n");

  printf("First 6 bytes of page 8 (%lx): ", p + (pagesize * 7));
  for (i = pagesize * 7; i < (pagesize * 7 + 6); i++) {
    printf("%c", *(p + i));
  }
  printf("\n");


  // Question 9
  // Protect 5 and 6
  if (mprotect(p + pagesize * 4, pagesize, PROT_WRITE) == -1){
    handle_error("mprotect");
  }
  if (mprotect(p + pagesize * 5, pagesize, PROT_WRITE) == -1){
    handle_error("mprotect");
  }

  i = pagesize * 4;
  *(p + i) = 'e';
  *(p + (i + 1)) = 'm';
  *(p + (i + 2)) = 'm';
  *(p + (i + 3)) = '4';
  *(p + (i + 4)) = '5';

  i = pagesize * 5;
  *(p + i) = 'e';
  *(p + (i + 1)) = 'm';
  *(p + (i + 2)) = 'm';
  *(p + (i + 3)) = '4';
  *(p + (i + 4)) = '5';

  printf("First 5 bytes of page 5 (%lx): ", p + (pagesize * 4));
  for (i = pagesize * 4; i < (pagesize * 4 + 5); i++) {
    printf("%c", *(p + i));
  }
  printf("\n");

  printf("First 5 bytes of page 6 (%lx): ", p + (pagesize * 5));
  for (i = pagesize * 5; i < (pagesize * 5 + 5); i++) {
    printf("%c", *(p + i));
  }
  printf("\n");


  // Question 10
  // Copy
  char *buffer2 = memalign(pagesize, pagesize * 2);
  if (buffer2 == NULL)
    handle_error("memalign");

  memcpy(buffer2, p + pagesize * 6, pagesize * 2);

  printf("First 6 bytes of new page 1 (%lx): ", buffer2);
  for (i = 0; i < 6; i++) {
    printf("%c", *(buffer2 + i));
  }
  printf("\n");

  printf("First 6 bytes of new page 2 (%lx): ", (buffer2 + pagesize));
  for (i = pagesize; i < pagesize + 6; i++) {
    printf("%c", *(buffer2 + i));
  }
  printf("\n");


  // Question 11
  // Copy 6
  memcpy(buffer2, p + pagesize * 5, pagesize);
  // Copy 9
  memcpy(buffer2 + pagesize, p + pagesize * 8, pagesize);

  printf("First 6 bytes of new page 1 (%lx): ", buffer2);
  for (i = 0; i < 6; i++) {
    printf("%c", *(buffer2 + i));
  }
  printf("\n");

  printf("First 6 bytes of new page 2 (%lx): ", (buffer2 + pagesize));
  for (i = pagesize; i < pagesize + 6; i++) {
    printf("%c", *(buffer2 + i));
  }
  printf("\n");

  exit(EXIT_SUCCESS);
}