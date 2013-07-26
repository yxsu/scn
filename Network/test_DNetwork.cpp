#include "DNetwork.hpp"
#include <iostream>

using namespace std;
using namespace scn;

int main()
{
   DNetwork<double, size_t> two;
   two.AddNode(12.0);
   two.AddNode(29.4);
   two.AddNode(10, 29.4);
   two.AddEdge(0, 1, 10);
   two(0, 1).data() = 5;
   //
   for(auto node = two.begin(); node != two.end(); node++)
   {
      for(auto edge = node.out_begin(); edge != node.out_end(); edge++)
      {
	 cout<<*node<<"("<<node.data()<<") -- "<<edge.data()<<" -> "<<*edge<<"("<<two(*edge).data()<<")"<<endl;
      }
   }
   return 0;
}


















