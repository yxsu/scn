#include "DNetwork.hpp"
#include <iostream>

using namespace std;
using namespace scn;

int main()
{
   DNetwork<> one;
   DNetwork<double, size_t> two;
   two.AddNode(12.0);
   two.AddNode(29.4);
   two.AddNode(10, 29.4);
   cout<<two.HasNode(1)<<endl;
   return 0;
}


















