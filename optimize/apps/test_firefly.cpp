#include "../include/firefly_algorithm.hpp"
#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class FunctionTest : public FireflyAlgorithm
{
public:
   FunctionTest(size_t numberOfFirefly, size_t dimension)
      : FireflyAlgorithm(numberOfFirefly, dimension)
   {};
   ~FunctionTest()
   {};
   virtual void UpdateIntensity(ptFirefly &firefly)
   {
      assert(dimension == 2);
      firefly->intensity = function(firefly->position[0], firefly->position[1]);
   }
   void PrintTheBest()
   {
      sort(swarm.begin(), swarm.end(), [](const ptFirefly &one,
					  const ptFirefly &two)->bool
	   {
	      if(one->intensity < two->intensity)
		 return true;
	      else
		 return false;
	   });
      for(size_t i = 0; i < swarm.size(); i++)
      {
	 cout<<"The best intensity is "<<swarm[i]->intensity;
	 cout<<" (x, y) : "<<"("<<swarm[i]->position[0]<<","
	     <<swarm[i]->position[1]<<")"<<endl;
      
      }
   }
protected:
   
   double function(double x, double y)
   {
      double temp = exp(-(x-4)*(x-4) - (y-4)*(y-4));
      temp += exp(-(x+4)*(x+4) - (y-4)*(y-4));
      temp += 2 * exp(-x*x - (y+4)*(y+4));
      temp += 2 * exp(-x*x - y*y);
      return temp;
   }
};

int main()
{
   FunctionTest test(12, 2);
   vector<double> max = {5,5};
   vector<double> min = {-5, -5};
   vector<double> random = {1, 1};
   test.InitSwarm(max, min, random);
   test.Run(100);
   test.PrintTheBest();

   return 0;
}
