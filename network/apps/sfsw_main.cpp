#include "../include/graph.hpp"
#include "../include/network_generator.hpp"
#include "../include/network_measurements.hpp"
#include <map>
#include <fstream>
#include <sstream>
using namespace std;
using namespace scn;

int main()
{
   int size = 15;
   UGraph::pGraph graph = GenTreeStructuredSFSW(size);
   //compute the average degree in each level
   map<size_t, pair<size_t, size_t>> level_degree;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      int count = 0;
      size_t index = *node + 1;
      while(index > 0)
      {
	 count++;
	 index >>= 1;
      }
      level_degree[count].first++;
      level_degree[count].second += node->GetDegree();
   }
   //save
   stringstream temp;
   temp<<"level_degree_"<<size<<".txt";
   ofstream outfile(temp.str());
   outfile<<"[level]   [number of node]   [average_degree]"<<endl;
   for(auto iter = level_degree.begin(); iter != level_degree.end(); iter++)
   {
      outfile<<iter->first<<"   "<<iter->second.first<<"   "
	     <<double(iter->second.second) / iter->second.first<<endl;
   }
   outfile.close();
   return 0;
}
