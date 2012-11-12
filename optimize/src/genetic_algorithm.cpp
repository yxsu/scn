#include "../include/genetic_algorithm.hpp"
#include<cassert>
#include<algorithm>
#include<iterator>
#include<random>
using std::sort;
using std::copy;
using std::insert_iterator;


GeneticAlgorithm::GeneticAlgorithm(void)
{
}
GeneticAlgorithm::~GeneticAlgorithm(void)
{
}

void GeneticAlgorithm::GetBestChromosome(Chromosome &best)
{
   assert(!swarm.empty());
   best.fitness_result = swarm[0]->fitness_result;
   best.data = swarm[0]->data;
}

void GeneticAlgorithm::InitChromosomeSwarm(size_t numbers, size_t length, size_t numberOfTrue = 0)
{
   swarm.clear();
   swarm.reserve(2 * numbers);
   //random generator
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> random(0, length -1);

   for(size_t i = 0; i < numbers; i++)
   {
      //init a chromosome
      shared_ptr<Chromosome> temp(new Chromosome);
      temp->fitness_result = INIT_FITNESS_VALUE;
      temp->lengthOfChromosome = length;
      if(numberOfTrue > 0)
      {
	 temp->data.reserve(numberOfTrue * 2);

	 while(temp->data.size() < numberOfTrue)
	 {
	    temp->data.insert(random(gen));
	 }
      }
      else
      {
	 temp->data.reserve(length);
	 for(size_t j = 0; j < length / 2; j++)
	 {
	    temp->data.insert(random(gen));
	 }
      }
      //rand
      swarm.push_back(temp);
   }
   numberOfChromosome = numbers;
   lengthOfChromosome = length;
}

void GeneticAlgorithm::DoCrossOverAndMutation(size_t mutation_times)
{
   shared_ptr<Chromosome> child_one, child_two;
   unordered_set<size_t> mutation_flag;
   //random generator
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> random(0, lengthOfChromosome -1);

   for(size_t parent = 0; parent < lengthOfChromosome - 1; parent+=2)
   {//choose a pair (iter, iter + 1)
      //assign
      child_one.reset(new Chromosome(*swarm[parent]));
      child_two.reset(new Chromosome(*swarm[parent + 1]));
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
      while(mutation_flag.size() < mutation_times)
      {
	 mutation_flag.insert(random(gen));
      }
      //mutation
      for(auto iter : mutation_flag)
      {
	 child_one->flip(iter);
	 child_two->flip(iter);
      }
      //compute fitness result
      child_one->fitness_result = FitnessFunction(child_one->data);
      child_two->fitness_result = FitnessFunction(child_two->data);
      //add
      swarm.push_back(child_one);
      swarm.push_back(child_two);
   }
}

void GeneticAlgorithm::DoSelection()
{
   //sort
   sort(swarm.begin(), swarm.end(), 
	[](const shared_ptr<Chromosome> &left, 
	   const shared_ptr<Chromosome> &right)->bool
	{
	   if(left->fitness_result < right->fitness_result)
	      return true;
	   else
	      return false;
	});
   swarm.erase(swarm.begin() + numberOfChromosome, swarm.end());
}

void GeneticAlgorithm::SetCrossOverPattern(vector<size_t> &pattern)
{
   assert(pattern.size() <= lengthOfChromosome);
   
   copy(pattern.begin(), pattern.end(), 
	insert_iterator<unordered_set<size_t> >(cross_over_pattern, cross_over_pattern.begin()));
}
