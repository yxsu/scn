
#include "../include/wavelet.hpp"

#include <vector>
#include <cstdlib>
#include <iostream>
using namespace std;

#define N 16
#define logN 4

int main()
{
   Wavelet wavelet;
   vector<double> test_data(N);
   
   cout<<"Original data is:"<<endl;
   for(size_t i = 0; i < N; i++)
   {
      test_data[i] = double(rand() % 983) / 467;
      cout<<test_data[i]<<" ";
      if(i % 4 == 3)
	 cout<<endl;
   }
   cout<<endl;
   wavelet.InitWavelet(Wavelet::HAAR_2, false);
   wavelet.LoadData(test_data);
   wavelet.Transform(true);
   //print
   vector<double> result;
   wavelet.GetData(-1, result);
   cout<<"Smoothing coefficients:"<<result[0]<<endl;
   for(size_t i = 0; i < logN; i++)
   {
      wavelet.GetData(i, result);
      cout<<"Level "<<i<<endl;
      for(size_t j = 0; j < result.size(); j++)
      {
	 cout<<result[j]<<" ";
	 if(j % 4 == 3)
	    cout<<endl;
      }
      cout<<endl;
   }

   return 0;
}
