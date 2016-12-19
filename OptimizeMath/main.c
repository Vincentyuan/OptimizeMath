//
//  main.c
//  OptimizeMath
//
//  Created by 袁萍 on 2016/12/8.
//  Copyright © 2016年 袁萍. All rights reserved.
//

#include <stdio.h>
#include <string.h>
//optimized project
char buf[1000000];
double * getArrayFromFile(char *);
int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    //read data module
    char * filePath = "math.txt";
    double * originalArray = getArrayFromFile(filePath);
    
    return 0;
}
// the file structure should follow the rule in the document
double * getArrayFromFile(char * filePath){
   // printf("%s\n",filePath);
    
    char path[100]="";
        //printf("%s\n",path);
    strcat(path, "/Users/yuanping/Documents/code/xcode/OptimizeMath/OptimizeMath/");
    strcat(path,filePath);
  //    printf("%s\n",path);
    FILE *fp=fopen(path,"r");
    int numberOfX = 0;
    if(!fp)
    {
        printf("can't open file\n");
        return 0;
    }
    char *p;
    while(!feof(fp))
    {
       //read data from file
        fgets(buf, 1000000, fp);   //读取文件中的一行到buf中
        // strtok函数用于分割buf中的字符串，分割符号都写在第二个参数中
        for (p = strtok(buf, " \t\r\n"); p; p = strtok(NULL, " \t\r\n")) {
            printf("%s\n", p);
        }
    }
    printf("\n");
    fclose(fp);
    
    return 0;
    
}

