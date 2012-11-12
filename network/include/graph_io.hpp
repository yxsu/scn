#pragma once
#include "graph.hpp"
#include <string>
namespace scn
{
   namespace io
   {
/**
 * @name WriteToDotFile - write the structure of an undirected
 * graph into the dot form, which used as input of GraphViz
 * @param graph -  graph 
 * @param filename - Name of
 * @return void
 */
      void WriteToDotFile(Graph::pGraph graph, std::string filename);

      void WriteToTikzFile(Graph::pGraph graph, std::string filename);
/**
 * @name ReadFromNetFile - Reads graph from Pajek's .net file.
 * @param filename - Name of
 * @return std::pair
 */
      std::pair<Graph::pGraph, bool> ReadFromNetFile(std::string filename);

      void WriteToNetFile(Graph::pGraph graph, std::string filename);
   };
};
