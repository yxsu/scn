#include "../include/matrix.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <valarray>
#include <ctime>
using namespace std;

#define SIZE 5
int main()
{
   // Matrix::pMatrix test(new Matrix(SIZE, SIZE));
   Matrix test1(SIZE, SIZE), test2(SIZE, SIZE);
   for(size_t row = 0; row < SIZE; row++)
   {
      for(size_t col = 0; col < SIZE; col++)
      {
	 test1(row, col) = rand() % SIZE;
	 test2(row, col) = rand() % SIZE;
      }
   }
   test1.Print("test1");
   test2.Print("test2");
   Matrix test3(test1, slice(0, 3, 1), slice(0,3,2));
   test3.Print("test3");
   test3 = 6;
   test3.Print("test3 -6");
   Matrix test4(3, {1, 2, 3, 2, 4, 5, 3, 5, 6});
   test4.Print("test4");
   
   clock_t start = clock();
   Matrix result = test1 * test2;
   clock_t end = clock();
   cout<<"Time comsuming in valarray is "<<double(end - start)/CLOCKS_PER_SEC<<endl;
   if(test4.IsSymmetric() == true)
      cout<<"symmetric"<<endl;
   else
      cout<<"asymmetric"<<endl;
   cout<<"eigenvalue list is"<<endl;
   auto eigenvector = test4.GetEigenValueAndVector();
   for(auto& i : eigenvector)
   {
      cout<<"value is "<<i.first<<endl;
      for(auto &j : i.second)
	 cout<<j<<" ";
      cout<<endl;
   }
   test4.DrawSpectralDensity(0.01);
//
   //set
/*   ifstream infile("matrix.txt");
   int buffer;
   for(size_t row = 0; row < SIZE; row++)
   {
      for(size_t col = 0; col < SIZE; col++)
      {
	 infile >> buffer;
	 test->SetElement(row, col, double(buffer));
      }
   }
   infile.close();
   //print
   cout<<"test is "<<endl;
   for(size_t row = 0; row < SIZE; row++)
   {
      for(size_t col = 0; col < SIZE; col++)
      {
	 cout<<test->GetElement(row, col)<<" ";
      }
      cout<<endl;
   }
   //compute eigenvalue and vector
   SymMatrix::pValueVectorList result = test->GetEigenValueAndVector();
   for(auto iter = result->begin(); iter != result->end(); iter++)
   {
      cout<<"-------------------------------"<<endl;
      cout<<iter->first<<endl;
      for(auto iter_vector = iter->second->begin(); 
	  iter_vector != iter->second->end(); iter_vector++)
      {
	 cout<<" "<<*iter_vector;
      }
      cout<<endl;
   }*/
   return 0;
}
