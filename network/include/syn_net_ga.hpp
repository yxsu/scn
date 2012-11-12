#pragma once
#include<genetic_algorithm.hpp>
#include<matrix.hpp>
#include<string>
using std::string;

/** SynNet_GA
 *
 * This class is used to seek a finest complext network in
 * terms of synchronization. 
 */

class SynNet_GA : public GeneticAlgorithm
{
public:
/**
 * @name SynNet_GA - Constructor
 * @param size -  the number of nodes in the network
 * @param number -  the number of edges in the network
 * @return SynNet_GA
 */
   SynNet_GA(size_t size, size_t number);

   ~SynNet_GA();
/**
 * @name Run - The entry of this algorithm
 * @param numberOfChromosome -  number Of Chromosomes in this experiment 
 * @param last_time - if the unchanged times is longer than last_time,
 * this algorithm will stop
 * @param numberOfThreads - it support multithread model
 * @return void
 */
   void Run(size_t numberOfChromosome, size_t iter_time);

protected:
/**
 * @name SaveToFile - Save log and final best network data into
 * @param filename - filename and path
 * @param isResult - Flag is Result 
 * @return void
 */
   void SaveToFile(string filename, bool isResult);

/**
 * @name DoCrossOverAndMutation - overrid oversion of cross-over and mutation
 * @param begin - the head of a range for a thread
 * @param end - the end of a range for a thread
 * @return void
 */
   void DoCrossOverAndMutation(size_t begin, size_t end);
/**
 * @name CreateMatrix - Creates the actual matrix based on the bits
 * stream, which represents the liking relationships among those nodes
 * in the networking
 * @param bits -  bits 
 * @param matrix -  matrix 
 * @return void
 */
   void CreateMatrix(const unordered_set<size_t>& bits, Matrix &matrix);
/**
 * @name FitnessFunction - the actual evaluation function in the
 * genetic algorithm
 * @param data -  the binary stream 
 * @return double, the fitness function, the smaller is better
 */
   virtual double FitnessFunction(const unordered_set<size_t> &data);
protected:

   size_t lengthOfBits;		/**<  length Of Bits which represent
				 * the linking relationship in the network */
   size_t dimension;		/**<  size of the complex network
				 * matrix. This is actually the
				 * number of nodes in the network */
   size_t numberOfEdges;	/**<  number Of Edges in the
				 * network. In the process of
				 * generating a new network, the
				 * number of edges must be unchanged */

   Matrix bestMatrix;	/**< the best matrix */
};
