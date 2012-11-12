#include "../include/graph_io.hpp"
#include <fstream>
#include <chrono>
#include <ctime>
using std::ofstream;
using std::ios_base;
using std::endl;

void scn::io::WriteToDotFile(scn::Graph::pGraph graph, std::string filename)
{
   ofstream out_file(filename + ".dot", ios_base::out | ios_base::trunc);
   //print the head
   out_file<<"graph G {"<<endl;
   //print edges list
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      size_t node_id = node->GetIndexOfNode();
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 if(node_id > *other && graph->HasEdge(node_id, *other))
	    continue;
	 
	 out_file<<"   "<<node_id<<" -- "<<*other<<endl;
      }
   }
   //print the tail
   out_file<<"}"<<endl;
   out_file.close();
}

void scn::io::WriteToTikzFile(scn::Graph::pGraph graph, std::string filename)
{
   ofstream out_file(filename + ".tex", ios_base::out | ios_base::trunc);
   float diameterOfNode = 0.5;
   float radius = diameterOfNode * graph->GetNumberOfNodes() / (2*3.14); 
   //write header
   out_file<<"\\documentclass{article}"<<endl
	   <<"\\usepackage{tikz}"<<endl
	   <<"\\begin{document}"<<endl
	   <<"\\begin{tikzpicture}"<<endl
           <<"[feature/.style={fill=red, circle, inner sep = "<<diameterOfNode<<"}]"<<endl;
   //write body
   //draw node
   float angle = 0;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      out_file<<"\\node[feature] ("<<*node<<") at ("<<angle<<" : "<<radius/1.5<<") {};"<<endl;
      angle += 360.0 / graph->GetNumberOfNodes();
   }
   //draw edge
  for(auto node = graph->begin(); node != graph->end(); node++)
  {
     for(auto other = node->begin(); other != node->end(); other++)
     {
         out_file<<"\\draw ("<<*node<<") -- ("<<*other<<");"<<endl;
     } 
  }
   //write end
   out_file<<"\\end{tikzpicture}"<<endl
	   <<"\\end{document}"<<endl;
   out_file.close();
   std::system(std::string("xelatex " + filename +".tex").c_str());
   std::system(std::string("evince "+filename + ".pdf").c_str());
   
}

void scn::io::WriteToNetFile(Graph::pGraph graph, std::string filename)
{
   using namespace std::chrono;
   ofstream outfile(filename, std::ios_base::trunc);
   //print the head
   std::time_t date = system_clock::to_time_t(system_clock::now());
   outfile<<"*Network Regular, created at "
	  <<std::ctime(&date);
   outfile<<"*Vertices "<<graph->GetNumberOfNodes()<<endl;
   outfile<<"*Edges"<<endl;
   //print edge list
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 outfile<<*node<<" "<<*other<<" 1"<<endl;
      }
   }
   outfile.close();
}
