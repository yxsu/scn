#pragma once
#include "graph.hpp"

namespace scn
{
/**
 * @name RandomWalkByURW - unrestricted random walk
 * @param graph -  graph 
 * @param indexOfSource - index Of Source 
 * @param indexOfTarget - index Of Target 
 * @return size_t - number of steps of this random walk
 */
   size_t RandomWalkByURW(scn::UGraph::pGraph graph,
			size_t indexOfSource, size_t indexOfTarget);
/**
 * @name RandomWalkByNRRW - no-retracting random walk
 * @param graph -  graph 
 * @param indexOfSource - index Of Source 
 * @param indexOfTarget - of index Of Target 
 * @return size_t - number of steps of this random walk
 */
   size_t RandomWalkByNRRW(scn::UGraph::pGraph graph,
			 size_t indexOfSource, size_t indexOfTarget);
/**
 * @name RandomWalkBySARW - self-avoiding random walk
 * @param graph -  graph 
 * @param indexOfSource - Number of index Of Source 
 * @param indexOfTarget - Number of index Of Target 
 * @return size_t
 */
   size_t RandomWalkBySARW(scn::UGraph::pGraph graph,
			 size_t indexOfSource, size_t indexOfTarget);
}
