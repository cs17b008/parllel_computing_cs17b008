#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define T 5

int a[T][T], b[T][T], res[T][T], transpose[T][T];
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
  int flag=0;
  
  
  //no of vertices
  e=5;
  for (int i=0;i<T;i++){
       	for(int j=0;j<T;j++){
        		 res[i][j]=0;
       	}
  }
  
  
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Bcast(&terminal1, 1,MPI_INT,0,MPI_COMM_WORLD);
  
  printf("In rank: %d\n", rank); 
  
  if(a[terminal1][terminal2]!=0){
  	printf("shortest path is possible \n");
  }
  
  else{
	  if(rank==0){
	  
	  
	  	//creating adjoint matrix 
  		RandomGraphs(e);
  		
	  	for (int i = 0; i < T; i++){
			for (int j = 0; j < T; j++) {
			    transpose[i][j] = 0;
			}
		}
		
	  	for (int i = 0; i < T; i++){
			for (int j = 0; j < T; j++) {
			    transpose[j][i] = b[i][j];
			}
		}
		
  		
	  	//dividing the matrix
	  	
	  	int tempT=T;
	  	int tempnp=np-1;
	  	 
	  	 for(int end=1;end<np;end++){
	  	  r=tempT/tempnp;
	  	  printf("%d\n",r);
	  	  MPI_Send(&mark, 1, MPI_INT, end, 1,MPI_COMM_WORLD);
	  	  MPI_Send(&r, 1, MPI_INT, end, 1, MPI_COMM_WORLD);
	  	  MPI_Send(&a[terminal1][0], T, MPI_INT, end, 1, MPI_COMM_WORLD);
	  	  tempT=tempT-r;
	  	  tempnp=tempnp-1;
	  	  printf("%d--%d\n",mark,mark+r-1);
	  	  for(int j= mark; j<=mark+r-1;j++){
	  	   MPI_Send(&transpose[j][0], r*T, MPI_INT, end, 1,MPI_COMM_WORLD);
	  	  }
	  	  
	  	  mark=mark+r;
	  	 }
	  	 
	  	 
	  	/* printf("------*^^++++^^^*-----%d-\n",flag);
	  	 
	  	 while(flag==1){
	  		MPI_Recv(&flag,1, MPI_INT, terminal1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  		MPI_Recv(&res[terminal1][0], T, MPI_INT, terminal1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  		
	  		//dividing the task
	  		for(int i=0;i<T;i++){
	  			 int start=i;
	  			 MPI_Send(&b, T*T, MPI_INT, start, 3, MPI_COMM_WORLD);	
	  			 MPI_Send(&res[terminal1][0], T, MPI_INT, start, 3, MPI_COMM_WORLD);
	  			 
	  		}
	  	 }
	  	 
	  	 
	  	 */
	  
	  }
	  
	  if(rank > 0){
	  
	  	int start=0;
	  	MPI_Recv(&mark, 1, MPI_INT, start, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  	MPI_Recv(&r, 1, MPI_INT, start, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  	printf("%d+-%d\n",r,mark);
	  	MPI_Recv(a, T, MPI_INT, start, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  	MPI_Recv(transpose, r*T, MPI_INT, start, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  	
	 		
	  	int transpose1[T][T];
	  	for (int i = 0; i < T; i++){
			for (int j = 0; j < T; j++) {
			    transpose1[i][j] = 0;
			}
		}
	  	for (int i = 0; i < T; i++){
			for (int j = 0; j < T; j++) {
			    transpose1[j][i] = transpose[i][j];
			}
		}
		for (int i = 0; i < T; i++){
			for (int j = 0; j < T; j++) {
			    printf("%d ",transpose1[i][j]);
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
	  	
	  	
	  	for(int j=0;j<T;j++){
	  		tempa[0][j] =0;
	  		for(int temp=0;temp<T;temp++){
	 			tempa[0][j]=tempa[0][j]+(a[0][temp] * transpose1[temp][j]);
	 		}
	 		printf("%d*\n",tempa[0][j]); 
	 	}
	  	
	  	for(int j=0;j<T;j++){
	  		for(int temp=0;temp<T;temp++){
	 			printf("%d ",tempa[j][temp]);
	 		}
	 		printf("-\n");
	 	}
	 	
	 	int s=0,t=0;
	  	for(int i=0;i<T;i++){
	  		if(i>=mark && i<=mark+r-1){
		  		res[s][i]=tempa[s][t];
		  		t=t+1;
		  		}
			
		}
	  	for(int j=0;j<T;j++){
	  		for(int temp=0;temp<T;temp++){
	 			printf("%d ",res[j][temp]);
	 		}
	 		printf("+\n");
	 	}
	  	
	  	/*//checking
	  	if(terminal1>=mark && terminal1<=mark+r-1){
	  		printf("flag=%d\n",flag);
	  		printf("%d,%d\n",terminal1,terminal2);
	  	  	if(res[terminal1][terminal2]!=0){
	  	  	     flag=0;
	  	  	     printf("shortest path is present\n");
	  	  	     //stop
	  	  	}
	  	  	else{
	  	  	    flag=1;
	  	  	    MPI_Send(&flag,1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	  	  	    MPI_Send(&res[terminal1][0], T, MPI_INT, 0, 2, MPI_COMM_WORLD);  
	  	  	}
	  	}
	  	*/
	  	
	  	/*MPI_Barrier(MPI_COMM_WORLD);
	  	start=0;
	  	MPI_Recv(b, T*T, MPI_INT, start, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  	MPI_Recv(a, r*T, MPI_INT, start, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  	MPI_Barrier(MPI_COMM_WORLD);*/
	  	
	  	
	  	
	  		
	  }
	  
  }
  
  MPI_Finalize();
  
  return 0;
    
}
