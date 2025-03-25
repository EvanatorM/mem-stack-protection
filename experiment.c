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

  exit(EXIT_SUCCESS);
}