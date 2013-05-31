#include "network_measurements.hpp"
#include <cassert>
#include <cmath>
#include <queue>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <utility>
#include <vector>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <gnuplot.h>
using namespace scn;
using namespace std;

double scn::Ruler::ComputeAverageDegree()
{
   double sum = 0;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      sum += node->GetDegree();
   }
   return sum / graph->GetNumberOfNodes();
}

valarray<double> scn::Ruler::ComputeSpectrum()
{
   Matrix adjacency = graph->GetAdjacencyMatrix();
   return adjacency.GetEigenvalueList();
}

std::pair<double, double> scn::Ruler::GetLambda2AndRatio()
{
   auto list = graph->GetLaplacianMatrix().GetEigenvalueList();
   return make_pair(list[list.size() - 2], list[0] / list[list.size() - 2]);
}

Map<double> scn::Ruler::ComputeSpectralDensity(double slide)
{
   auto lambdas = ComputeSpectrum();
   //get the mapping
   Map<double> result;
   int start = floor(lambdas[0]);
   int end = ceil(lambdas[lambdas.size() - 1]);
   for(int i = 0; i < (end - start) / slide; i++)
   {
      result[double(start) + i * slide] = 0;
   }
   for(auto& iter : lambdas)
   {
      result[floor(iter / slide) * slide]++;
   }
   //normalized
   for(auto iter = result.begin(); iter != result.end(); iter++)
   {
      iter->second /= graph->GetNumberOfNodes();
   }
   return result;
}

size_t scn::Ruler::GetShortestDistance(size_t indexOfSource, size_t indexOfTarget)
{
   assert(graph->HasNode(indexOfSource) && graph->HasNode(indexOfTarget));
   //breadth-first search
   queue<size_t> search_queue;
   Map<size_t> distance;
   unordered_set<size_t> in_queue;
   size_t front;
   //initial
   distance[indexOfSource] = 0;
   search_queue.push(indexOfSource);
   in_queue.insert(indexOfSource);
   
   while(!search_queue.empty())
   {
      front = search_queue.front();
      search_queue.pop();
      in_queue.erase(front);

      auto node = graph->find(front);
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 if(*other == indexOfTarget)
	    return distance[*node] + 1;

	 if(in_queue.find(*other) == in_queue.end() &&
	    (distance.find(*other) == distance.end() || distance[front] + 1 < distance[*other]))
	 {
	    distance[*other] = distance[front] + 1;
	    search_queue.push(*other);
	    in_queue.insert(*other);
	 }
      }
   }
   //if this point is reached, the indexOfTarget is an isolated node 
   return Graph::NaF;
}

void scn::Ruler::RunDjikstra(size_t indexOfSource)
{
   auto& distance = distance_sssp;//using distance_sssp eariler
   assert(graph->HasNode(indexOfSource));
   //init
   distance.reserve(graph->GetNumberOfNodes());
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      distance[*node] = Graph::NaF;
   }
   distance[indexOfSource] = 0;

   list<size_t> queue;
   //fill index of nodes into queue
   for(size_t i = 0; i < graph->GetNumberOfNodes(); i++)
   {
      queue.push_back(i);
   }
   //begin
   size_t next_distance;
   while(!queue.empty())
   {
      //get min one
      auto min = min_element(queue.begin(), queue.end(), 
			     [&](const size_t &one, const size_t &two)->bool
			     {
				if(distance[one] < distance[two])
				   return true;
				else
				   return false;
			     });
      auto node = graph->find(*min);  

      if(distance[*node] < Graph::NaF)
	 next_distance = distance[*node] + 1;
      else
	 next_distance = Graph::NaF;
      //relax neighbors
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 if(distance[*other] > next_distance)
	 {
	    distance[*other] = next_distance;
	 } 
      }
      queue.erase(min);
   }
}

size_t scn::Ruler::GetDiameter()
{
   auto& distance = distance_sssp;
   size_t diameter = 0;
   distance.clear();
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      RunSPFA(*node);
      //search for the max distance except NaF
      
      for(auto other = graph->begin(); other != graph->end(); other++)
      {
	 if(distance[*other] > diameter && distance[*other] < Graph::NaF)
	 {
	    diameter = distance[*other];
	 }
      }
   }
   return diameter;
}

pair<size_t, double> scn::Ruler::GetDiameterAndAverageDistance()
{
   size_t sum = 0;
   size_t count = 0;
   size_t diameter = 0;
   // auto& distance = distance_sssp;
   //  distance.clear();
   Map<size_t> distance;
   for(size_t index = 0; index < graph->GetNumberOfNodes(); index++)
   {
      RunSPFA(index, distance);
      //add distance except NaF
      for(auto iter = distance.begin(); iter != distance.end(); iter++)
      {
          if(iter->first == index)
              continue;

          if(iter->second < Graph::NaF)
          {
              sum += iter->second;
              count++;
              if(iter->second > diameter)
                  diameter = iter->second;
          }
      }
   }
   return make_pair(diameter, double(sum) / count);
}

double scn::Ruler::GetAverageDistanceByDjikstra()
{
   size_t sum = 0;
   size_t count = 0;
   auto& distance = distance_sssp;
   distance.clear();
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      RunDjikstra(*node);
      //add distance except NaF
      for(auto other = graph->begin(); other != graph->end(); other++)
      {
	 if(node == other)
	    continue;

	 if(distance[*other] < Graph::NaF)
	 {
	    sum += distance[*other];
	    count++;
	 }
      }
   }
   return double(sum) / count;
}

double scn::Ruler::GetAverageDistance()
{
   size_t sum = 0;
   size_t count = 0;
   auto& distance = distance_sssp;
   distance.clear();
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      RunSPFA(*node);
      //add distance except NaF
      for(auto iter = distance.begin(); iter != distance.end(); iter++)
      {
	 if(iter->first == *node)
	    continue;

	 if(iter->second < Graph::NaF)
	 {
	    sum += iter->second;
	    count++;
	 }
      }
   }
   return double(sum) / count;
}

double scn::Ruler::GetGlobalEfficiency()
{
   double sum = 0;
   auto& distance = distance_sssp;
   distance.clear();
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      RunSPFA(*node);
      //add distance
      for(auto iter = distance.begin(); iter != distance.end(); iter++)
      {
	 if(iter->first == *node)
	    continue;

	 sum += 1.0 / static_cast<double>(iter->second);
      }
   }
   double size = static_cast<double>(graph->GetNumberOfNodes());
   return size * (size - 1) / sum;
}

void scn::Ruler::RunSPFA(size_t indexOfSource, Map<size_t>& distance)
{
   assert(graph->HasNode(indexOfSource));
//   Map<size_t>& distance = distance_sssp;
   //distance, stored the distance information of each node
   //init
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      distance[*node] = Graph::NaF;
   }
   distance[indexOfSource] = 0;
   queue<size_t> queue_buffer;
   unordered_set<size_t> in_queue;
   queue_buffer.push(indexOfSource);//push source node
   size_t front;
   while(!queue_buffer.empty())
   {
      front = queue_buffer.front();
      queue_buffer.pop();
      in_queue.erase(front);
      auto node = graph->find(front);
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 if(in_queue.find(*other) == in_queue.end() &&
	    distance[front] + 1 < distance[*other])
	 {
	    distance[*other] = distance[front] + 1;
	    queue_buffer.push(*other);
	    in_queue.insert(*other);
	 }
      }
   }
}

void scn::Ruler::RunSPFA(size_t indexOfSource)
{
   assert(graph->HasNode(indexOfSource));
//   Map<size_t>& distance = distance_sssp;
   //distance, stored the distance information of each node
   Map<size_t>& distance = distance_sssp;
   //init
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      distance[*node] = Graph::NaF;
   }
   distance[indexOfSource] = 0;

   static queue<size_t> queue_buffer;
   static unordered_set<size_t> in_queue;
   queue_buffer.push(indexOfSource);//push source node
   size_t front;
   while(!queue_buffer.empty())
   {
      front = queue_buffer.front();
      queue_buffer.pop();
      in_queue.erase(front);
      auto node = graph->find(front);
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 if(in_queue.find(*other) == in_queue.end() &&
	    distance[front] + 1 < distance[*other])
	 {
	    distance[*other] = distance[front] + 1;
	    queue_buffer.push(*other);
	    in_queue.insert(*other);
	 }
      }
   }
}

double scn::Ruler::GetVulnerability(size_t indexOfNode)
{
   double original_efficiency = GetGlobalEfficiency();
   IndexList edges;
   if(indexOfNode != Graph::NaF)
   {//compute the vulnerability of one node
      //remove the current node, note: only in_degree is used in
      //UGraph
      tie(edges, std::ignore) = graph->RemoveNode(indexOfNode);
      //node edge
      double new_efficiency = GetGlobalEfficiency();
      //restore the original graph
      graph->AddEdge(graph->AddNode(indexOfNode), edges);
      return (original_efficiency - new_efficiency) / original_efficiency;
   }
   else
   {//compute the vunlerability of the whole network
      IndexList setOfNode = graph->CopyIndexOfNodes();
      //compute
      double max_vul = -1e200, new_vul;
      for(auto node : setOfNode)
      {
	 //remove the current node, note: only in_degree is used in
	 //UGraph
	 tie(edges, std::ignore) = graph->RemoveNode(node);
	 new_vul = (original_efficiency - GetGlobalEfficiency()) / original_efficiency;
	 if(new_vul > max_vul)
	    max_vul = new_vul;
	 //restore the original graph
	 graph->AddEdge(graph->AddNode(node), edges);
      }
      return max_vul;
   }
}

double scn::Ruler::GetTransitivity()
{
   size_t numberOfTriangles = 0;
   size_t numberOfTriples = 0;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      for(auto other1 = node->begin(); other1 != node->end(); other1++)
      {
	 for(auto other2= other1 + 1; other2 != node->end(); other2++)
	 {
	    if(graph->HasEdge(*other1, *other2))
	    {
	       numberOfTriangles++;
	    }
	 }
      }
      numberOfTriples += node->GetDegree() * (node->GetDegree() - 1) / 2;
   }
   return static_cast<double>(numberOfTriangles) / 
      static_cast<double>(numberOfTriples);
}

double scn::Ruler::GetClusteringCoeff(size_t indexOfNode)
{
   if(indexOfNode == UGraph::NaF)
   {//the whole network
      double coefficient = 0;
      for(auto node = graph->begin(); node != graph->end(); node++)
      {
	 size_t numberOfTriangles = 0;
	 for(auto other1 = node->begin(); other1 != node->end(); other1++)
	 {
	    for(auto other2 = other1 + 1; other2 != node->end(); other2++)
	    {
	       if(graph->HasEdge(*other1, *other2))
		  numberOfTriangles++;
	    }
	 }
	 if(node->GetDegree() > 1)
	 {
	    coefficient += 2 * static_cast<double>(numberOfTriangles) /
	       (node->GetDegree() * (node->GetDegree() - 1));
	 }
      }
      return coefficient / graph->GetNumberOfNodes();
   }
   else
   {//one vertex
      auto node = graph->find(indexOfNode);
      double numberOfTriangles = 0;
      for(auto other1 = node->begin(); other1 != node->end(); other1++)
      {
	 for(auto other2 = other1 + 1; other2 != node->end(); other2++)
	 {
	    if(graph->HasEdge(*other1, *other2))
	       numberOfTriangles++;
	 }
      }
      if(node->GetDegree() > 1)
	 return 2 * numberOfTriangles / (node->GetDegree() * (node->GetDegree() - 1));
      else
	 return 0.0;
   }
}

double scn::Ruler::GetCyclicCoeff(size_t indexOfNode)
{
   IndexList edges;
   auto& distance = distance_sssp;
   distance.clear();
   if(indexOfNode != UGraph::NaF)
   {//one vertex
      tie(edges, std::ignore) = graph->RemoveNode(indexOfNode);
      double sum = 0;
      for(auto head = edges.begin(); head != edges.end(); head++)
      {
	 RunSPFA(*head);
	 for(auto tail = head + 1; tail != edges.end(); tail++)
	 {
	    sum += 1.0 / static_cast<double>(distance[*tail] + 2);
	 }
      }
      ///restore
      graph->AddEdge(graph->AddNode(indexOfNode), edges);
      return 2 * sum / static_cast<double>(edges.size() * (edges.size() - 1));
   }
   else
   {//the whole network
      //copy node list
      IndexList nodes = graph->CopyIndexOfNodes();
      double total_sum = 0;
      for(auto node = nodes.begin(); node != nodes.end(); node++)
      {
	 tie(edges, std::ignore) = graph->RemoveNode(*node);
	 double sum = 0;
	 for(auto head = edges.begin(); head != edges.end(); head++)
	 {
	    RunSPFA(*head);
	    for(auto tail = head + 1; tail != edges.end(); tail++)
	    {
	       sum += 1.0 / static_cast<double>(distance[*tail] + 2);
	    }
	 }
	 ///restore, add node first, then add list of edges of this node
	 graph->AddEdge(graph->AddNode(*node), edges);
	 //accumulate
	 total_sum += 2 * sum / static_cast<double>(edges.size() * (edges.size() - 1));
      }
      return total_sum / static_cast<double>(graph->GetNumberOfNodes());
   }
}

double scn::Ruler::GetRichClubCoeff(size_t degree)
{
   IndexList setOfHighNode;//whose degree is greater than
                           //argument degree
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      if(node->GetDegree() > degree)
      {
	 setOfHighNode.push_back(*node);
      }
   }
   double sum = 0;
   for(auto one = setOfHighNode.begin(); one != setOfHighNode.end(); one++)
   {
      for(auto two = one + 1; two != setOfHighNode.end(); two++)
      {
	 if(graph->HasEdge(*one, *two))
	    sum++;
      }
   }
   return 2 * sum / static_cast<double>(setOfHighNode.size() * (setOfHighNode.size() - 1));
}

double scn::Ruler::GetPearsonCorrCoeff()
{
   size_t sum = 0;
   size_t product = 0;
   size_t square_sum = 0;
   for(auto i = graph->begin(); i != graph->end(); i++)
   {
      for(auto j = i + 1; j != graph->end(); j++)
      {
	 if(graph->HasEdge(*i, *j))
	 {
	    sum += i->GetDegree() + j->GetDegree();
	    product += i->GetDegree() * j->GetDegree();
	    square_sum += i->GetDegree() * i->GetDegree() + j->GetDegree() * j->GetDegree();
	 }
      }
   }
   double tmp = 1.0 / static_cast<double>(graph->GetNumberOfEdges());
   return (tmp * product - (tmp * sum) * (tmp * sum) / 4) /
      (tmp * square_sum / 2 - (tmp * sum) * (tmp * sum) / 4); 
}

double scn::Ruler::GetEntropyOfDegreeDist()
{
   //get degree distribution
   Map<size_t> distribution;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      distribution[node->GetDegree()]++;
   }
   //compute the entropy
   double sum = 0;
   double pk = 0;
   for(auto iter = distribution.begin(); iter != distribution.end(); iter++)
   {
      pk = static_cast<double>(iter->second) / graph->GetNumberOfNodes();
      sum -= pk * log2(pk);
   }
   return sum;
}

double scn::Ruler::GetSearchInfo(size_t indexOfSource, size_t indexOfTarget)
{
   assert(graph->HasNode(indexOfSource));
   assert(graph->HasNode(indexOfTarget));
   assert(indexOfSource != indexOfTarget);
   //get shortest distance
   auto& distance = distance_sssp;
   RunSPFA(indexOfSource);
   double sum = 0;
   //search in breadth-first way
   queue<pair<size_t, double>> path_queue;//pair : indexOfNode,
					  //probability of path
   path_queue.push(make_pair(indexOfTarget, 1.0));
   while(!path_queue.empty())
   {
      auto current = path_queue.front();
      auto node = (*graph)[current.first];
      path_queue.pop();
      
      for(auto other = node->begin(); other != node->end(); other++)
      {//put all of previous-node in path into queue
	 if(distance[*other] == distance[*node] - 1)
	 {
	    if(distance[*other] == 0)
	       sum += current.second / static_cast<double>(node->GetDegree());
	    else
	       path_queue.push(make_pair(*other, current.second / 
					 static_cast<double>((*graph)[*other]->GetDegree() - 1))); 
	 }
      }
   }
   return - log2(sum);
}

double scn::Ruler::GetAverageSearchInfo()
{
   double sum = 0;
   for(auto node1 = graph->begin(); node1 != graph->end(); node1++)
   {
      for(auto node2 = graph->begin(); node2 != graph->end(); node2++)
      {
	 if(node1 == node2)
	    continue;
	 sum += GetSearchInfo(*node1, *node2);
      }
   } 
   return sum / static_cast<double>(graph->GetNumberOfNodes() * graph->GetNumberOfNodes());
}

double scn::Ruler::GetAccessInfo(size_t indexOfNode)
{
   double sum = 0;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      if(*node == indexOfNode)
	 continue;

      sum += GetSearchInfo(indexOfNode, *node);
   }
   return sum / static_cast<double>(graph->GetNumberOfNodes());
}

double scn::Ruler::GetHideInfo(size_t indexOfNode)
{
   double sum = 0;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      if(*node == indexOfNode)
	 continue;

      sum += GetSearchInfo(*node, indexOfNode);
   }
   return sum / static_cast<double>(graph->GetNumberOfNodes());
}

pair<size_t, size_t> scn::Ruler::GetNumberOfShortestPath(size_t indexOfSource, size_t indexOfTarget,
							 size_t indexOfThrough)
{
   assert(graph->HasNode(indexOfSource));
   assert(graph->HasNode(indexOfTarget));
   //find shortest path
   auto& distance = distance_sssp;
   RunSPFA(indexOfSource);
   
   if(indexOfThrough == UGraph::NaF)
   {//no passing through any specified node
      size_t sum = 0;
      queue<size_t> path;
      path.push(indexOfTarget);
      //breadth-first search
      while(!path.empty())
      {
	 if(indexOfSource == path.front())
	    sum++;
	 else
	 {
	    auto current = graph->find(path.front());
	    size_t current_distance = distance[*current];
	    for(auto other = current->begin(); other != current->end(); other++)
	    {
	       if(distance[*other] == current_distance - 1)
		  path.push(*other);
	    }
	 }
	 path.pop();
      }
      return make_pair(sum, sum);
   }
   else
   {
      assert(graph->HasNode(indexOfThrough));
      assert(indexOfSource != indexOfThrough);
      assert(indexOfTarget != indexOfThrough);

      size_t sum_all = 0;
      size_t sum_through = 0;
      queue<pair<size_t, bool>> path;//pair(indexOfNode, Does it Pass
				     //through the give node?
      path.push(make_pair(indexOfTarget, false));
      //breadth-first search
      while(!path.empty())
      {
	 auto current = path.front();
	 if(current.first == indexOfThrough)
	    current.second = true;
	 
	 if(indexOfSource == current.first)
	 {
	    sum_all++;
	    if(current.second)
	       sum_through++;
	 }
	 else
	 {
	    size_t current_distance = distance[current.first];
	    auto current_node = graph->find(current.first);
	    for(auto other = current_node->begin(); other != current_node->end(); other++)
	    {
	       if(distance[*other] == current_distance - 1)
		  path.push(make_pair(*other, current.second));
	    }
	 }
	 path.pop();
      }
      return make_pair(sum_all, sum_through);
   }
}


double scn::Ruler::GetBetweennessCentrality(size_t indexOfNode)
{
   double sum = 0;
   for(auto node1 = graph->begin(); node1 != graph->end(); node1++)
   {
      if(*node1 == indexOfNode)
	 continue;

      for(auto node2 = graph->begin(); node2 != graph->end(); node2++)
      {
	 if(*node2 == indexOfNode || node1 == node2)
	    continue;
	 //compute
	 auto result = GetNumberOfShortestPath(*node1, *node2, indexOfNode);
	 sum += static_cast<double>(result.second) / 
	    static_cast<double>(result.first);
      }
   }
   return sum;
}

scn::IndexList scn::Ruler::FindClosureGroup(size_t indexOfSeed, size_t length)
{
   unordered_set<size_t> list({indexOfSeed});
   //breadth-first search to find adjacent nodes
   queue<size_t> search_queue;
   unordered_set<size_t> in_queue;
   //init
   search_queue.push(indexOfSeed);
   in_queue.insert(indexOfSeed);
   //begin
   size_t front;
   while(!search_queue.empty())
   {
      front = search_queue.front();
      search_queue.pop();
      in_queue.erase(front);
      auto node = graph->find(front);
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 if(in_queue.find(*other) != in_queue.end())
	    continue;
	 if(list.find(*other) != list.end())
	    continue;
	 if(all_of(list.begin(), list.end(), [&](size_t i)->bool
		   {
		      if(GetShortestDistance(i, *other) < length)
			 return true;
		      else
			 return false;
		   }))
	 {
	    search_queue.push(*other);
	    in_queue.insert(*other);
	    list.insert(*other);
	 }
      }
   }
   return IndexList(list.begin(), list.end());
}

string scn::Ruler::DrawDegreeDistribution()
{
   Map<size_t> distribution;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      distribution[node->GetDegree()]++;
   }
   //copy and sort
   vector<pair<double, double>> pairs(distribution.begin(), distribution.end());
   sort(pairs.begin(), pairs.end(), [&](const pair<size_t, size_t> &one, 
					const pair<size_t, size_t> &two)
	{
	   if(one.first < two.first)
	      return true;
	   else
	      return false;
    });
   //draw
   Gnuplot plot;
   plot.SetXYData(pairs);
   plot.SetFileName("degree_distribution");
   return plot.DrawHistogram();
}

Matrix scn::Ruler::GetGeodesicMatrix()
{
   Matrix result(graph->GetNumberOfNodes(), graph->GetNumberOfNodes());
   auto& distance = distance_sssp;
   size_t diameter = 0;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      RunSPFA(*node);
      for(auto iter = distance.begin(); iter != distance.end(); iter++)
      {
	 result(*node, iter->first) = iter->second;
	 if(iter->second > diameter)
	 {
	    diameter = iter->second;
	 }
      }
   }
   result.Print("Matrix:");

   for(size_t i = 0; i < result.GetHeight(); i++)
   {
      result(i, i) = - valarray<double>(result.row(i)).sum();
   }

   result /= static_cast<double>(diameter);
   return result;
}

size_t scn::Ruler::RandomWalkByURW(size_t indexOfSource, size_t indexOfTarget)
{
   unordered_set<size_t> neighbors_of_target(graph->find(indexOfTarget)->begin(),
					     graph->find(indexOfTarget)->end());
   vector<size_t> neighbors;
   size_t steps = 0;
   size_t next = indexOfSource;
   srand(size_t(time(00)));
   do
   {
      neighbors.assign(graph->find(next)->begin(), graph->find(next)->end());
      next = neighbors[rand() % neighbors.size()];
      steps++;
   }while(neighbors_of_target.find(next) == neighbors_of_target.end());

   return steps;
}

size_t scn::Ruler::MFPTofRandomWalk()
{
   size_t total_steps = 0;
   vector<size_t> neighbors;
   srand(size_t(time(00)));

   for(size_t source = 0; source < graph->GetNumberOfNodes(); source++)
   {

       for(size_t target = 0; target < graph->GetNumberOfNodes(); target++)
       {
           size_t steps = 0;
           size_t next = source;
           do
           {
               neighbors.assign(graph->find(next)->begin(), graph->find(next)->end());
               next = neighbors[rand() % neighbors.size()];
               steps++;
           }while(next != target);

           total_steps += steps;
       }
   }
   return static_cast<double>(total_steps) /
           static_cast<double>(graph->GetNumberOfNodes() * graph->GetNumberOfNodes());
}

size_t scn::Ruler::MRTofRandomWalk()
{
   
   size_t total_steps = 0;
   vector<size_t> neighbors;
   srand(size_t(time(00)));

   for(size_t source = 0; source < graph->GetNumberOfNodes(); source++)
   {
       size_t next = source;
       size_t steps = 0;
       do
       {
           neighbors.assign(graph->find(next)->begin(), graph->find(next)->end());
           next = neighbors[rand() % neighbors.size()];
           steps++;
       }while(next != source);
       total_steps += steps;
   }
   return static_cast<double>(total_steps) / 
      static_cast<double>(graph->GetNumberOfNodes());
}

size_t scn::Ruler::RandomWalkByNRRW(size_t indexOfSource, size_t indexOfTarget)
{
   unordered_set<size_t> neighbors_of_target(graph->find(indexOfTarget)->begin(),
					     graph->find(indexOfTarget)->end());
   vector<size_t> neighbors;
   size_t steps = 0;
   size_t next = indexOfSource;
   size_t previous = indexOfSource;
   srand(size_t(time(00)));
   do
   {
      neighbors.assign(graph->find(next)->begin(), graph->find(next)->end());
      do
      {
	 next = neighbors[rand() % neighbors.size()];
      }while(next == previous);
      steps++;
      previous = next;
   }while(neighbors_of_target.find(next) == neighbors_of_target.end());

   return steps;
}

size_t scn::Ruler::RandomWalkBySARW(size_t indexOfSource, size_t indexOfTarget)
{
   unordered_set<size_t> neighbors_of_target(graph->find(indexOfTarget)->begin(),
					     graph->find(indexOfTarget)->end());
   vector<size_t> neighbors;
   size_t steps = 0;
   size_t next = indexOfSource;
   unordered_set<size_t> history;
   stack<size_t> precessors;
   history.insert(indexOfSource);
   precessors.push(indexOfSource);

   srand(size_t(time(00)));
// judge first
   if(graph->HasEdge(indexOfSource, indexOfTarget))
      return 1;

   do
   {
      auto other = graph->find(next);
      neighbors.clear();
      for(auto iter = other->begin(); iter != other->end(); iter++)
      {
	 if(history.find(*iter) == history.end())
	    neighbors.push_back(*iter);
      }
      if(neighbors.empty())
      {
	 history.insert(*other);
	 precessors.pop();
	 next = precessors.top();
	 history.insert(next);
	 steps++;
	 continue;
      }
      do
      {
	 next = neighbors[rand() % neighbors.size()];
      }while(history.find(next) != history.end());
      steps++;
      history.insert(next);
      precessors.push(next);
   }while(neighbors_of_target.find(next) == neighbors_of_target.end());

   return steps;
}

Map<size_t> scn::Ruler::GetShortestDistanceDistribution()
{
   Map<size_t> distribution;
   auto& distance = distance_sssp;
   distance.clear();
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      RunSPFA(*node);
      //add distance except NaF
      for(auto iter = distance.begin(); iter != distance.end(); iter++)
      {
	 if(iter->first == *node)
	    continue;

	 if(iter->second < Graph::NaF)
	 {
	    distribution[iter->second]++;
	 }
      }
   }
   return distribution;
}

DVector<size_t, double> scn::Ruler::GetPSDegreeCorrelation()
{
   //get degree distribution
   Map<size_t> degree_dist;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      degree_dist[node->GetDegree()]++;
   }
   //compute degree correlation
   unordered_map<size_t, double> degree_corre;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      double degree_sum = 0;
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 degree_sum += (*graph)[*other]->GetDegree();
      }
      degree_corre[node->GetDegree()] += degree_sum / 
	 static_cast<double>(node->GetDegree()); 
   }
   //normalize
   for(auto iter = degree_corre.begin(); iter != degree_corre.end(); iter++)
   {
      iter->second /= degree_dist[iter->first];
   }
   DVector<size_t, double> list_corre(degree_corre.begin(), degree_corre.end());
   sort(list_corre.begin(), list_corre.end());
   return list_corre;
}

pair<double, DVector<size_t, double>> scn::Ruler::GetClusteringDegreeCorre()
{
   unordered_map<size_t, double> degree_dist, degree_cc;
   //accumulate degree and clustering coefficient
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      degree_dist[node->GetDegree()]++;
      degree_cc[node->GetDegree()] += GetClusteringCoeff(*node);
   }
   //get average
   double degree_average = 0;//1-order moment of degree
   double degree_sqr_average = 0;//2-order moment of degree
   for(auto iter = degree_dist.begin(); iter != degree_dist.end(); iter++)
   {
      //average clustering coefficient in each degree
      degree_cc[iter->first] /= iter->second;
      //probability of degree
      iter->second /= graph->GetNumberOfNodes();
      //1-order moment of degree
      degree_average += iter->first * iter->second;
      //2-order moment of degree
      degree_sqr_average += iter->first * iter->first * iter->second;
   }
   //compute global clustering degree correlation
   double clustering_degree_corre = 0;
   for(auto iter = degree_dist.begin(); iter != degree_dist.end(); iter++)
   {
      clustering_degree_corre += iter->first * (iter->first - 1) * iter->second
	 * degree_cc[iter->first];
   }
   clustering_degree_corre /= (degree_sqr_average - degree_average);
   //sort and get result
   DVector<size_t, double> result(degree_cc.begin(), degree_cc.end());
   sort(result.begin(), result.end());
   return make_pair(clustering_degree_corre, result);
}

DVector<double, double> scn::Ruler::GetClusteringCoeffDist(double slide)
{
   assert(slide >= 0 && slide <= 1);
   if(slide == 0)
   {
      Map<double> cc_dist;
      for(auto node = graph->begin(); node != graph->end(); node++)
      {
	 cc_dist[GetClusteringCoeff(*node)]++;
      }
      for(auto iter = cc_dist.begin(); iter != cc_dist.end(); iter++)
      {
	 iter->second /= graph->GetNumberOfNodes();
      }
      DVector<double, double> result(cc_dist.begin(), cc_dist.end());
      sort(result.begin(), result.end());
      
      return result;
   }
   else
   {
      unordered_map<size_t, double> cc_dist;
      for(auto node = graph->begin(); node != graph->end(); node++)
      {
	 cc_dist[static_cast<size_t>(GetClusteringCoeff(*node) / slide)]++;
      }
      DVector<double, double> result(cc_dist.begin(), cc_dist.end());
      sort(result.begin(), result.end());
      for(auto iter = result.begin(); iter != result.end(); iter++)
      {
	 iter->first = iter->first * slide + slide / 2;
	 if(iter->first > 1)
	    iter->first = 1;

	 iter->second /= graph->GetNumberOfNodes();
      }
      return result;
   }
}
