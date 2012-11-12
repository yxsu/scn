#include "../include/firefly_algorithm.hpp"
#include <cassert>
#include <random>
#include <cmath>
#include <iostream>
using std::cout;
using std::endl;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::sqrt;
using std::exp;

FireflyAlgorithm::FireflyAlgorithm(size_t numberOfFirefly, size_t dimension)
   : dimension(dimension)
{
   swarm.resize(numberOfFirefly);
}

FireflyAlgorithm::~FireflyAlgorithm()
{
}

void FireflyAlgorithm::InitSwarm(vector<double> &range_max, vector<double> &range_min,
				 vector<double> &range_random)
{
   //assign
   this->range_max.assign(range_max.begin(), range_max.end());
   this->range_min.assign(range_min.begin(), range_min.end());
   scaling.assign(range_random.begin(), range_random.end());
   //compute absorption coefficient
   absorption = 1 / ComputeDistance2(range_max, range_min);
   //create random device
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> random(0, 1);
   //create the swarm
   for(size_t i = 0; i < swarm.size(); i++)
   {
      swarm[i].reset(new Firefly);
      swarm[i]->position.resize(dimension);

      for(size_t d = 0; d < dimension; d++)
      {
	 swarm[i]->position[d] = range_min[d] + random(gen) * (range_max[d] - range_min[d]);
      }
      //compute intensity
      UpdateIntensity(swarm[i]);
   }
}

double FireflyAlgorithm::ComputeDistance(vector<double> &one, vector<double> &other)
{
   assert(one.size() == other.size());
   
   double result = 0;
   double buffer;
   for(size_t i = 0; i < one.size(); i++)
   {
      buffer = one[i] - other[i];
      result += buffer * buffer;
   }
   return sqrt(result);
}

double FireflyAlgorithm::ComputeDistance2(vector<double> &one, vector<double> &other)
{
   assert(one.size() == other.size());
   
   double result = 0;
   double buffer;
   for(size_t i = 0; i < one.size(); i++)
   {
      buffer = one[i] - other[i];
      result += buffer * buffer;
   }
   return result;
}

void FireflyAlgorithm::Run(size_t maxGeneration)
{
   double itoj, jtoi;//relative intensities

   for(size_t times = 0; times < maxGeneration; times++)
   {
      for(size_t i = 0; i < swarm.size(); i++)
      {
	 for(size_t j = 0; j < swarm.size(); j++)
	 {
	    //compute relative intensities
	    itoj = ComputeRelativeIntensity(swarm[i], swarm[j]->position);
	    jtoi = ComputeRelativeIntensity(swarm[j], swarm[i]->position);
	    //move
	    if(itoj == jtoi)
	       continue;
	    else if(itoj > jtoi)
	    {//move j to i
	       Move(swarm[j], swarm[i]);
	       UpdateIntensity(swarm[j]);
	    }
	    else
	    {//move i to j
	       Move(swarm[i], swarm[j]);
	       UpdateIntensity(swarm[i]);
	    }
	 }
      }
      //print a firefly
      cout<<"iter "<<times<<" firefly 0: intensity is "<<swarm[0]->intensity
	  <<" (x,y) : ("<<swarm[0]->position[0]<<","
	  <<swarm[0]->position[1]<<")"<<endl;
   }
}

double FireflyAlgorithm::ComputeRelativeIntensity(ptFirefly &firefly, vector<double> &at)
{
   double distance2 = ComputeDistance2(firefly->position, at);
   return firefly->intensity * exp( - absorption * distance2);
}

void FireflyAlgorithm::Move(ptFirefly &from, ptFirefly &to)
{
   //create random device
   random_device rd;
   mt19937 gen(rd());
   uniform_int_distribution<> random(0, 1);

   double alpha = random(gen);
   double distance2 = ComputeDistance2(from->position, to->position);

   for(size_t d = 0; d < dimension; d++)
   {
      double value = exp(- absorption * distance2) * (to->position[d] - from->position[d])
	 + from->position[d] + alpha * (random(gen) - 0.5) * scaling[d];
      //check the bounds
      if(value < range_min[d])
	 value = range_min[d];
      if(value > range_max[d])
	 value = range_max[d];

      from->position[d] = value;
   }
}
