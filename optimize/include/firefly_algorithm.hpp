#pragma once
#include <vector>
#include <memory>
using std::vector;
using std::shared_ptr;

class FireflyAlgorithm
{
public:
   struct Firefly
   {
      double intensity;
      vector<double> position;
   };
   typedef shared_ptr<Firefly> ptFirefly;
public:
   
   FireflyAlgorithm(size_t numberOfFirefly, size_t dimension);

   ~FireflyAlgorithm();

   void InitSwarm(vector<double> &range_max, vector<double> &range_min, 
		  vector<double> &range_random);

   void Run(size_t maxGeneration);

protected:

   virtual void UpdateIntensity(ptFirefly &firefly) = 0;

   double ComputeRelativeIntensity(ptFirefly &firefly, vector<double> &at);
/**
 * @name Move - Move firefly "from" toward firefly "to"
 * @param from -  from 
 * @param to -  to 
 * @return void
 */
   void Move(ptFirefly &from, ptFirefly &to);
/**
 * @name ComputeDistance - Compute the standard Cartesian distance
 * @param one -  one 
 * @param other -  other 
 * @return double
 */
   virtual double ComputeDistance(vector<double> &one, vector<double> &other);
/**
 * @name ComputeDistance2 - Compute the power of Cartesian distance
 * @param one -  one 
 * @param other -  other 
 * @return double
 */
   virtual double ComputeDistance2(vector<double> &one, vector<double> &other);

protected:/* data */
   
   size_t dimension;
   double absorption;
   vector<double> range_max;
   vector<double> range_min;
   vector<double> scaling;
   vector<ptFirefly> swarm;
};
