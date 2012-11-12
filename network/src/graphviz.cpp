#include "../include/graphviz.hpp"
#include "../include/graph_io.hpp"
#include<string>
#include<cassert>

void scn::plot::DrawCircleForm(scn::Graph::pGraph graph, std::string filename)
{
   scn::io::WriteToDotFile(graph, filename);

   std::string temp("circo -Tpng ");
   temp += filename + ".dot";
   temp += " -o " + filename +".png";
   
   if(std::system(temp.c_str()) != 0)
   {
      assert(false);
   }
}
