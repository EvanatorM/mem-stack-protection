#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]){
	printf("Vulnerable\n");

  char buffer[24];
	scanf("%s", buffer);

	if(!strcmp(buffer, "CIT368"))
		printf("Password is Correct\n");
	else
		printf("Invalid Pass, Try Again!\n");
  
  return 0;
}