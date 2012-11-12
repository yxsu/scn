#pragma once
#include "graph.hpp"
#include <matrix.hpp>

namespace scn
{
   class Ruler
   {
   public://constructor
/**
 * @name Ruler - Default constructor, the to-be-measured graph is
 * necessary
 * @param graph -  graph 
 * @return Ruler
 */
      Ruler(scn::UGraph::pGraph graph)
	 :graph(graph){};
/**
 * @name Ruler - destructor
 * @return void
 */
      ~Ruler(){};
   public://public operations

      double ComputeAverageDegree();
/**
 * @name ComputeSpectrum - compute the spectrum of the adjacency
 * matrix of the given graph
 * @return std::vector
 */
      valarray<double> ComputeSpectrum();
/**
 * @name GetLambda2AndRatio - compute the lambda 2 of Lapalacian
 * matrix and the the ratio of lambda2 and lambda n
 * @return std::pair<lambda2, ratio>
 */
      std::pair<double, double> GetLambda2AndRatio();
/**
 * @name DrawDegreeDistribution - Draw the histogram image of degree
 * distribution of the graph. The drawing tool is Gnuplot
 * @return std::string - return the path of generated image
*/
      std::string DrawDegreeDistribution();
/**
 * @name ComputeSpectralDensity - Compares the
 * @param slide - Identifier of
 * @return Mapping
 */
      Map<double> ComputeSpectralDensity(double slide);
/**
 * @name GetEntropyOfDegreeDist - compute the entropy of degree
 * distribution. This measurement is related to the robustness of
 * network
 * @return double
 */
      double GetEntropyOfDegreeDist();
/**
 * @name GetShortestDistance - compute shortest distance between two
 * nodes, this function is implemented by breadth-first search, thus
 * only unweighted graph is valid.  
 * @param indexOfSource -  index Of Source 
 * @param indexOfTarget -  index Of Target 
 * @return size_t
 */
      size_t GetShortestDistance(size_t indexOfSource, size_t indexOfTarget);
/**
 * @name GetShortestDistanceDistribution - compute the distribution of
 * shortest distance among all pairs of nodes in the network
 * @return Map
 */
      Map<size_t> GetShortestDistanceDistribution();
/**
 * @name GetDeodesicMatrix - Get a symmetric matrix whose element
 * reprensents the short-length between a pair of nodes
 * @return Matrix
 */
      Matrix GetGeodesicMatrix();

/**
 * @name GetDiameter - compute the diameter of a given graph, the
 * default algorithm is SPFA
 * @return size_t
 */
      size_t GetDiameter();
/**
 * @name GetDiameterAndAverageDistance - compute diameter and average
 * distance together. It is a efficient way if these two value are needed
 * @return pair<diameter, average_distance>
 */
      std::pair<size_t, double> GetDiameterAndAverageDistance();
/**
 * @name GetAverageDistanceByDjikstra - compute average distance of a
 * given graph by Djikstra algorithm
 * @return double
 */
      double GetAverageDistanceByDjikstra();
/**
 * @name GetAverageDistance - compute average distance of a given
 * graph by default SPFA algorithm
 * @return double
 */
      double GetAverageDistance();
/**
 * @name GetGlobalEfficiency - return the global efficiency of a given
 * network by default SPFA algorithm. This measurement quantifies
 * the efficiency of the network in sending information between vertices, assuming
 * that the efficiency for sending information between two vertices i and j is proportional
 * to the reciprocal of their distance. 
 * @return double
 * @ref 2001_PhysRevLett_V.Latora_Efficient Behavior of Small-World Networks
 */
      double GetGlobalEfficiency();
/**
 * @name GetVulnerability - compute the vulnerability of whole network
 * or a single node. the vulnerability of a vertex can be defined as
 * the drop in global efficiency when the vertex and all its edges are 
 * removed from the network
 * @param indexOfNode -  when this argument is absent then this
 * function compute the vulnerability of whole network, else compute
 * the vulnerability of a node indexed by indexOfNode
 * @return double
 * @ref 2005_PhysRevE_V.Latora_Vulnerability and protection of infrastructure networks
 */
      double GetVulnerability(size_t indexOfNode = scn::UGraph::NaF);
/**
 * @name GetTransitivity - Get the transitivity of a given
 * network. The transitivity is the first kind of clustering
 * coefficient based on undirected unweighted network.
 * @return double
 * @ref 2001_PhysRevE_M.E.J.Newman_Scientific collaboration
 * networks. I. Network construction and fundamental results
 */
      double GetTransitivity();
/**
 * @name GetClusteringCoefficient - Get the clustering coefficient of
 * a network.
 * @param indexOfNode -  return the clustering coefficient of specified vertex 
 * @param - return the average clustering coefficient of every vertex
 * in network
 * @return double
 */
      double GetClusteringCoeff(size_t indexOfNode = scn::UGraph::NaF);
/**
 * @name GetClusteringCoeffDist - get the distribution of clustering
 * coefficient in the network
 * @param slide - the width of beam in the distribution. If this value
 * is equal to 0, there is no re-sampling operation
 * @return DVector
 */
      DVector<double, double> GetClusteringCoeffDist(double slide = 0);
/**
 * @name GetCyclicCoeff - compute the cyclic coefficient of network
 * @param indexOfNode -  get the cyclic coefficient of the vertex
 * @param  -  return the average cyclic coefficient of all verteices
 * in network
 * @return double
 */
      double GetCyclicCoeff(size_t indexOfNode = scn::UGraph::NaF);
/**
 * @name GetRichClubCoeff - compute the rich-club coefficient of
 * degree k of a given network
 * @param degree -  input degree 
 * @return double
 */
      double GetRichClubCoeff(size_t degree);
/**
 * @name GetPSDegreeCorrelation - get the list of degree correlation
 * proposed by Pastor-Satorras
 * @return DVector - list of pair<degree, degree-correlation>
 * @ref 2001+PhysRevLett+R.Pastor-Satorras+Dynamical and Correlation
 * Properties of the Internet
 */
      DVector<size_t, double> GetPSDegreeCorrelation();
/**
 * @name GetClusterDegreeCorre - compute the clustering-degree
 * correlation of the network
 * @return DVector: list of pair<degree, local clustering coefficient>
 */
/**
 * @name GetClusteringDegreeCorre - compute the clustering-degree
 * correlation and the global degree-correlation based clustering
 * coefficient of the network
 * @return pair<global clustering coefficient, list of pair<degree,
 * local clustering coefficient>
 */
      pair<double, DVector<size_t, double>> GetClusteringDegreeCorre();
/**
 * @name GetPearsonCorrCoeff - compute the Pearson correlation
 * coefficient of the given network. When this coefficient is
 * positive, the network is assortative, when it is negative, the
 * network is disassortative
 * @return double
 */
      double GetPearsonCorrCoeff();
/**
 * @name GetSearchInfo - compute search information to locate vertex
 * 'indexOfTarget' from vertex 'indexOfSource' 
 * @param indexOfSource -  index Of Source 
 * @param indexOfTarget -  index Of Target 
 * @return double
 * @ref 2005_PhysRevLett_M.Rosvall_Networks and Cities An information Perspective
 */
      double GetSearchInfo(size_t indexOfSource, size_t indexOfTarget);
/**
 * @name GetAverageSearchInfo - compute average search information
 * through all pairs of nodes in a network
 * @return double
 * @ref 2005_PhysRevLett_M.Rosvall_Networks and Cities An information Perspective
 */
      double GetAverageSearchInfo();    
/**
 * @name GetAccessInfo - compute how difficult it is to locate a
 * vertex in the network from the given node designated by indexOfNode
 * @param indexOfNode -  the given node
 * @return double
 */
      double GetAccessInfo(size_t indexOfNode);
/**
 * @name GetHideInfo - compute how difficult it is to find the given
 * node specified by indexOfNode from other vertices in the network
 * @param indexOfNode -  index Of Node 
 * @return double
 */
      double GetHideInfo(size_t indexOfNode);
/**
 * @name GetBetweennessCentrality - compute the betweenness centrality
 * of an given node
 * @param indexOfNode -  index Of Node 
 * @return double
 */
      double GetBetweennessCentrality(size_t indexOfNode);
/**
 * @name FindClosureGroup - find a community in which the shortest length of
 * any pairs of nodes are less than "length"
 * @param indexOfSeed -  the found community contains this node 
 * @param length -  length 
 * @return IndexList - the node list of this community
 */
      IndexList FindClosureGroup(size_t indexOfSeed, size_t length);
/**
 * @name RandomWalkByURW - unrestricted random walk
 * @param indexOfSource - index Of Source 
 * @param indexOfTarget - index Of Target 
 * @return size_t - number of steps of this random walk
 */
      size_t RandomWalkByURW(size_t indexOfSource, size_t indexOfTarget);
/**
 * @name RandomWalkByNRRW - no-retracting random walk
 * @param indexOfSource - index Of Source 
 * @param indexOfTarget - of index Of Target 
 * @return size_t - number of steps of this random walk
 */
      size_t RandomWalkByNRRW(size_t indexOfSource, size_t indexOfTarget);
/**
 * @name RandomWalkBySARW - self-avoiding random walk
 * @param indexOfSource - Number of index Of Source 
 * @param indexOfTarget - Number of index Of Target 
 * @return size_t
 */
      size_t RandomWalkBySARW(size_t indexOfSource, size_t indexOfTarget);
/**
 * @name MFPTByRandomWalk - Mean First-Passage Time by random walk
 * @return size_t
 */
      size_t MFPTofRandomWalk();
/**
 * @name MRTofRandomWalk - Mean Return-time by a random walker. the
 * step the walker returns to a node first since it left the node. 
 * @return size_t
 */
      size_t MRTofRandomWalk();

   private://private operations
 /**
 * @name RunDjikstra - single source shorest path of Djikstra
 * @param indexOfSource -  index Of Source 
 * @return void
 */
      void RunDjikstra(size_t indexOfSource);
/**
 * @name RunSPFA - single source, shorest path of SPFA 
 * @param indexOfSource -  index Of Source 
 * @return void
 */
      void RunSPFA(size_t indexOfSource, Map<size_t> &distance);
      void RunSPFA(size_t indexOfSource);
/**
 * @name GetNumberOfShortestPath - compute the number of shorest path
 * from indexOfSource to indexOfTarget that passing through the indexOfThrough
 * @param indexOfSource -  index Of Source 
 * @param indexOfTarget -  index Of Target 
 * @param indexOfThrough -  index Of Through 
 * @param  -  if this argument is empty, this function just compute
 * the number of shorest path without passing any specified node
 * @return std::pair<number of shortest path from source to target,
 * number of shortest path that passing through the indexOfThrough>,
 * if the third argument is absent, the second returned value is same
 * to the first one
 */
      std::pair<size_t, size_t> GetNumberOfShortestPath(size_t indexOfSource, size_t indexOfTarget,
							size_t indexOfThrough = UGraph::NaF);
   protected://data
      scn::UGraph::pGraph graph;/**<  graph to be measured */
      scn::Map<size_t> distance_sssp;/**<used in single-source
				     * shortest path */
   };
};
