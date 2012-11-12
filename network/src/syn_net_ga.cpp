#include "../include/syn_net_ga.hpp"
#include "../include/graph.hpp"
#include "../include/graph_io.hpp"
#include "../include/graphviz.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <ctime>
#include <fstream>
#include <sstream>
#include <memory>
#include <functional>
#include <omp.h>
using std::accumulate;
using std::cout;
using std::cerr;
using std::endl;
using std::max_element;
using std::shuffle;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::unordered_set;
using std::clock_t;
using std::clock;
using std::ofstream;
using std::stringstream;
using std::shared_ptr;
using std::bind;

SynNet_GA::SynNet_GA(size_t size, size_t number)
   : GeneticAlgorithm()
{
   assert(size > 0);

   dimension = size;
   lengthOfBits = (size - 1) * size / 2;//upper triangle
   numberOfEdges = number;
}

SynNet_GA::~SynNet_GA()
{
}

void SynNet_GA::CreateMatrix(const unordered_set<size_t> &bits, Matrix &matrix)
{
   assert(bits.size() == numberOfEdges);
   matrix = Matrix(dimension, dimension);
   //add linking relationship
   for(size_t row = 1; row < dimension; row++)
   {
      for(size_t col = 0; col < row; col++)
      {
	 if(bits.find((row - 1)*row / 2 + col) != bits.end())//true, 1
	 {
	    matrix(row, col) = 1;
	 }
	 else
	 {
	    matrix(row, col) = 0;
	 }
      }
   }
   //set the diagonal
   for(size_t row = 0; row < dimension; row++)
   {
      matrix(row, row) = - valarray<double>(matrix.row(row)).sum();//keep the sum of whole row unchanged
   }
}

double SynNet_GA::FitnessFunction(const unordered_set<size_t>  &data)
{   
   valarray<double> eigenvalue;
   Matrix bufferMatrix;
   CreateMatrix(data, bufferMatrix);
   /* compute the second largest eigenvalue. If this value is
    * smaller, the result is better
    */
   eigenvalue = bufferMatrix.GetEigenvalueList(); 
   if(eigenvalue.size() == 0)
   {
      cerr<<"Faill in GetEigenvalueList !"<<endl;
      return INIT_FITNESS_VALUE;
   }
   else
   {
      double lambda2 = eigenvalue[bufferMatrix.GetWidth() - 2];
      if(lambda2 > -1e-6)
      {
	return INIT_FITNESS_VALUE;
      }
      else
	return eigenvalue[0] / lambda2;

   }
}


void SynNet_GA::Run(size_t numberOfChromosome, size_t last_time)
{
   //init
   InitChromosomeSwarm(numberOfChromosome, lengthOfBits, numberOfEdges);
   cout<<"init ok..."<<endl;
   //set the cross-over pattern
   cross_over_pattern.clear();
   size_t range = dimension / 2;
   cross_over_pattern.reserve(range * (range + 1));
   for(size_t col = range; col < dimension ; col++)
   {
      size_t base = (col -1) * col / 2;
      for(size_t i = 0; i < range; i++ )
      {
	 cross_over_pattern.insert(i + base);
      }
   }
   Chromosome best;
   double previous_best = INIT_FITNESS_VALUE;
   size_t count_unchanged = 0, iter_time = 0;
   clock_t start, previous, now;
   start = clock();
   previous = start;
   cout<<"begin to run..."<<endl;
   //run
   while(count_unchanged < last_time)
   {
      DoCrossOverAndMutation(0, numberOfChromosome);
      DoSelection();
      //print the best
      GetBestChromosome(best);
      now = clock();
//      cout<<"edge "<<numberOfEdges<<" iter "<<iter_time<<" : best: "<<best.fitness_result
//	  <<" (time: "<<double(now - previous)/CLOCKS_PER_SEC/omp_get_num_procs()<<"/"
//	  <<double(now - start)/CLOCKS_PER_SEC/omp_get_num_procs()<<")"<<endl;
      previous = now;
      //change
      if(best.fitness_result < previous_best)
      {
	 previous_best = best.fitness_result;
	 count_unchanged = 0;
      }
      else
      {
	 count_unchanged++;
      }
      iter_time++;
   }
   //print the best
   CreateMatrix(best.data, bestMatrix);
   scn::UGraph::pGraph graph(new scn::UGraph(bestMatrix.GetWidth()));
   
   for(size_t row = 0; row < bestMatrix.GetWidth(); row++)
   {
      for(size_t col = 0; col < bestMatrix.GetWidth(); col++)
      {
	 cout<<bestMatrix(row, col)<<" ";
	 if(bestMatrix(row, col) == 1)
	    graph->AddEdge(row, col);
      }
      cout<<endl;
   }
   //save to file
   stringstream ss;
   ss<<"data/net_"<<bestMatrix.GetWidth()<<"_"<<numberOfEdges<<"_"<<best.fitness_result<<"_ga_"
     <<numberOfChromosome<<"_"<<iter_time;
   //draw
   scn::plot::DrawCircleForm(graph, ss.str());
   ss<<".net";
   SaveToFile(ss.str(), true);
   
}

void SynNet_GA::SaveToFile(string filename, bool isResult)
{
   ofstream outfile(filename, std::ios_base::trunc);
   //print the head
   outfile<<"*Network Regular"<<endl;
   outfile<<"*Vertices "<<bestMatrix.GetWidth()<<endl;
   outfile<<"*Edges"<<endl;
   //print the edges
   for(size_t row = 1; row < bestMatrix.GetWidth(); row++)
   {
      for(size_t col = 0; col < row; col++)
      {
	 if(abs(bestMatrix(row, col) - 1) < 1e-6)
	 {
	    outfile<<row + 1<<" "<<col + 1<<" 1"<<endl;
	 }
      }
   }
   outfile.close();
}

void SynNet_GA::DoCrossOverAndMutation(size_t begin, size_t end)
{
   shared_ptr<Chromosome> child_one, child_two;
   unordered_set<size_t> mutation_flag;
   //random generator
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> random(0, lengthOfChromosome -1);
   swarm.reserve(2* numberOfChromosome);
//#pragma omp parallel for private(child_one, child_two, mutation_flag,rd, gen) firstprivate(random)
   for(size_t parent = begin; parent < end / 2; parent++)
   {//choose a pair (iter, iter + 1)  	
      //assign
      child_one.reset(new Chromosome(*swarm[2*parent]));
      child_two.reset(new Chromosome(*swarm[2*parent + 1]));
      //cross over
      for(auto iter : cross_over_pattern)
      {
	 if(child_one->IsTrue(iter) ^ child_two->IsTrue(iter))
	 {
	    child_one->flip(iter);
	    child_two->flip(iter);
	 }
      }      
      //create mutation flag
      mutation_flag.clear();
      while(mutation_flag.size() < numberOfEdges)
      {
	 mutation_flag.insert(random(gen));
      }
      //mutation
      for(auto iter : mutation_flag)
      {
	 child_one->flip(iter);
	 child_two->flip(iter);
      }
      //modify the mutation result to keep the numberOfEdges unchanged
      child_one->KeepTrueUnchanged(numberOfEdges);
      child_two->KeepTrueUnchanged(numberOfEdges);
      //compute fitness result
      child_one->fitness_result = FitnessFunction(child_one->data);
      child_two->fitness_result = FitnessFunction(child_two->data);
      //add
//      #pragma omp critical
      {
	 swarm.push_back(child_one);
	 swarm.push_back(child_two);
      }
   }
}
