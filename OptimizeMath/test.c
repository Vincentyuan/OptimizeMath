#include "stdio.h"
#define M 300
#define N 50

void main()
{
 int i,j;
 float a[M][N]={0};
 FILE *fp;
 if((fp=fopen("math.txt","rt"))==NULL)
 {
  printf("cannot open file\n");
  return;
 }
 for(i=0;i<M;i++)
 {
 for(j=0;j<N;j++)
  fscanf(fp,"%f",&a[i][j]);
 fscanf(fp,"\n");
}
 fclose(fp);
 for(i=0;i<M;i++)
 {
 for(j=0;j<N;j++)
     printf("%g ",a[i][j]);
   printf("\n");
}
}
