#pragma once
#include<vector>
#include<unordered_set>
#include<memory>
#include<random>
#include<cassert>
using std::vector;
using std::unordered_set;
using std::shared_ptr;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;


/** GeneticAlgorithm
 *
 * 
 */

class GeneticAlgorithm
{
public:
   static constexpr double INIT_FITNESS_VALUE = 1.0e10;/**< the init value of
					   * fitness_result in each chromosome */
   struct Chromosome
   {
      unordered_set<size_t>	data;/**<  data represent the binary
				      * stream, the indexs in the set
				      * represent the corresponding
				      * bit is 1 */
      double  fitness_result;
      size_t lengthOfChromosome;/**<  length Of Chromosome  */
      
      Chromosome()
	 : fitness_result(INIT_FITNESS_VALUE)
      {
      };
      Chromosome(const Chromosome &other)
      {
	 fitness_result = other.fitness_result;
	 data = other.data;
	 lengthOfChromosome = other.lengthOfChromosome;
      };
      
      inline bool IsTrue(size_t index)
      {
	 assert(index < lengthOfChromosome);

	 if(data.find(index) != data.end())
	    return true;
	 else
	    return false;
      }
      inline void flip(size_t index)
      {
	 assert(index < lengthOfChromosome);
	 
	 auto iter = data.find(index);
	 if(iter == data.end())
	 {//previous 0
	    data.insert(index);
	 }
	 else
	 {//previous 1
	    data.erase(iter);
	 }
      }
/**
 * @name KeepTrueUnchanged - keep the number of 1 unchanged
 * @return void
 */
      inline void KeepTrueUnchanged(size_t numberOfTrue)
      {
	 //random generator
	 random_device rd;
	 mt19937 gen(rd());
	 uniform_int_distribution<> random(0, lengthOfChromosome -1);
	 
	 while(data.size() < numberOfTrue)
	 {
	    data.insert(random(gen));
	 }
	 while(data.size() > numberOfTrue)
	 {
	    //point to the position
	    auto iter = data.begin();
	    size_t length = random(gen) % data.size();
	    while(length > 0)
	    {
	       iter++;
	       length--;
	    }
	    
	    data.erase(iter);
	 }
      }
   };
public:
/**
 * @name GeneticAlgorithm - Default constructor
 * @param  - Empty 
 * @return GeneticAlgorithm
 */
   GeneticAlgorithm(void);
/**
 * @name GeneticAlgorithm - Default destructor
 * @param  - Empty 
 * @return void
 */
   ~GeneticAlgorithm(void);
public://public operation
/**
 * @name FitnessFunction - the evaluation function. This is a pure
 * virtual function, it must be implemented in the derived class
 * @param data -  data 
 * @return double
 */
   virtual double FitnessFunction(const unordered_set<size_t> &data) = 0;
/**
* @name InitChromosomeSwarm - Init this experiment.
* @param numbers -  number of chromosome in this experiment
* @param length -  length of each chromosome
* @return void
*/
   void InitChromosomeSwarm(size_t numbers, size_t length, size_t numberOfTrue);
/**
 * @name SetCrossOverPattern - Specify which bits are used in cross over
 * @param pattern -  it stores the index of bits which are used in
 * cross over
 * @return void
 */
   void SetCrossOverPattern(vector<size_t> &pattern);
protected:
/**
 * @name DoCrossOverAndMutation - stage of cross over
 * @param flag -  flag, designate which bit is used to cross over
 * @param mutation_times -  mutation times 
 * @return void
 */
   void DoCrossOverAndMutation(size_t mutation_times);
/**
 * @name DoSelection - stage of selection. Save numberOfChromosome
 * chromosomes whose fitness result is well
 * @return void
 */
   void DoSelection();
/**
 * @name GetBestChromosome - get the best chromosome
 * @param best -  best 
 * @return void
 */
   void GetBestChromosome(Chromosome &best);
protected://private data
   vector<shared_ptr<Chromosome> >    swarm;
   unordered_set<size_t>                  cross_over_pattern;
   size_t numberOfChromosome;	/**<  number Of Chromosome in genetic algorithm  */
   size_t lengthOfChromosome;	/**<  length Of Chromosome  */
};
