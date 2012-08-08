#include <stdio.h>

void print_hash(unsigned char hash[], int length) 
{   
   int idx; 
   for (idx=0; idx < length; idx++) 
      printf("%02x",hash[idx]); 
   printf("\n"); 
}  