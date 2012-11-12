#include "../include/graph.hpp"
#include "../include/network_generator.hpp"
#include "../include/network_measurements.hpp"
using namespace std;
using namespace scn;

inline void WriteToNetFile(std::string path, UNetwork<>::pNetwork &network)
   {
      using std::endl;
      std::ofstream outfile(path);
      UGraph::pGraph graph = network->GetTopology();
      
      outfile<<"*Vertices "<<graph->GetNumberOfNodes()<<endl;
      //write node
      for(auto node = graph->begin(); node != graph->end(); node++)
      {
	 auto position = network->GetNodePosition(*node);
	 outfile<<*node + 1<<" "<<*node + 1<<" "<<position[0]<<" "
		<<position[1]<<" "<<position[2]<<endl;
      }
      
      outfile<<"*Arcs"<<endl;
      outfile<<"*Edges"<<endl;

      //write edge
      for(auto node = graph->begin(); node != graph->end(); node++)
      {
	 for(auto other = node->begin(); other != node->end(); other++)
	 {
	    if(*other < *node)
	    {
	       outfile<<*node + 1<<" "<<*other + 1<<" 1"<<endl;
	    }
	 }
      }
      outfile.close();
   }

   inline UNetwork<>::pNetwork ReadUNetworkFromNetFile(std::string path)
   {
      using std::getline;
      using std::string;
      using std::cout;
      using std::endl;
      std::ifstream infile(path);
      UGraph::pGraph graph(new UGraph());
      UNetwork<>::pNetwork network(new UNetwork<>(graph));

      string line;
      string temp;
      std::stringstream ss;
      //read header
      while(getline(infile, line))
      {
	 ss.str(line);
	 if(ss>>temp && temp == "*Vertices")
	 {//read nodes
	    size_t numberOfNodes;
	    ss>>numberOfNodes;
	    size_t index;
	    string flag;
	    double x,y,z;
	    for(size_t i = 0; i < numberOfNodes; i++)
	    {
	       getline(infile, line);
	       ss.clear();
	       ss.str(line);
	       if(ss>>index>>flag>>x>>y>>z)//read content
	       {
		  graph->AddNode(index - 1);
		  network->SetNodePosition(index - 1, x, y, z);
	       }
	    }
	    assert(numberOfNodes == graph->GetNumberOfNodes());
	 }
	 else if(line == "*Edges")
	 {//read edge
	    size_t indexOfNode1, indexOfNode2;
	    double weight;
	    while(getline(infile, line))
	    {
	       ss.clear();
	       ss.str(line);
	       if(ss>>indexOfNode1>>indexOfNode2>>weight)
		  graph->AddEdge(indexOfNode1 - 1, indexOfNode2 -1);
	    }
	 }
      }
      infile.close();
      return network;
   }


int main()
{
   // UGraph::pGraph graph = GenSmallWorldByEdgeIteration(3);
   UGraph::pGraph graph = GenPinWheelShapedSW(4);
   UNetwork<>::pNetwork network(new UNetwork<>(graph));
   
   Ruler ruler(graph);
   auto result = ruler.GetClusteringCoeffDist(0.5);
   // cout<<"global clustering coefficient is "<<result.first<<endl;
   for(auto iter : result)
   {
      cout<<iter.first<<" : "<<iter.second<<endl;
   }
   //WriteToNetFile("test.net", network);
   
   UNetwork<>::pNetwork new_network = ReadUNetworkFromNetFile("test.net");
   UGraph::pGraph new_graph = new_network->GetTopology();
   cout<<"old "<<"("<<graph->GetNumberOfNodes()<<", "<<graph->GetNumberOfEdges()<<")"<<endl;
   cout<<"new "<<"("<<new_graph->GetNumberOfNodes()<<", "<<new_graph->GetNumberOfEdges()<<")"<<endl;
   return 0;
}
