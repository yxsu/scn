#pragma once
#include "graph.hpp"

namespace scn
{
/**
 * @name GenKNearestNetwork - Generates a K-Nearest network
 * @param numberOfNodes -  number Of Nodes 
 * @param k -  each node will connect with forward k nodes and
 * backward k nodes. the degree of each node is 2*k 
 * @return UGraph::pGraph return the generated network
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenKNearestNetwork(size_t numberOfNodes, size_t k);
/**
 * @name GenSmallWorldNetworkByWS - Generates WS small world network
 * @param numberOfNodes - Number of nodes
 * @param k - same with GenKNearestNetwork
 * @param probability - probability of re-connection 
 * @return UGraph::pGraph - the network
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenSmallWorldNetworkByWS(size_t numberOfNodes, size_t k,
					   double probability);
/**
 * @name GenSmallWorldNetworkByNW - Generates a NW small world network
 * @param numberOfNodes - Number of Nodes
 * @param k - same with GenKNearestNetwork and GenSmallWolrdNetworkByWS
 * @param probability - probability of additive connection
 * @return UGraph::pUGraph
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenSmallWorldNetworkByNW(size_t numberOfNodes, size_t k,
					    double probability);
/**
 * @name GenTreeStructuredSFSW - Generates a scale-free small-world
 * network based on true structure
 * @param numberOfNodes -  number Of Nodes 
 * @return UGraph::pGraph
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenTreeStructuredSFSW(size_t numberOfNodes);
/**
 * @name GenTreeStructuredSW - Generates a deterministic small-world
 * network based on true structure
 * @param times -  iteration times 
 * @return UGraph::pGraph
 * @ref 2012+IEICE+S.Z.Guo+A Tree-Structured Deterministic Small-World Network
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenTreeStructuredSW(size_t times);
/**
 * @name GenRandomNetwork - Generates ER random network
 * @param numberOfNodes - Number of number Of Nodes 
 * @param probability - probability of connection 
 * @return UGraph::pGraph
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenRandomNetwork(size_t numberOfNodes, double probability);
/**
 * @name GenScaleFreeNetwork - Generate scale free or BA network
 * @param numberOfNodes - Number of number Of Nodes 
 * @param degree - Number of degree of new node
 * @return UGraph::pGraph
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenScaleFreeNetwork(size_t numberOfNodes, size_t degree);
/**
 * @name GenSwirlShapedSWNetwork - Generates a Swirl-Shaped
 * deterministic network
 * @param times -  iteration times
 * @return UGraph::pGraph
 * @ref 2011+ITJ+C.Li+A Swirl-shaped Deterministic Network with High
 * Clustering Coefficient
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenSwirlShapedNetwork(size_t times);
/**
 * @name GenPinWheelShapedSW - Generates a PinWheel-Shaped small-world network
 * @param times -  iteration times
 * @return UGraph::pGraph
 * @ref 2012+ICICEL+S.Z.Guo+A PinWheel-shaped Deterministic
 * small-world network
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenPinWheelShapedSW(size_t times);
/**
 * @name GenSmallWorldByEdgeIteration - Generates a deterministic
 * small-world network by edge iteration.
 * @param times -  times for iteration
 * @return UGraph::pGraph
 * @ref 2006_Z.Zhang_A deterministic small-world network created by edge iterations
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenSmallWorldByEdgeIteration(size_t times);
/**
 * @name GenUniformRecursiveTree - Generates a Uniform Recursive Tree(URT)
 * @param numberOfNodes - number of nodes
 * @return UGraph::pGraph
 * @ref 1995_R.T.Smythe_A survey of recursive trees
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenUniformRecursiveTree(size_t numberOfNodes);
/**
 * @name GenDURT - Generates a Deterministic Uniform Recursive Tree 
 * @param times -  times of iteration
 * @return UGraph::pGraph
 * @ref 2008_Z.Zhang_Topologies and Laplacian spectra of a
 * deterministic uniform recursive tree
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenDURT(size_t times);
/**
 * @name GenSmallWorldNetworkFromDURT - Generates a deterministic
 * small-world network from a Deterministic Uniform Recursive Tree
 * @param times -  times of iteration
 * @return UGraph::pGraph
 * @ref 2012_Z.M.Lu_A small-world network derived from the
 * deterministic uniform recursive tree
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenSmallWorldNetworkFromDURT(size_t times);
/**
 * @name GenCommunityNetwork - Generate the simplest community network
 * @param numberOfNodes -  number Of Nodes 
 * @param numberOfCommunity -  number Of Community 
 * @param inner_prob - probability for the linking within a single community
 * @param outer_prob - probability for the linking between two communities 
 * @return UGraph::pGraph
 */
   UGraph::pGraph NETWORKSHARED_EXPORT GenCommunityNetwork(size_t numberOfNodes, size_t numberOfCommunity,
				      double inner_prob, double outer_prob);
/**
 * @name GenNetworkFromDegreeDistribution - Generates a new string of
 * type p UGraph List 
 * @param degree_list - degree distribution. the first is number of
 * degree, the second is number of nodes
 * @return pUGraphList - return a list of graphs which fit the same
 * degree distribution
 */
   pUGraphList NETWORKSHARED_EXPORT GenNetworkFromDegreeDistribution(Map<size_t> &degree_list);
/**
 * @name IsDegreeListGraphical - test whether a given degree
 * distribution can construct a graph. This method is based on
 * Havel-Hakimi algorithm
 * @param degree_distribution - degree distribution 
 * @return bool - return true if this degre distribution can contruct
 * a connected graph.
 */
   bool NETWORKSHARED_EXPORT IsDegreeListGraphical(Map<size_t> &degree_distribution);
/**
 * @name RenormalizeByBoxCounting - a given undirected network is
 * renormalized by the method of box-counting described in Ref. This
 * method belongs to network clutering
 * @param graph -  graph, the given undirected network
 * @param length -  the diameter of all new box is less than this "length"
 * @return UGraph::pGraph - the renormalized undirected network
 * @ref 2005_Nature_C.Song_Self-similarity of complex networks
 */
   UGraph::pGraph NETWORKSHARED_EXPORT RenormalizeByBoxCounting(UGraph::pGraph graph, size_t length);
};
