
//void printfBinaryTree(struct node * );
void printfIntArray(int *,int );
void printfDoubleArray(double *,int);


void printfIntArray(int * intArray,int length){
	//printf("%s\n"," the path is :" );
	int i = 0; 
	
	for(;i<length;i++){
		printf("%d",*(intArray+i) );
	}
}
void printfDoubleArray(double * doubleArray,int length){
	//printf("%s\n"," the path is :" );
	int i = 0; 
	
	for(;i<length;i++){
		printf("%f ",*(doubleArray+i) );
	}
	
}

