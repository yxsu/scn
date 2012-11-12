#pragma once

#include <string>
#include "graph.hpp"

namespace scn
{
   namespace plot
   {
/**
 * @name DrawCircleForm - write the structure of an undirected
 * graph into dot form, then call GraphViz to render this graph
 * @param graph -  graph 
 * @param filename - the name of dot file and rendered image file
 * @return void
 */
      void DrawCircleForm(Graph::pGraph graph, std::string filename);
   }
};
