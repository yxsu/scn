#pragma once
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <memory>
namespace scn
{
using namespace std;
constexpr size_t NaF = -1;

   template<class EdgeData>
   class _edge_iterator
   {
   public:
      typedef EdgeData    value_type;
      typedef size_t      reference;
      typedef EdgeData*   pointer;
   public:
      _edge_iterator(){;}
      
      _edge_iterator(typename std::unordered_map<size_t, EdgeData>::iterator &&iter)
	 :iterator_base(iter){;}
      
      inline reference operator*() const
      {return iterator_base->first;}
      
      inline pointer operator->() const
      {return std::addressof(iterator_base->second);}

      inline _edge_iterator& operator++()
      {
	 iterator_base++;
	 return *this;
      }

      inline _edge_iterator& operator++(int)
      {
	 iterator_base++;
	 return *this;
      }

      inline _edge_iterator operator+(int count)
      {
	 _edge_iterator temp = *this;
	 for(int i = 0; i < count; i++)
	    temp.iterator_base++;
	 return temp;
      }
      
      inline bool operator==(const _edge_iterator& other) const
      {return iterator_base == other.iterator_base;}
      
      inline bool operator!=(const _edge_iterator& other) const
      {return iterator_base != other.iterator_base;}
      
      inline _edge_iterator& operator=(const _edge_iterator& other)
      {
	 iterator_base = other.iterator_base;
	 return *this;
      }

   protected:
      typename std::unordered_map<size_t, EdgeData>::iterator iterator_base;
   };
   //provide a way to access undirected network
   class _edge_undirected_iterator
   {
      
   };
   
   template<class NodeData, class EdgeData>
   class _node
   {
   public:
      typedef _edge_iterator<EdgeData> in_iterator;
      typedef _edge_iterator<EdgeData> out_iterator;
      typedef _edge_undirected_iterator u_itertor;
   public:
      _node(){;}
      _node(NodeData& node)
	 :node_data(node){;}
      ~_node(){;}
   protected:
      std::unordered_map<size_t, EdgeData> in_degree;
      std::unordered_map<size_t, EdgeData> out_degree;
      NodeData  node_data;
   };

   template<class NodeData, class EdgeData>
   class _node_iterator
   {
   public:
      typedef _node<NodeData, EdgeData> value_type;
      typedef size_t  reference;
      typedef NodeData* pointer;
   public:
      _node_iterator(){;}

      _node_iterator(typename std::unordered_map<size_t, _node<NodeData, EdgeData>>::iterator &&iter)
	 :current(iter)
      {;}
      
      inline reference operator*() const
      {return current->first;}
      
      inline pointer operator->()
      {return std::addressof(current->second.node_data);}
      
      inline _node_iterator& operator++()
      {
	 current++;
	 return *this;
      }
      
      inline _node_iterator& operator++(int)
      {
	 current++;
	 return *this;
      }

      inline _node_iterator operator+(int count)
      {
	 _node_iterator temp = *this;
	 for(int i = 0; i < count; i++)
	    temp.current++;
	 return temp;
      }

      inline bool operator==(const _node_iterator& other) const
      {return current == other.current;}

      inline bool operator!=(const _node_iterator& other) const
      {return current != other.current;}

      inline _node_iterator& operator=(const _node_iterator& other)
      {
	 current = other.current;
	 return *this;
      }

   public://for edge access
      
      inline typename _node<NodeData, EdgeData>::in_iterator in_begin()
      {return _node<NodeData, EdgeData>::in_iterator(current->second.in_degree.begin());}
      
      inline typename _node<NodeData, EdgeData>::in_iterator in_end()
      {return _node<NodeData, EdgeData>::in_iterator(current->second.in_degree.end());}
      
      inline typename _node<NodeData, EdgeData>::out_iterator out_begin()
      {return _node<NodeData, EdgeData>::out_iterator(current->second.out_degree.begin());}

      inline typename _node<NodeData, EdgeData>::out_iterator out_end()
      {return _node<NodeData, EdgeData>::out_iterator(current->second.out_degree.end());}

      inline size_t GetDegree(){return current->second.in_degree.size() +
	    current->second.out_degree.size();}
      inline size_t GetInDegree(){return current->second.in_degree.size();}
      inline size_t GetOutDegree(){return current->second.out_degree.size();}
      
   protected:
      typename std::unordered_map<size_t, _node<NodeData, EdgeData>>::iterator current;
   };

//////////////////////////////////////////////////////////////////////
   
   template<class NodeData = size_t, class EdgeData = size_t>
   class DNetwork
   {
   public:
      DNetwork()
      {;}
   public:
      typedef _node_iterator<NodeData, EdgeData> iterator;
   public://STL way to access network
      inline iterator begin()
      {return iterator(node_list.begin());}
      
      inline iterator end()
      {return iterator(node_list.end());}
      
      inline iterator find(size_t indexOfNode)
      {return iterator(node_list.find(indexOfNode));}
      
   public:
      
      size_t AddNode(NodeData node = NodeData())
      {
	 size_t size = node_list.size();
	 node_list[size] = _node<NodeData, EdgeData>(node);
	 return size;
      }

      void AddNode(size_t indexOfNewNode, NodeData node = NodeData())
      {
	 assert(!HasNode(indexOfNewNode));
	 node_list[indexOfNewNode] = _node<NodeData, EdgeData>(node);
      }

      bool HasNode(size_t indexOfNode)
      {return node_list.find(indexOfNode) != node_list.end();}
      
   protected:
      std::unordered_map<size_t, _node<NodeData, EdgeData>> node_list;
};
}















