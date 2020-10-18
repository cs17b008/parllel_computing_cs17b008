#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define T 5

int a[T][T], b[T][T], res[T][T];
int terminal1=2, terminal2=2;


void RandomGraphs(int e)
{
   int i, j;
   int edge[e][3];
   i = 0;
   
   while(i < e)
   {
	  edge[i][0] = rand()%e+1;
	  edge[i][1] = rand()%e+1;
	  edge[i][2] = rand()%e+1;
	  i++;
   }
   
   for (int i=0;i<e;i++){
       for(int j=0;j<3;j++){
          int a1 =i;
          int b1 =edge[i][j]-1;
          if(a1 != b1){
              a[a1][b1] = 1;
              a[b1][a1] = 1;
              b[a1][b1] = 1;
              b[b1][a1] = 1;
          }else {
              a[a1][b1] = 0;
              a[b1][a1] = 0;
              b[a1][b1] = 0;
              b[b1][a1] = 0;   
          }
       }
   }
}
 


int main(int argc, char **argv)
{

  int rank;
  int np;
  int i;
  int r, mark=0;
  int n, e;
  //no of vertices
  e=5;
  for (int i=0;i<T;i++){
       	for(int j=0;j<T;j++){
        		 res[i][j]=0;
       	}
 }
  
  //creating adjoint matrix 
  	RandomGraphs(e);
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Bcast(&terminal1, 1,MPI_INT,0,MPI_COMM_WORLD);
  
  printf("In rank: %d\n", rank); 
  
  if(rank==0){
  	printf("-------------------------------------\n");
  	for (int i=0;i<e;i++){
       	for(int j=0;j<e;j++){
         		 printf("%d ",a[i][j]);
       	}	
      	printf("\n");
  	}
  	printf("---------------------------------------\n");
  	//dividing the matrix
  	
  	int tempT=T;
  	int tempnp=np-1;
  	 
  	 for(int end=1;end<np;end++){
  	  r=tempT/tempnp;
  	  MPI_Send(&mark, 1, MPI_INT, end, 1,MPI_COMM_WORLD);
  	  MPI_Send(&r, 1, MPI_INT, end, 1, MPI_COMM_WORLD);
  	  MPI_Send(&b, T*T, MPI_INT, end, 1, MPI_COMM_WORLD);
  	  
  	  printf("%d-%d-%d-%d \n", r,tempT,tempnp, mark);
  	  tempT=tempT-r;
  	  tempnp=tempnp-1;
  	  for(int j= mark; j<=mark+r-1;j++){
  	   MPI_Send(&a[j][0], r*T, MPI_INT, end, 1,MPI_COMM_WORLD);
  	  }
  	  
  	  mark=mark+r;
  	  printf("%d\n",mark);
  	 }
  	 
  	 printf("------*^^++++^^^*------\n");
  /*	 for(int i=1;i<np;i++){
  	 	int start=i;
	 	MPI_Recv(&mark, 1, MPI_INT, start, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  	MPI_Recv(&r, 1, MPI_INT, start, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  	   	MPI_Recv(res, T*T, MPI_INT, start, 2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  	 }
  	 printf("------*^^++++^^^*------\n");
  */
  }
  
  if(rank > 0){
  
  	int start=0;
  	MPI_Recv(&mark, 1, MPI_INT, start, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  	MPI_Recv(&r, 1, MPI_INT, start, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  	MPI_Recv(b, T*T, MPI_INT, start, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  	MPI_Recv(a, r*T, MPI_INT, start, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  	
  	printf("--------%d------\n", rank);
  	for (int i=0;i<T;i++){
       	for(int j=0;j<T;j++){
         		 printf("%d ",a[i][j]);
       	}
      	printf("\n");
  	} 
  	printf("------+------\n");
  	for (int k=0;k<T;k++){
       	for(int l=0;l<T;l++){
         		 printf("%d ",b[k][l]);
       	}	
      	printf("\n");
  	}
  	
  	
  	//multiplication
  	int tempa[T][T];
  	for (int i=0;i<T;i++){
       	for(int j=0;j<T;j++){
         		 tempa[i][j]=0;
       	}
  	} 
  	
  	for(int i=0;i<r;i++){
  		for(int j=0;j<T;j++){
  			tempa[i][j] =0;
  			for(int temp=0;temp<T;temp++){
  				tempa[i][j]=tempa[i][j]+(a[i][temp] * b[temp][j]);
  			}
  		}
  	}
  	
  	printf("------***-------\n");
  	for (int i=0;i<T;i++){
       	for(int j=0;j<T;j++){
         		 printf("%d ",tempa[i][j]);
       	}
      	printf("\n");
  	} 
  	printf("------***------\n");
  	printf("---%d--%d--\n",mark,r);
  	int s=0;
  	for(int i=mark;i<=mark+r-1;i++){
	  	for(int j=0;j<T;j++){
	  		res[i][j]=tempa[s][j];
		} 	
	  	s=s+1;	
  	}
  	
  	
  	//checking
  	if(terminal1>=mark && terminal1<=mark+r-1){
  		printf("%d,%d\n",terminal1,terminal2);
  	  	if(res[terminal1][terminal2]!=0){
  	  	     printf("shortest path is present\n");
  	  	}
  	}
  	
  	
  	
  	
  	
  	
  	printf("------*+*-------\n");
  	for (int i=0;i<T;i++){
       	for(int j=0;j<T;j++){
         		 printf("%d ",res[i][j]);
       	}
      	printf("\n");
  	} 
  	printf("------*+*------\n");
  	

  	
  /*	MPI_Send(&mark, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
  	MPI_Send(&r, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
  	 for(int j= mark; j<=mark+r-1;j++){
  	   MPI_Send(&res[j][0], r*T, MPI_INT, 0, 1,MPI_COMM_WORLD);
  	  }
  	
  	printf("------*^^^^^^^*------\n");
  */	
  
  	
  	
  }
  
  
  
  MPI_Finalize();
  
  for (int i=0;i<T;i++){
       	for(int j=0;j<T;j++){
         		 printf("%d ",res[i][j]);
       	}
      	printf("\n");
  	}
  
  
  
  
  return 0;
    
}
