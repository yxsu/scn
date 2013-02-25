#pragma once
#include "graph.hpp"

namespace scn
{
   ////////////////////////////////////////////////////////////////////////
////////////////////
   template<typename GraphType, typename NodeData, typename EdgeData>
   class Network
   {
   public:
      typedef NodeData* pNode;
      typedef EdgeData* pEdge;
   public:
/**
 * @name Network - Constructor with specified network topology. The
 * index in network topology must be less  than 2^32.
 * @param graph -  graph 
 * @return Network
 */
      Network(typename GraphType::pGraph &graph)
	 :graph(graph)
      {
	 if(graph->GetNumberOfNodes() != 0)
	 {
	    CreateCirclePosition(0.5, 0.5, 0.5);
	 }
      }

      ~Network(){}
      
      typename GraphType::pGraph GetTopology(){return graph;}

   public://operation about node
      
      void SetNodeData(typename GraphType::iterator &node, pNode &data)
      {
	 node_data[*node] = data;
      }

      void SetNodeData(size_t indexOfNode, pNode &data)
      {
	 assert(graph->HasNode(indexOfNode));
	 node_data[indexOfNode] = data;
      }

      pNode GetNodeData(typename GraphType::iterator &node)
      {
	 return node_data[*node];
      }

      pNode GetNodeData(size_t indexOfNode)
      {
	 assert(graph->HasNode(indexOfNode));
	 return node_data[indexOfNode];
      }
      
      void SetNodePosition(size_t indexOfNode, double x, double y, double z)
      {
	 assert(graph->HasNode(indexOfNode));
	 std::array<double, 3> temp;
	 temp[0] = x;
	 temp[1] = y;
	 temp[2] = z;
	 position[indexOfNode] = temp;
      }

      void SetNodePosition(size_t indexOfNode, std::array<double, 3> &position)
      {
	 assert(graph->HasNode(indexOfNode));
	 this->position[indexOfNode] = position;
      }

      std::array<double, 3> GetNodePosition(typename GraphType::iterator &node)
      {
	 return position[*node];
      }

      std::array<double, 3> GetNodePosition(size_t indexOfNode)
      {
	 assert(graph->HasNode(indexOfNode));
	 return position[indexOfNode];
      }
      
   public://operation about edge, all of them are pure virtual function
      
      virtual void SetEdgeData(typename GraphType::iterator head, size_t indexOfTail,
			       pEdge &data, bool reverse = false) = 0;

      virtual void SetEdgeData(size_t indexOfHead, size_t indexOfTail,
			       pEdge &data, bool reverse = false) = 0;
      
      
      virtual pEdge GetEdgeData(typename GraphType::iterator head, size_t indexOfTail,
				bool reverse = false) = 0;

      virtual pEdge GetEdgeData(size_t indexOfHead, size_t indexOfTail,
				bool reverse = false) = 0;

   public://network io

      virtual void WriteToNetFile(std::string path) = 0;
  
   protected:
      
      void CreateCirclePosition(double origin_x, double origin_y, double radius)
      {
	 double angle = 0;
	 double delta = 2 * 3.14 / graph->GetNumberOfNodes();
	 
	 for(auto node = graph->begin(); node != graph->end(); node++, angle+=delta)
	 {
	    std::array<double, 3> temp;
	    temp[0] = origin_x + radius * cos(angle);//x
	    temp[1] = origin_y + radius * sin(angle);//y
	    temp[2] = 0.5;
	    SetNodePosition(*node, temp);
	 }
      }
   protected:
      typename GraphType::pGraph graph;
      std::unordered_map<size_t, std::array<double, 3>> position;
      std::unordered_map<size_t, pNode> node_data;
      std::unordered_map<scn::uint64, pEdge> edge_data;
   };


//////////////////////////////////////////////////////////
/////////////

   template<typename NodeData = size_t, typename EdgeData = double>
   class UNetwork
      :public Network<UGraph, NodeData, EdgeData> 
   {
   public:
      typedef NodeData* pNode;
      typedef EdgeData* pEdge;
      typedef std::shared_ptr<UNetwork> pNetwork;
/**
 * @name UNetwork - Constructor with specified network topology. The
 * index in network topology must be less  than 2^32.
 * @param graph -  graph 
 * @return UNetwork
 */
      UNetwork(UGraph::pGraph graph)
	 :Network<UGraph, NodeData, EdgeData>(graph)
      {
      }

      ~UNetwork(){}
   public:
      
      virtual void SetEdgeData(UGraph::iterator head, size_t indexOfTail,
			       pEdge &data, bool reverse = false)
      {
	 assert(*head <= 0xffffffff);
	 assert(indexOfTail <= 0xffffffff);
	 assert(graph->HasNode(indexOfTail));
	 uint64 index;
	 if(*head < indexOfTail)
	 {
	    index = (*head) << 32;
	    edge_data[index + indexOfTail] = data;
	 }
	 else
	 {
	    index = indexOfTail << 32;
	    edge_data[index + *head] = data;
	 }
      }

      virtual void SetEdgeData(size_t indexOfHead, size_t indexOfTail,
			       pEdge &data, bool reverse = false)
      {
	 assert(indexOfHead <= 0xffffffff);
	 assert(indexOfTail <= 0xffffffff);
	 assert(graph->HasNode(indexOfHead));
	 assert(graph->HasNode(indexOfTail));

	 uint64 index;
	 if(indexOfHead < indexOfTail)
	 {
	    index = (indexOfHead) << 32;
	    edge_data[index + indexOfTail] = data;
	 }
	 else
	 {
	    index = indexOfTail << 32;
	    edge_data[index + indexOfHead] = data;
	 }
      }
      
      virtual pEdge GetEdgeData(UGraph::iterator head, size_t indexOfTail,
				bool reverse = false)
      {
	 assert(*head <= 0xffffffff);
	 assert(indexOfTail <= 0xffffffff);
	 assert(graph->HasNode(indexOfTail));
	 uint64 index;
	 if(*head < indexOfTail)
	 {
	    index = (*head) << 32;
	    return edge_data[index + indexOfTail];
	 }
	 else
	 {
	    index = indexOfTail << 32;
	    return edge_data[index + *head];
	 }
      }

      virtual pEdge GetEdgeData(size_t indexOfHead, size_t indexOfTail,
				bool reverse = false)
      {
	 assert(indexOfHead <= 0xffffffff);
	 assert(indexOfTail <= 0xffffffff);
	 assert(graph->HasNode(indexOfHead));
	 assert(graph->HasNode(indexOfTail));
	 uint64 index;
	 if(indexOfHead < indexOfTail)
	 {
	    index = (indexOfHead) << 32;
	    return edge_data[index + indexOfTail];
	 }
	 else
	 {
	    index = indexOfTail << 32;
	    return edge_data[index + indexOfHead];
	 }
      }

      virtual void WriteToNetFile(std::string path)
      {

      }

   protected:
      using Network<UGraph, NodeData, EdgeData>::graph;
      using Network<UGraph, NodeData, EdgeData>::position;
      using Network<UGraph, NodeData, EdgeData>::node_data;
      using Network<UGraph, NodeData, EdgeData>::edge_data;
   };

/////////////////////////////////////////////////////////////////
///////////
 template<typename NodeData = size_t, typename EdgeData = double>
   class DNetwork
      :public Network<DGraph, NodeData, EdgeData> 
   {
   public:
      typedef NodeData* pNode;
      typedef EdgeData* pEdge;
      typedef std::shared_ptr<DNetwork> pNetwork;
/**
 * @name DNetwork - Constructor with specified network topology. The
 * index in network topology must be less  than 2^32.
 * @param graph -  graph 
 * @return DNetwork
 */
      DNetwork(DGraph::pGraph graph)
	 :Network<DGraph, NodeData, EdgeData>(graph)
      {
      }

      ~DNetwork(){}
   public:
      
      virtual void SetEdgeData(DGraph::iterator head, size_t indexOfTail,
			       pEdge &data, bool reverse = false)
      {
	 assert(*head <= 0xffffffff);
	 assert(indexOfTail <= 0xffffffff);
	 assert(graph->HasNode(indexOfTail));
	 uint64 index;
	 if(reverse)
	 {
	    index = (*head) << 32;
	    edge_data[index + indexOfTail] = data;
	 }
	 else
	 {
	    index = indexOfTail << 32;
	    edge_data[index + *head] = data;
	 }
      }

      virtual void SetEdgeData(size_t indexOfHead, size_t indexOfTail,
			       pEdge &data, bool reverse = false)
      {
	 assert(indexOfHead <= 0xffffffff);
	 assert(indexOfTail <= 0xffffffff);
	 assert(graph->HasNode(indexOfHead));
	 assert(graph->HasNode(indexOfTail));

	 uint64 index;
	 if(reverse)
	 {
	    index = (indexOfHead) << 32;
	    edge_data[index + indexOfTail] = data;
	 }
	 else
	 {
	    index = indexOfTail << 32;
	    edge_data[index + indexOfHead] = data;
	 }
      }
      
      virtual pEdge GetEdgeData(DGraph::iterator head, size_t indexOfTail,
				bool reverse = false)
      {
	 assert(*head <= 0xffffffff);
	 assert(indexOfTail <= 0xffffffff);
	 assert(graph->HasNode(indexOfTail));
	 uint64 index;
	 if(reverse)
	 {
	    index = (*head) << 32;
	    return edge_data[index + indexOfTail];
	 }
	 else
	 {
	    index = indexOfTail << 32;
	    return edge_data[index + *head];
	 }
      }

      virtual pEdge GetEdgeData(size_t indexOfHead, size_t indexOfTail,
				bool reverse = false)
      {
	 assert(indexOfHead <= 0xffffffff);
	 assert(indexOfTail <= 0xffffffff);
	 assert(graph->HasNode(indexOfHead));
	 assert(graph->HasNode(indexOfTail));
	 uint64 index;
	 if(reverse)
	 {
	    index = (indexOfHead) << 32;
	    return edge_data[index + indexOfTail];
	 }
	 else
	 {
	    index = indexOfTail << 32;
	    return edge_data[index + indexOfHead];
	 }
      }

      virtual void WriteToNetFile(std::string path)
      {

      }
   protected:
      using Network<UGraph, NodeData, EdgeData>::graph;
      using Network<UGraph, NodeData, EdgeData>::position;
      using Network<UGraph, NodeData, EdgeData>::node_data;
      using Network<UGraph, NodeData, EdgeData>::edge_data;
   };


};
