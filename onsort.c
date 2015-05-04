#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int main(int argc,char** argv){
	int rank,size;
	int i,j,n;
	/*Everyone*/
	int* A=NULL;		/*Input array*/
	/*Only Master*/
	int* B=NULL;		/*Result Array*/
	int* pos=NULL;

	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	n=3;
	A=(int*)malloc(sizeof(int)*n);
	if(rank==0){
		printf("Enter %d elements :\n",n);
		pos=(int*)malloc(sizeof(int)*n);
		B=(int*)malloc(sizeof(int)*n);
		for(i=0;i<n;i++)
			scanf("%d",&A[i]);
	}

	MPI_Bcast(A,n,MPI_INT,0,MPI_COMM_WORLD);

	int res=0;
	if(A[rank/n]>A[rank%n]){		/*Assuming size==3, A[rank/]n is always==0 and A[rank%n] = {0, 1, 2}. i.e, tests A[0] > A[0, 1, 2]*/
		res=1;						/*Res = 1 if in decreasing order*/
		
	}
	printf("%d calculates [%d][%d]=%d\n",rank,rank/n,rank%n,res);

	if(rank!=0)
		MPI_Send(&res,1,MPI_INT,0,0,MPI_COMM_WORLD);

		if(rank==0)
		{	/*Master only*/
			int* result=(int*)malloc(sizeof(int)*size);
			result[0]=res;				/*res == 0*/
			for(i=1;i<size;i++){
				MPI_Recv(&result[i],1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			}

			for(i=0;i<n;i++){
				pos[i]=0;
				for(j=0;j<n;j++){
					pos[i]+=result[i*3+j];
				}
				printf("Pos[%d]=%d\n",i,pos[i]); 
				B[pos[i]]=A[i];
			}
			printf("Sorted Order : ");
			for(i=0;i<n;i++){
				printf("%d ",B[i]);
			}
			printf("\n");
		}
	MPI_Finalize();
}


