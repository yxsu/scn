#include "../include/norm_network.hpp"
#include "../include/network_generator.hpp"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <utility>
#include <random>
#include <cmath>
#include <stack>
#include <functional>
using namespace std;
using namespace scn;
/*
scn::NormNet::NormNet()
   :UGraph()
{
}

scn::NormNet::NormNet(size_t size)
   : UGraph(size)
{
}

scn::NormNet::~NormNet()
{

}

void scn::NormNet::CreateDegreeTable(size_t numberOfEdge)
{
   degree_coeff.clear();
   if(numberOfEdge * 2 % GetNumberOfNodes() == 0)
   {//average degree
      degree_coeff.push_back(numberOfEdge * 2 / GetNumberOfNodes());
      list_degree_num.emplace_back(1, GetNumberOfNodes());//only one solution:{GetNumberOfNodes()}
   }
   else
   {
      double average_degree = static_cast<double>(numberOfEdge) * 2 / GetNumberOfNodes();
      cout<<"average_degree is "<<average_degree<<endl;
      vector<double> prob(GetNumberOfNodes());
      for(size_t i = 0; i < prob.size(); i++)
      {
	 prob[i] = GetGaussianValue(static_cast<double>(i), average_degree, 1.0);
      }
      //normize
      double sum = accumulate(prob.begin(), prob.end(), 0.0);
      for(size_t i = 0; i < prob.size(); i++)
      {
	 prob[i] *= static_cast<double>(GetNumberOfNodes()) / sum;
	 if(prob[i] >= 0.9)
	 {
	    degree_coeff.push_back(i);
	    cout<<"prob["<<i<<"]="<<prob[i]<<endl;
	 }
      }
      sort(degree_coeff.begin(), degree_coeff.end(), 
	   [&prob](const int &one, const int &two)->bool
	   {
	      return prob[one] < prob[two] ? true : false;
	   });
      //print degree_coeff
      cout<<"coeff :";
      for(auto value : degree_coeff)
      {
	 cout<<value<<" ";
      }
      cout<<endl;
      //try to find a integral solution of this degree distribution
      SolveDegreeNumber(GetNumberOfNodes(), numberOfEdge);
      //print list
      for(auto iter = list_degree_num.begin();
	  iter != list_degree_num.end(); iter++)
      {
	 cout<<"number:";
	 for(auto value : *iter)
	    cout<<value<<" ";
	 //test graphic
	 Map<size_t> degree_table;
	 for(size_t i = 0; i < degree_coeff.size(); i++)
	 {
	    degree_table[degree_coeff[i]] = iter->at(i);
	 }
	 if(!IsDegreeListGraphical(degree_table))
	 {
	    cout<<" is not graphic";
	    iter = list_degree_num.erase(iter);
	    iter--;//iter points to next position previously, this operation
		   //eliminates the effect of iter++ in for loop head
	 }
	 else
	    cout<<" is graphic";
	 cout<<endl;
      }
   }  
}

void scn::NormNet::SolveDegreeNumber(int numberOfNodes, int numberOfEdge)
{
   assert(numberOfNodes != 0 && numberOfEdge != 0);
   list_degree_num.clear();
   int product = numberOfEdge * 2;
   auto head = degree_coeff.begin();
   deque<int> pool;
   function<void(int, int)> recursive = [&](int numberOfNodes, int product)
   {  
      int final = min(numberOfNodes, product / *(head + pool.size()));
      for(int i = 0; i <= final; i++)
      {
	 if(i == numberOfNodes && *(head + pool.size()) * i == product)
	 {//find a solution
	    pool.push_back(i);
	    list_degree_num.emplace_back(degree_coeff.size(), 0);
	    copy(pool.begin(), pool.end(), list_degree_num.rbegin()->begin());
	    pool.pop_back();
	    continue;
	 }
	 pool.push_back(i);
	 if(pool.size() < degree_coeff.size())
	 {
	    recursive(numberOfNodes - i, product - *(head + pool.size()-1) * i);
	 }
	 pool.pop_back();
      }
   };
   //solve
   recursive(numberOfNodes, product);
}

void scn::NormNet::TryDegreeNumber(int current, int least, int sum_node, 
				   int sum_edge, vector<pair<int, int>>& coeff)
{
//   cout<<"("<<current<<","<<least<<","<<sum_node<<","<<sum_edge<<")"<<endl;
   
   //condition
//   if(sum_edge < 0 || sum_node < 0 ||current >= coeff.size())
//      return;
   if(sum_edge ==  coeff[current].first * sum_node)
   {
      
      if(sum_node < least)
	 return;
      //     if(current < coeff.size() - 1)
//	 return;//the last position should not be zero

      coeff[current].second = sum_node;
      vector<int> result(coeff.size());
      for(size_t i = 0; i < coeff.size(); i++)
      {
	 result[i] = coeff[i].second;
      }
      list_degree_num.push_back(result);
      return;
   }
   //test current = 0
   coeff[current].second = 0;
   TryDegreeNumber(current + 1, least, sum_node, sum_edge, coeff);
   //test other
   for(int i = least; i < sum_node; i++)
   {
            
      coeff[current].second = i;
      TryDegreeNumber(current + 1, i, sum_node - i,
		      sum_edge - i * coeff[current].first, coeff);
   }
}


bool scn::NormNet::AddEdge(size_t indexOfHead, size_t indexOfTail,
		      bool reserve)
{
    if(unfull_nodes.find(indexOfHead) == unfull_nodes.end())
      return false;//head has reached its final degree
   if(unfull_nodes.find(indexOfTail) == unfull_nodes.end())
      return false;//tail has reached its final degree
   if(HasEdge(indexOfHead, indexOfTail))
      return false;
   
   UGraph::AddEdge(indexOfHead, indexOfTail);
   
   if(GetDegree(indexOfHead) == degreeOfNode[indexOfHead])
      unfull_nodes.erase(indexOfHead);
   if(GetDegree(indexOfTail) == degreeOfNode[indexOfTail])
      unfull_nodes.erase(indexOfTail);
   return true;
}

bool scn::NormNet::RemoveEdge(size_t indexOfHead, size_t indexOfTail,
			 bool reserve)
{
   if(!HasEdge(indexOfHead, indexOfTail))
      return false;
   
   UGraph::RemoveEdge(indexOfHead, indexOfTail);
   //add to unfull_nodes;
   unfull_nodes.insert(indexOfHead);
   unfull_nodes.insert(indexOfTail);
   return true;
}

void scn::NormNet::Build(size_t numberOfEdge)
{
   CreateDegreeTable(numberOfEdge);
   SymMatrix::pMatrix best_graph;
   double best_lambda2 = 0;
   
   for(auto number : list_degree_num)
   {
      //create degreeOfNode
      degreeOfNode.clear();
      for(size_t i = 0; i < number.size(); i++)
      {
	 degreeOfNode.insert(degreeOfNode.end(), number[i], degree_coeff[i]);
      }
      LinkNodes();
      double temp = GetLambda2();
      if(abs(temp) > best_lambda2)
      {
	 best_graph = GetAdjacencyMatrix();
	 best_lambda2 = abs(temp);
      }
      else
      {
	 clear();
	 Create(best_graph->GetSize());
      }
   }
   CreateFromAdjacencyMatrix(best_graph);
   cout<<"Best lambda 2 of ("<<GetNumberOfNodes()<<", "<<GetNumberOfEdges()
       <<") is "<<best_lambda2<<endl;
}

void scn::NormNet::LinkNodes()
{
   //set unfull_nodes;
   for(auto node = begin(); node != end(); node++)
   {
      unfull_nodes.insert(*node);
   }
   
   while(!unfull_nodes.empty())
   {
      //small degree
      vector<size_t> candidate(unfull_nodes.begin(), unfull_nodes.end());
      sort(candidate.begin(), candidate.end(),
	   [&](const size_t &one, const size_t &two)->bool
	   {//less than
	      if(degreeOfNode[one] > degreeOfNode[two])
		 return true;//put bigger degree to the head of
			     //candiate list
	      else if(degreeOfNode[one] < degreeOfNode[two])
		 return false;
	      else
	      {//degree number are same, compare the sum of degree of
	       //linked nodes
		 size_t sum_one = 0;
		 for(auto iter = begin(); iter != end();iter++)
		 {
		    sum_one += degreeOfNode[*iter];
		 }
		 size_t sum_two = 0;
		 for(auto iter = begin(); iter != end();iter++)
		 {
		    sum_two += degreeOfNode[*iter];
		 }
		 if(sum_one > sum_two)
		    return true;
		 else if(sum_one < sum_two)
		    return false;
		 else
		 {
		    if(GetDegree(one) > GetDegree(two))
		       return true;
		    else
		       return false;
		 }
	      }
	   });
      //big degree
      auto iter_big = candidate.begin();
      auto iter_small = candidate.rbegin();
      if(*iter_small == *iter_big)
	 iter_small++;
      //print the candidate
      //  cout<<"Candidate list is "<<endl;
//      for(size_t i = 0; i < candidate.size(); i++)
//      {
//	 cout<<candidate[i]<<" ";
//      }
//      cout<<endl;

      while(HasEdge(*iter_big, *iter_small))
      {
	 iter_small++;
	 if(*iter_small == *iter_big)
	    return;
      }
      AddEdge(*iter_big, *iter_small);
       while(!AddEdge(*iter_big, *iter_small))
      {
	 iter_small++;
	 if(iter_small == candidate.rend())
	    break;
      }
      */
/*
      //final
      if(unfull_nodes.size() == 2 && HasEdge(*unfull_nodes.begin(),
					     *unfull_nodes.rbegin()))
      {
	 cout<<"-------Break !---------"<<endl;
	 size_t smallest_degree = 10e10;
	 for(auto node = begin(); node != end(); node++)
	 {
	    if(smallest_degree > node->GetDegree())
	       smallest_degree =  node->GetDegree();
	 }
	 for(auto node = begin(); node != end(); node++)
	 {
	    if(node->GetDegree() == smallest_degree && 
	       *iter_big != *node && !HasEdge(node, *iter_big))
	    {
	       UGraph::AddEdge(node, *iter_big);
	       break;
	    }
	 }
	 break;
      }
      
   }
}

double scn::NormNet::GetLambda2()
{
   auto list = GetLaplacianMatrix()->GetEigenvalueList();
   return (*list)[GetNumberOfNodes() - 2];
}

double scn::NormNet::GetGaussianValue(double x, double mu, double sigma)
{
   return exp(- (x - mu)*(x - mu) /(2 * sigma * sigma)) / (sqrt(2* 3.1415) * sigma);
}
*/
