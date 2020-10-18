q1:

input: two vertices, between which shortest path shoild be found.
output: shortest path.

->i generated a random graph form which i had crated adjacent matrix is formed.  
->First using the given vertices i had detemined the row which i should work on.
->then i shared the matrix using number of rows and total number of processes.
->using them i had tried to distribute the matrix multiplication.
  for eg: for a 5X5 matrix with 2 processores i divided the matrix into 2 parts which contains main matrix 2 columns and 3 columns respectively.
 ->using them i created a res row matrix from which we can get a[vertice1][vertice2] we will check wether its value is not equal to zero if equals to zero then we repeat above two steps untill we get its value not equal to zero.
 -> if the value of a[vertice1][vertice2] is not equal to zero we stop all processes an return the value saying that value as
the shortest distance between vertice1 and vertice2.



q2:
when the information is spread over different process . we will first get to know where are the required row and columns.
for example if we have vertex1 as 2 and vertex2 as 3 we will concentrate on gaining infomation about 2 row of the combined matrix and 3 column of the combined matrix. Because they are the ones that mainly effect the value of  a[vertice1][vertice2].
  
