#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
int main(){
 int i = 0;

 int BUFSIZE = 1000;
 char* words[20];
 FILE *fp = fopen("math.txt", "r");
 if (fp == 0){
        fprintf(stderr, "Error while opening");
        exit(1);
 }

 words[i] = malloc(BUFSIZE);
  while (fgets(words[i], BUFSIZE, fp)) {
        i++;
        words[i] = malloc(BUFSIZE);
 } 
 printf("Output: \n");
 srand(time(NULL));
 int j = rand()%i;
 int k = (j+1)%i;
 fflush(stdout);
 printf("%d - %s %d -%s", j, words[j], k, words[k]); 

 int x;
 for(x = 0; x<i; x++)
       free(words[x]);
 scanf("%d", x);
 fclose(fp);
 return 0;
}
