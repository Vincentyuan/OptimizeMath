int M = 100;
int N = 100;
int realRows;
int realColumns;

double initialValueForArray = -300.99;

void  getOriginalData(char* filePath,double (* dataArray)[N]){
	double *  data[100];
	char * line  = NULL;
	FILE * fp;
	size_t len = 0;
	ssize_t read;
	int row = 0;
	if((fp=fopen(filePath,"r"))==NULL){
		printf("can't open file\n");
	}else{
		while((read = getline(&line,&len,fp))!=-1){
		//	printf("%s",*(line+3)-*(line));
		//	str_split(line," ");
		//	printf("%s",line);
		//	printf("%d",len);
		//	printf("-%s-", (line+9));
			saveToArray(line,dataArray,row,len);
			row++;				
		}
	}
	fclose(fp);
	
}
void  saveToArray(char * charArray, double (*dataArray)[N],int row,int len){
	//get two space and splite get the value.
	int dataIndex = 0;
	int startIndex = 0;
	int endIndex = 0;
	int index = 0;
	
	int i =0;

	while(index<len)
	{

        if(*(charArray+index)!= ' '&&*(charArray+index)!='\n'&&*(charArray+index)!='.')
        {
			index++;
		}else{
			endIndex = index;
			//get each item and then multiply;
			int length = endIndex - startIndex;
			double number = 0;//stored a number 
//			printf("start index is %d, and the end index is %d the current index is %d\n",startIndex,endIndex,index);
			while(startIndex < endIndex){
				number += atof((charArray+startIndex))*pow(10,length-1);
				startIndex++;
				length -- ;				
			}
			startIndex++;//ensure that the start point to the non null field
			*(dataArray[row]+dataIndex++)= number;
            printf("number : %f data at array row :%d, index :%d value :%F\n",number,row,dataIndex-1,*(dataArray[row]+dataIndex-1));
			index++;
		}
	}
   
}
void printfAllDataInOriginal(double **afterData ,double  (*original) [N]){
	int i=0,j=0;
	int row = getRowNumber(original),column = getColumnNumber(original);
	for( i=0;i<row;i++){
		printf("the row %d",i);
		for(j=0;j<column;j++){
			printf(" %f ",*(afterData[i]+j));//*(afterData[i]+j));				
		}
		printf("\n");
	}
}
double ** getReadedData(double (*original) [N]){
	
	int rowNumber = getRowNumber(original);

	int columnNumber = getColumnNumber(original);

	double ** returnData = malloc((N+M)*sizeof(double));
	//returnData = nprintf("here");

	int i =0, j=0;
	double * rows;
	for( i = 0; i<rowNumber;i++){
		//double rows[N] ;
		rows = malloc(N*sizeof(double));
		//printf("row number  :%d columns number : %d\n",rowNumber,columnNumber);
		//printf("rows is :%f returnData : %f\n",rows,*(returnData+i) );
		//printf("\n%s %f \n","hi" ,*(rows+i));
		*(returnData+i) = rows; //new double [columnNumber];

	}
	
	for (i = 0; i < rowNumber; ++i)
	{
		for (j = 0; j < columnNumber; ++j)
		{
			returnData[i][j] = *(original[i]+j);
		}
	}
	return returnData;
}
int getRowNumber(double (*original)[N]){
	//get row number
	int rowNumber = 0;
	
	while(*(original[rowNumber]) != initialValueForArray && (*(original[rowNumber])!=0.0)){
		rowNumber++;

	}
	/*
	for (int i = 0; i < N; ++i)
	{
		if(*(original[rowNumber+i]) != initialValueForArray && (*(original[rowNumber+i])!=0.0)){
			rowNumber++;
			printf("%d\n",rowNumber);
		}
	}
	*/

	
	return rowNumber;
}
int getColumnNumber(double (*original)[N]){
	//get max columns number for each row
	int columnNumber = 0;
	
	while(*(original[0]+columnNumber) != initialValueForArray && (*(original[0]+columnNumber)!=0.0)){
		columnNumber++;
	}

	return columnNumber;
}
