#include <stdio.h>
#include <stdlib.h>

// int main () {
//    int i, n;
//    time_t t;
   
//    n = 5;
   
//    /* Intializes random number generator */
//    srand((unsigned) time(&t));

//    /* Print 5 random numbers from 0 to 49 */
//    for( i = 0 ; i < n ; i++ ) {
//       printf("%d\n", rand() % 50);


int main() {
   unsigned long millis = 0;

   while(1) {
      millis++;
      printf("millis %lu %lu %d\n", millis, millis/30000, rand() %3);
   }
  
   return 0;
}
