//Name : Abhishek Jain
//ECE 8893
//Assignment 1

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <signal.h>
#include <math.h>
#include <mpi.h>

#include "Complex.h"
#include "InputImage.h"

using namespace std;

int main(int argc,char**argv)
{
string f2("Tower.txt");
const char* fn = f2.c_str();
//const char*fn("Tower.txt");// =fn;
InputImage image=InputImage(fn);  
int rc,numtasks, rank,source=0, tag=1,k=0,l=0;


MPI_Status stat;

//MPI_Datatype rowtype; 
//MPI_Datatype columntype;
 rc = MPI_Init(&argc,&argv);
 if (rc != MPI_SUCCESS) {
 printf ("Error starting MPI program. Terminating.\n");
 MPI_Abort(MPI_COMM_WORLD, rc);
 }
 else
 {MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
 MPI_Comm_rank(MPI_COMM_WORLD,&rank);
 printf ("Number of tasks= %d My rank= %d\n", numtasks,rank);     
   }

  Complex* img = image.GetImageData();

       Complex**store;
        int w = image.GetWidth();
   int h = image.GetHeight();
 int size=w*h;

Complex**temp;
temp=new Complex*[size];

 
   store = new Complex*[size];
for(int i=0;i<w;i++)
{ store[i]=new Complex[w]; 
temp[i]=new Complex[w];
} 
//Complex* copy2=new Complex[size]; 
Complex* copy= new Complex[size];
Complex* add=new Complex[size];
Complex* temp2=new Complex[size];
Complex *copy2=new Complex[size];
Complex inverse = Complex((w),0);
// MPI_Type_contiguous(size, MPI_INT, &rowtype);
// MPI_Type_commit(&rowtype);
// MPI_Type_vector(size, 1, size, MPI_INT, &columntype);
// MPI_Type_commit(&columntype);
//
 int each=size/numtasks;


/* if(rank==0)
  {
  for(int i=0;i<2;i++)
 {  for(int j=0;j<2;j++,k++)
 {
  store[i][j]=img[k];
  }
 cout<<endl;
 }
   for(int i=0;i<2;i++)
  {
 for(int j=0;j<2;j++)
  {       store[i][j].Print();
  }
   cout<<endl;

 }*/
  if(rank==0)
 {//img=img+0;
    for(int i=0;i<numtasks;i++)
 {rc= MPI_Send(img,each*sizeof(Complex) ,MPI_CHAR, i, 0, MPI_COMM_WORLD);

 if (rc != MPI_SUCCESS)
 {
 cout << "Rank " << rank << " send failed, rc " << rc << endl;                  
 MPI_Finalize();
 // exit(1);
                                                                                }
 img=img+each;                                                               
}
} 
//if(rank!=0)
//{
rc= MPI_Recv(copy, each*sizeof(Complex), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat);
 if (rc != MPI_SUCCESS)
 {                                                                               cout << "Rank " << rank << " recv failed, rc " << rc << endl;
 MPI_Finalize();
//exit(1):

}
/*cout<<"Message from rank" <<rank<<endl;
for(int i=0;i<8;i++)
{
copy[i].Print();

}*/


for(int i=0;i<size;i++)
{
add[i]=Complex(cos(0),sin(0))-Complex(cos(0),sin(0));
}

for(int j=0;j<(h/numtasks);j++)
{
for(int n=(j*w);n<((j+1)*w);n++)
{//add[n]=Complex(cos(0),sin(0))-Complex(cos(0),sin(0));
for(int k=0;k<w;k++)

{//Complex temp=Complex(cos((((2*M_PI)*n*k)/w)),(((sin((((-2*M_PI)*n*k)/w))))));
//temp=temp.Mag();
add[n]=add[n]+(copy[k+(j*w)]*Complex(cos((((2*M_PI)*n*k)/w)),(((sin((((-2*M_PI)*n*k)/w)))))));
//add[n]=add[n].Mag();
}
//add[n]=add[n].Mag();
//add[i]=Complex(cos(0),sin(0))-Complex(cos(0),sin(0));
}
}

/*if(rank==15)
{
for(int i=(3840);i<4096;i++)
{//add[i]=add[i].Mag();
add[i].Print();
cout<<" ";
}

}*/

//cout<<"Testing here"<<endl;

//--------------------------------------Done With 1 D ------------------------------------------------
if(rank!=0)
{

rc= MPI_Send(add,each*sizeof(Complex), MPI_CHAR, 0,0, MPI_COMM_WORLD);
 if (rc != MPI_SUCCESS)
 { cout << "Rank " << rank << " send failed, rc " << rc << endl;
 MPI_Finalize();

}

}
if(rank==0)
{for(int i=1;i<numtasks;i++)
{add=add+each;
rc= MPI_Recv(add, each*sizeof(Complex), MPI_CHAR, i, 0, MPI_COMM_WORLD, &stat);
 if (rc != MPI_SUCCESS)
 { cout << "Rank " << rank << " recv failed, rc " << rc << endl;
 MPI_Finalize();

}
}
add=add-((numtasks-1)*each);

}

if(rank==0)
//image.SaveImageData("1doutput.txt",add,w,h);

//---------------------------transposing---------------------------------

if(rank==0)
  {int k=0;
  for(int i=0;i<h;i++)
 {  for(int j=0;j<w;j++,k++)
 {
  store[i][j]=add[k];
  }

 }
}
if(rank==0)
  {int k=0;
  for(int i=0;i<h;i++)
 {  for(int j=0;j<w;j++,k++)
 {
  temp2[k]=store[j][i];
 
 }
 
  
}
/*for(int i=0;i<8;i++)
cout<<temp2[i].Mag()<<" ";*/
}
if(rank==0)
 {
    for(int i=0;i<numtasks;i++)
 {rc= MPI_Send(temp2,each*sizeof(Complex) ,MPI_CHAR, i, 0, MPI_COMM_WORLD);

 if (rc != MPI_SUCCESS)
 {
 cout << "Rank " << rank << " send failed, rc " << rc << endl;
 MPI_Finalize();
 
  }
  temp2=temp2+each;
  }
temp2=temp2-((numtasks-1)*each);
 
}
rc= MPI_Recv(copy, each*sizeof(Complex), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat);
 if (rc != MPI_SUCCESS)
 { cout << "Rank " << rank << " recv failed, rc " << rc << endl;
 MPI_Finalize();


}

 for(int i=0;i<size;i++)
{
add[i]=Complex(cos(0),sin(0))-Complex(cos(0),sin(0));
}

for(int j=0;j<h/numtasks;j++)
{
for(int n=(j*w);n<((j+1)*w);n++)
{
for(int k=0;k<w;k++)

{
add[n]=add[n]+(copy[k+(j*w)]*Complex(cos((((2*M_PI)*n*k)/w)),sin((((-2*M_PI)*n*k)/w))));

}


}

}
if(rank!=0)
{

rc= MPI_Send(add,each*sizeof(Complex), MPI_CHAR, 0,0, MPI_COMM_WORLD);
if (rc != MPI_SUCCESS)
{ cout << "Rank " << rank << " send failed, rc " << rc << endl;
 MPI_Finalize();
 
  }
}


 if(rank==0)
{for(int i=1;i<numtasks;i++)
{add=add+each;
 rc= MPI_Recv(add, each*sizeof(Complex), MPI_CHAR, i, 0, MPI_COMM_WORLD, &stat);
 if (rc != MPI_SUCCESS)
 { cout << "Rank " << rank << " recv failed, rc " << rc << endl;
  MPI_Finalize();


 }
 }
add=add-((numtasks-1)*each);

 }

if(rank==0)
  {int k=0;
  for(int i=0;i<h;i++)
 {  for(int j=0;j<w;j++,k++)
 {
  store[i][j]=add[k];
  }

 }
}
if(rank==0)
{
for(int i=0;i<h;i++)
{
for(int j=0;j<w;j++)
{
temp[i][j]=store[j][i];
}
}
}
if(rank==0)
  {int k=0;
  for(int i=0;i<h;i++)
 {  for(int j=0;j<w;j++,k++)
 {
  temp2[k]=temp[i][j];
  }

}

image.SaveImageData("Output2D.txt",temp2,w,h);
}   
//-------------------------------Done with 2d ---------------- inverse strts---------------------------------------
if(rank==0)
 {
    for(int i=0;i<numtasks;i++)
 {rc= MPI_Send(temp2,each*sizeof(Complex) ,MPI_CHAR, i, 0, MPI_COMM_WORLD);

 if (rc != MPI_SUCCESS)
 {
 cout << "Rank " << rank << " send failed, rc " << rc << endl;
 MPI_Finalize();

  }
  temp2=temp2+each;
  }
temp2=temp2-((numtasks-1)*each);

}
rc= MPI_Recv(copy, each*sizeof(Complex), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat);
 if (rc != MPI_SUCCESS)
 { cout << "Rank " << rank << " recv failed, rc " << rc << endl;
 MPI_Finalize();


}

 for(int i=0;i<size;i++)
{
add[i]=Complex(cos(0),sin(0))-Complex(cos(0),sin(0));
}

for(int j=0;j<h/numtasks;j++)
{
for(int n=(j*w);n<((j+1)*w);n++)
{
for(int k=0;k<w;k++)

{
add[n]=add[n]+(copy[k+(j*w)]*Complex(cos((((2*M_PI)*n*k)/w)),sin((((2*M_PI)*n*k)/w))));

}
add[n]=add[n]/inverse;

}
}

if(rank!=0)
{

rc= MPI_Send(add,each*sizeof(Complex), MPI_CHAR, 0,0, MPI_COMM_WORLD);
if (rc != MPI_SUCCESS)
{ cout << "Rank " << rank << " send failed, rc " << rc << endl;
 MPI_Finalize();

  }
}


 if(rank==0)
{for(int i=1;i<numtasks;i++)
{add=add+each;
 rc= MPI_Recv(add, each*sizeof(Complex), MPI_CHAR, i, 0, MPI_COMM_WORLD, &stat);
 if (rc != MPI_SUCCESS)
 { cout << "Rank " << rank << " recv failed, rc " << rc << endl;
  MPI_Finalize();


 }
 }
add=add-((numtasks-1)*each);

 }

if(rank==0)
  {int k=0;
  for(int i=0;i<h;i++)
 {  for(int j=0;j<w;j++,k++)
 {
  store[i][j]=add[k];
  }

 }
}
if(rank==0)
{
for(int i=0;i<h;i++)
{
for(int j=0;j<w;j++)
{
temp[i][j]=store[j][i];
}
}
}
if(rank==0)
  {int k=0;
  for(int i=0;i<h;i++)
 {  for(int j=0;j<w;j++,k++)
 {
  temp2[k]=temp[i][j];
  }

}
}
if(rank==0)
 {
    for(int i=0;i<numtasks;i++)
 {rc= MPI_Send(temp2,each*sizeof(Complex) ,MPI_CHAR, i, 0, MPI_COMM_WORLD);

 if (rc != MPI_SUCCESS)
 {
 cout << "Rank " << rank << " send failed, rc " << rc << endl;
 MPI_Finalize();

  }
  temp2=temp2+each;
  }
temp2=temp2-((numtasks-1)*each);

}
rc= MPI_Recv(copy, each*sizeof(Complex), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &stat);
 if (rc != MPI_SUCCESS)
 { cout << "Rank " << rank << " recv failed, rc " << rc << endl;
 MPI_Finalize();


}

 for(int i=0;i<size;i++)
{
add[i]=Complex(cos(0),sin(0))-Complex(cos(0),sin(0));
}

for(int j=0;j<h/numtasks;j++)
{
for(int n=(j*w);n<((j+1)*w);n++)
{
for(int k=0;k<w;k++)

{
add[n]=add[n]+(copy[k+(j*w)]*Complex(cos((((2*M_PI)*n*k)/w)),sin((((2*M_PI)*n*k)/w))));

}
add[n]=add[n]/inverse;

}
}
if(rank!=0)
{

rc= MPI_Send(add,each*sizeof(Complex), MPI_CHAR, 0,0, MPI_COMM_WORLD);
if (rc != MPI_SUCCESS)
{ cout << "Rank " << rank << " send failed, rc " << rc << endl;
 MPI_Finalize();

  }
}


 if(rank==0)
{for(int i=1;i<numtasks;i++)
{add=add+each;
 rc= MPI_Recv(add, each*sizeof(Complex), MPI_CHAR, i, 0, MPI_COMM_WORLD, &stat);
 if (rc != MPI_SUCCESS)
 { cout << "Rank " << rank << " recv failed, rc " << rc << endl;
  MPI_Finalize();


 }
 }
add=add-((numtasks-1)*each);

 }

if(rank==0)
  {int k=0;
  for(int i=0;i<h;i++)
 {  for(int j=0;j<w;j++,k++)
 {
  store[i][j]=add[k];
  }

 }
}
if(rank==0)
{
for(int i=0;i<h;i++)
{
for(int j=0;j<w;j++)
{
temp[i][j]=store[j][i];
}
}
}
if(rank==0)
  {int k=0;
  for(int i=0;i<h;i++)
 {  for(int j=0;j<w;j++,k++)
 {
  temp2[k]=temp[i][j];
  }

}
}
if(rank==0)
image.SaveImageData("Output2D-Inverse.txt",temp2,w,h);
 MPI_Finalize();
 }

