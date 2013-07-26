#pragma once
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <memory>
namespace scn
{
   using namespace std;
   constexpr size_t NaF = -1;
   extern "C++" template<class NodeData, class EdgeData> class DNetwork;
   extern "C++" template<class NodeData, class EdgeData> class _node_iterator;
   template<class EdgeData>
   class _edge_iterator
   {
   public:
      typedef EdgeData    value_type;
      typedef size_t      reference;
      typedef EdgeData*   pointer;
   public:
      _edge_iterator(){;}
      
      _edge_iterator(typename std::unordered_map<size_t, shared_ptr<EdgeData>>::iterator &&iter)
	 :iterator_base(iter){;}
      
      inline reference operator*() const
      {return iterator_base->first;}
      
      inline pointer operator->() const
      {return iterator_base->second.get();}

      inline EdgeData& data()
      {return *(iterator_base->second);}

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
      typename std::unordered_map<size_t, shared_ptr<EdgeData>>::iterator iterator_base;
   };
   //provide a way to access undirected network
   class _edge_undirected_iterator
   {
      
   };

   template<class NodeData, class EdgeData>
   class _node
   {
   public:
      _node(){;}
      _node(NodeData& node)
	 :node_data(node){;}
      ~_node(){;}
   protected:
      friend class DNetwork<NodeData, EdgeData>;
      friend class _node_iterator<NodeData, EdgeData>;
      std::unordered_map<size_t, shared_ptr<EdgeData>> in_degree;
      std::unordered_map<size_t, shared_ptr<EdgeData>> out_degree;
      NodeData  node_data;
   };

   template<class NodeData, class EdgeData>
   class _node_iterator
   {
   public:
      typedef _edge_iterator<EdgeData> in_iterator;
      typedef _edge_iterator<EdgeData> out_iterator;
      typedef _edge_undirected_iterator u_itertor;
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

      inline NodeData& data()
      {return current->second.node_data;}
      
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
      
      inline in_iterator in_begin()
      {return in_iterator(current->second.in_degree.begin());}
      
      inline in_iterator in_end()
      {return in_iterator(current->second.in_degree.end());}
      
      inline out_iterator out_begin()
      {return out_iterator(current->second.out_degree.begin());}

      inline out_iterator out_end()
      {return out_iterator(current->second.out_degree.end());}

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

      inline pair<size_t, size_t> size()
      {
          size_t edge_count = 0;
          for(auto node = begin(); node != end(); node++)
              edge_count += node.GetDegree();
          return make_pair(node_list.size(), edge_count / 2);
      }
      
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

      void AddEdge(size_t indexOfHead, size_t indexOfTail, EdgeData edge = EdgeData(), bool reverse = false)
      {
	 auto head = node_list.find(indexOfHead);
	 auto tail = node_list.find(indexOfTail);
	 assert(head != node_list.end());
	 assert(tail != node_list.end());
	 shared_ptr<EdgeData> temp(new EdgeData(edge));
	 if(reverse)
	 {
	    head->second.out_degree.insert(make_pair(indexOfTail, temp));
	    tail->second.in_degree.insert(make_pair(indexOfHead, temp));
	 }
	 else
	 {
	    head->second.in_degree.insert(make_pair(indexOfTail, temp));
	    tail->second.out_degree.insert(make_pair(indexOfHead, temp));
	 }
      }

      bool HasEdge(size_t indexOfHead, size_t indexOfTail, bool reverse = false)
      {
	 auto head = node_list.find(indexOfHead);
	 auto tail = node_list.find(indexOfTail);
	 assert(head != node_list.end());
	 assert(tail != node_list.end());
	 if(reverse)
	    return head->second.out_degree.find(indexOfTail) != head->second.out_degree.end();
	 else
	    return tail->second.out_degree.find(indexOfHead) != tail->second.out_degree.end();
      }

      bool HasNode(size_t indexOfNode)
      {return node_list.find(indexOfNode) != node_list.end();}
   
   void RemoveEdge(size_t indexOfHead, size_t indexOfTail, bool reverse = false)
   {
      auto head = node_list.find(indexOfHead);
      auto tail = node_list.find(indexOfTail);
      assert(head != node_list.end());
      assert(tail != node_list.end());
      if(reverse)
      {
	 head->second.out_degree.erase(indexOfTail);
	 tail->second.in_degree.erase(indexOfHead);
      }
      else
      { 
	 head->second.in_degree.erase(indexOfTail);
	 tail->second.out_degree.erase(indexOfHead);
      }
   }

   void RemoveNode(size_t indexOfNode)
   {
      auto node = node_list.find(indexOfNode);
      assert(node != node_list.end());
//in-degree
      for(auto edge = node->second.in_degree.begin(); 
	  edge != node->second.in_degree.end(); edge++)
	 node_list[edge->first].out_degree.erase(indexOfNode);
      //out-degree
      for(auto edge = node->second.out_degree.begin();
	  edge != node->second.out_degree.end(); edge++)
	 node_list[edge->first].in_degree.erase(indexOfNode);
//erase node
      node_list.erase(node);
   }
   
      iterator operator()(size_t indexOfNode)
      {return find(indexOfNode);}

      _edge_iterator<EdgeData> operator()(size_t indexOfHead, size_t indexOfTail)
      {
	 assert(HasEdge(indexOfHead, indexOfTail));
	 return _edge_iterator<EdgeData>(node_list[indexOfHead].in_degree.find(indexOfTail));
      }
   protected:
      std::unordered_map<size_t, _node<NodeData, EdgeData>> node_list;
};
}
















