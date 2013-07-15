#pragma once
#include "network_global.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <memory>
#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <matrix.hpp>
#include <array>
#include <sstream>


namespace scn //short for Su' Complex Networks
{
   extern "C++" class Graph;
   extern "C++" class UGraph;
   extern "C++" class DGraph;
//************************************************************
//public structures or typename
//************************************************************
   template<typename Type> using Map = std::unordered_map<Type, Type>;
   template<typename Type> using pMap = std::shared_ptr<Map<Type>>;
   
   template<typename Type> using pVector = std::shared_ptr<std::vector<Type>>;

   template<typename Type1, typename Type2> 
   using DVector = std::vector<std::pair<Type1, Type2>>;

   enum{NaF = size_t(-1)};

   typedef std::vector<size_t>     IndexList;
   typedef std::vector<std::shared_ptr<UGraph>> UGraphList;
   typedef std::shared_ptr<UGraphList>          pUGraphList; 

//************************************************************
//Definition of Graphs
//************************************************************

   template<class NodeType>
   class _node_iterator : public std::unordered_map<size_t, NodeType>::iterator
   {
   public:
      typedef NodeType   value_type;
      typedef size_t     reference;//dereference for the index of node
      typedef NodeType*  pointer;
   public:
      _node_iterator(){;}
      
      _node_iterator(typename std::unordered_map<size_t, NodeType>::iterator &&iter)
	 :iterator_base(iter){;}
   
      inline reference operator*() const
      {return iterator_base->second.GetIndexOfNode();}
      
      inline pointer operator->() const
      {return std::addressof(iterator_base->second);}
   
      inline _node_iterator& operator++()
      {
	 iterator_base++;
	 return *this;
      }
      
      inline _node_iterator& operator++(int)
      {
	 iterator_base++;
	 return *this;
      }

      inline _node_iterator operator+(int count)
      {
	 _node_iterator temp = *this;
	 for(int i = 0; i < count; i++)
	    temp.iterator_base++;
	 return temp;
      }
      
      inline bool operator==(const _node_iterator& other) const
      {
	 return iterator_base == other.iterator_base;
      }

      inline bool operator!=(const _node_iterator& other) const
      {
	 return iterator_base != other.iterator_base;
      }

      inline _node_iterator& operator=(const _node_iterator& other)
      {
	 iterator_base = other.iterator_base;
	 return *this;
      }
   protected:
      typename std::unordered_map<size_t, NodeType>::iterator iterator_base;
   };
//////////////////////////////////////////////////////////////
//////////////////////////

   class _node
   {
   public:
      typedef std::unordered_set<size_t>::const_iterator  inout_iterator;
   public:
      class iterator : public std::unordered_set<size_t>::iterator
      {
      public:
	 typedef size_t   value_type;
	 typedef const size_t&  reference;
	 typedef const size_t*  pointer;
	 typedef std::unordered_set<size_t>::iterator _base_type;
      public:
	 iterator(){}
	 
	 iterator(_base_type &&in_degree_begin)
	    :current(in_degree_begin){}
	 
	 inline reference operator*() const
	 {return reference(*current);}
	 
	 inline pointer operator->() const
	 {return pointer(*current);}
	 
	 inline iterator& operator++()
	 {
	    current++;
	    return *this;
	 }
	 inline iterator operator+(int count)
	 {
	    iterator other = *this;
	    for(int i = 0; i < count; i++)
	    {
	       other.current++;
	    }
	    return other;
	 }
	 inline iterator& operator++(int)
	 {
	    current++;
	    return *this;
	 }
	 
	 inline bool operator==(const iterator& other) const
	 { return current == other.current;}
	 
	 inline bool operator!=(const iterator& other) const
	 {  return current != other.current;}
	 
	 inline iterator& operator=(const iterator& other)
	 {
	    current = other.current;
	    return *this;
	 }
      protected:
	 _base_type current;
      };      
   public:
      _node(size_t indexOfNode = -1) : index(indexOfNode){;}
      ~_node(){;}
   public:
      inline _node::iterator begin()
      {
	 return _node::iterator(in_degree.begin());
      }
  
      inline _node::inout_iterator in_begin()
      {return in_degree.begin(); }
      
      inline _node::inout_iterator out_begin()
      {return out_degree.begin(); }
      
      inline _node::iterator end()
      {return _node::iterator(out_degree.end());}
      
      inline _node::inout_iterator in_end()
      {return in_degree.end(); }
      
      inline _node::inout_iterator out_end()
      {return out_degree.end(); }
   public:
      inline size_t GetDegree(){return in_degree.size() + out_degree.size();}
      inline size_t GetInDegree(){return in_degree.size();}
      inline size_t GetOutDegree(){return out_degree.size();}
      inline size_t GetIndexOfNode(){return index;}
   protected:
      friend class Graph;
      friend class UGraph;
      friend class DGraph;
      size_t index;
      std::unordered_set<size_t> in_degree;/**<in_degree set form a
					    * complete endge sets
					    * with out_degree in DGraph */
      std::unordered_set<size_t> out_degree;/**<In UGraph, these
					     * out_degree has no meanings */
   };
//////////////////////////////////////////////////////////////
//////////////////////////

   class Graph
   {
   public://constant numbers
      enum {NaF = size_t(-1)};
      //static constexpr size_t NaF = -1;
      typedef _node_iterator<_node>   iterator;
      typedef std::shared_ptr<Graph>  pGraph;
   public://constructor and destructor
      
      Graph(){};

      Graph(size_t numberOfNodes)
      {
	 Create(numberOfNodes);
      };
      
      ~Graph(){};

   public://public operation
      
      inline void Create(size_t numberOfNodes)
      {
	 reserve(numberOfNodes);
	 for(size_t i = 0; i < numberOfNodes; i++)
	 {
	    AddNode();
	 }
      };

      inline IndexList CopyIndexOfNodes()
      {
	 IndexList list;
	 list.reserve(GetNumberOfNodes());
	 for(auto iter = node_list.begin(); iter != node_list.end(); iter++)
	 {
	    list.push_back(iter->first);
	 }
	 return list;
      }

      inline bool HasNode(size_t index)
      {
	 if(node_list.find(index) == node_list.end())
	    return false;
	 else
	    return true;
      }

      inline Graph::iterator AddNode(size_t index)
      {
	 assert(!HasNode(index));
	 return iterator(node_list.insert(std::make_pair(index, _node(index))).first);
      }

      inline size_t AddNode()
      {
	 assert(!HasNode(node_list.size()));
	 size_t index = node_list.size();
	 node_list[index].index = index;
	 return index;
      }
/**
 * @name RemoveNode - Directly remove a node and all associated edges.
 * @param index -  index of node
 * @return std::pair<the list of complentary nodes of in edges, 
 * the list of complentary nodes of out edges>. If remove a node in
 * undirected graph, please use: pair<list, std::ignore> to receive
 * the result of this function
 */
      inline std::pair<IndexList, IndexList>
	 RemoveNode(size_t index)
      { 
	 assert(HasNode(index));
	 auto iter = node_list.find(index);
	 IndexList in, out;
	 
	 in.assign(iter->second.in_degree.begin(), iter->second.in_degree.end());
	 out.assign(iter->second.out_degree.begin(), iter->second.out_degree.end());
	
	 for(auto other = iter->second.begin();
	     other != iter->second.end(); other++)
	 {
	    node_list[*other].in_degree.erase(index);
	    node_list[*other].out_degree.erase(index);
	 }
	 
	 node_list.erase(iter);
	 return std::make_pair(std::move(in), std::move(out));
      }

      inline size_t GetNumberOfEdges()
      {
	 size_t sum = 0;
	 for(auto iter = begin(); iter != end(); iter++)
	 {
	    sum += iter->GetDegree();
	 }
	 return sum / 2;
      }
      inline size_t GetDegree(size_t indexOfNode)
      {
	 assert(HasNode(indexOfNode));
	 return find(indexOfNode)->GetDegree();
      }
      inline size_t GetNumberOfNodes(){return node_list.size();}

   public://edge operations, all of them are abstract virtual function

      virtual bool AddEdge(size_t indexOfHead, size_t indexOfTail)=0;

      virtual bool AddEdge(size_t indexOfHead, IndexList& list)=0;

      virtual bool AddEdge(iterator head, size_t indexOfTail)=0;

      virtual bool AddEdge(iterator head, IndexList& list)=0;

      virtual bool AddEdge(iterator head, iterator tail) = 0;

      virtual bool RemoveEdge(size_t indexOfHead, size_t indexOfTail)=0;

      virtual bool RemoveEdge(iterator head, size_t indexOfTail)=0;
      
      virtual bool RemoveEdge(iterator head, iterator tail)=0;

      virtual bool HasEdge(size_t indexOfHead, size_t indexOfTail)=0;
      
      virtual bool HasEdge(iterator head, size_t indexOfTail)=0;

      virtual bool HasEdge(iterator head, iterator tail)=0;

   public://STL pattern
      
      void reserve(size_t size){node_list.reserve(size);}

      void clear(){node_list.clear();}
/**
 * @name size - get the number of nodes and edges. If not all these
 * two data are needed, it's better to call GetNumberOf...() instead
 * of this function
 * @return std::pair(number of nodes, number of edges)
 */
      std::pair<size_t, size_t> size()
      {
	 size_t sum = 0;
	 for(auto iter = begin(); iter != end(); iter++)
	 {
	    sum += iter->GetDegree();
	 }
	 return std::make_pair(node_list.size(), sum / 2);
      }

      inline Graph::iterator begin(){return iterator(node_list.begin());}
      
      inline Graph::iterator end() {return iterator(node_list.end());}

      inline Graph::iterator find(size_t index){return iterator(node_list.find(index));}

      inline Graph::iterator operator[](size_t index)
      {
	 assert(node_list.find(index) != node_list.end());
	 return iterator(node_list.find(index));
      }

      inline bool empty(){return node_list.empty();}
   protected:
      std::unordered_map<size_t, _node> node_list;
   };
//////////////////////////////////////////////////////////////
//////////////////////////

   class UGraph : public Graph
   {
   public:
      typedef std::shared_ptr<UGraph>  pGraph;
/**
 * @name UGraph - default constructor
 * @return int
 */
      UGraph(){}

      UGraph(size_t numberOfNodes):Graph(numberOfNodes){}

      UGraph(UGraph &other)
      {
	 //add node
	 for(auto node = other.begin(); node != other.end(); node++)
	 {
	    AddNode(*node);
	 }
	 //add edge
	 for(auto node = other.begin(); node != other.end(); node++)
	 {
	    for(auto tail = node->begin(); tail != node->end(); tail++)
	    {
	       AddEdge(*node, *tail);
	    }
	 }
      }
/**
 * @name UGraph - destructor
 * @return void
 */
      ~UGraph(){}

   public://implementation of virtual function

      inline virtual bool AddEdge(size_t indexOfHead, size_t indexOfTail)
      {
          assert(HasNode(indexOfHead));
          assert(HasNode(indexOfTail));
          assert(indexOfHead != indexOfTail);

          node_list[indexOfHead].in_degree.insert(indexOfTail);
          node_list[indexOfTail].in_degree.insert(indexOfHead);
          return true;
      }

      inline virtual bool AddEdge(size_t indexOfHead, IndexList& list)
      {
	 assert(HasNode(indexOfHead));
	 for(auto tail = list.begin(); tail != list.end(); tail++)
	 {
	    assert(HasNode(*tail));
	    assert(indexOfHead != *tail);
	    
	    node_list[indexOfHead].in_degree.insert(*tail);
	    node_list[*tail].in_degree.insert(indexOfHead);
	 }
	 return true;
      }

      inline virtual bool AddEdge(iterator head, size_t indexOfTail)
      {
	 assert(HasNode(indexOfTail));
	 assert(*head != indexOfTail);
	 
	 head->in_degree.insert(indexOfTail);
	 node_list[indexOfTail].in_degree.insert(*head);
	 return true;
      }

      inline virtual bool AddEdge(iterator head, IndexList& list)
      {
	 for(auto tail = list.begin(); tail != list.end(); tail++)
	 {
	    assert(HasNode(*tail));
	    assert(*head != *tail);
	    
	    head->in_degree.insert(*tail);
	    node_list[*tail].in_degree.insert(*head);
	 }
	 return true;
      }

      inline virtual bool AddEdge(iterator head, iterator tail)
      {
	 assert(head != tail);
	 
	 head->in_degree.insert(*tail);
	 tail->in_degree.insert(*head);

	 return true;
      }


      inline virtual bool RemoveEdge(size_t indexOfHead, size_t indexOfTail)
      {
	 assert(HasNode(indexOfHead));
	 assert(HasNode(indexOfTail));
	 assert(indexOfHead != indexOfTail);
	 
	 node_list[indexOfHead].in_degree.erase(indexOfTail);
	 node_list[indexOfTail].in_degree.erase(indexOfHead);
	 return true;
      }

      inline virtual bool RemoveEdge(iterator head, size_t indexOfTail)
      {
	 assert(HasNode(indexOfTail));
	 
	 head->in_degree.erase(indexOfTail);
	 node_list[indexOfTail].in_degree.erase(*head);
	 return true;
      }
      
      inline virtual bool RemoveEdge(iterator head, iterator tail)
      {
	 head->in_degree.erase(*tail);
	 tail->in_degree.erase(*head);

	 return true;
      }

      inline virtual bool HasEdge(size_t indexOfHead, size_t indexOfTail)
      {
	 assert(HasNode(indexOfHead));
	 assert(HasNode(indexOfTail));
	 if(node_list[indexOfHead].in_degree.find(indexOfTail) == 
	    node_list[indexOfHead].in_degree.end())
	    return false;
	 else
	    return true;
      }
      
      inline virtual bool HasEdge(iterator head, size_t indexOfTail)
      {
	 assert(HasNode(indexOfTail));
	 
	 if(head->in_degree.find(indexOfTail) == 
	    head->in_degree.end())
	    return false;
	 else
	    return true;
      }


      inline virtual bool HasEdge(iterator head, iterator tail)
      {
	 if(head->in_degree.find(*tail) == 
	    head->in_degree.end())
	    return false;
	 else
	    return true;
      }
      
   public://special operations

      Matrix GetAdjacencyMatrix()
      {
	 Matrix matrix(GetNumberOfNodes(), GetNumberOfNodes());
	 //write '1'
	 for(auto node = begin(); node != end(); node++)
	 {
	    for(auto other = node->begin(); other != node->end(); other++)
	    {
	       matrix(*node, *other)= 1.0;
	    }
	 }
	 return matrix;
      }

      void CreateFromAdjacencyMatrix(Matrix &matrix)
      {
	 assert(matrix.IsSymmetric());
	 clear();
	 Create(matrix.GetWidth());
	 for(size_t row = 0; row < matrix.GetHeight(); row++)
	 {
	    for(size_t col = row + 1; col < matrix.GetWidth(); col++)
	    {
	       if(abs(matrix(row, col) - 1.0) < 1e-3)
	       {
		  UGraph::AddEdge(row, col);
	       }
	    }
	 }
      }

      Matrix GetLaplacianMatrix()
      {
	 Matrix matrix = GetAdjacencyMatrix();
	 for(size_t i = 0; i < matrix.GetHeight(); i++)
	 {
	    matrix(i, i) = - valarray<double>(matrix.row(i)).sum();
	 }
	 return matrix;
      }
   };

//////////////////////////////////////////////////////////////
//////////////////////////

/** DGraph
 *
 * directed edge graph. The number of edges between any pair of nodes
 * is no more than 2.
 */

   class DGraph : public Graph
   {
   public:
      typedef std::shared_ptr<DGraph>  pGraph;

      DGraph(){}

      DGraph(size_t numberOfNodes):Graph(numberOfNodes){}
      
      ~DGraph(){}

   public://implementation of virtual function

      inline virtual bool AddEdge(size_t indexOfHead, size_t indexOfTail)
      {
          assert(HasNode(indexOfHead));
          assert(HasNode(indexOfTail));
          assert(indexOfHead != indexOfTail);

          node_list[indexOfHead].in_degree.insert(indexOfTail);
          node_list[indexOfTail].out_degree.insert(indexOfHead);
          return true;
      }
      
      inline virtual bool AddEdge(size_t indexOfHead, IndexList& list)
      {
          assert(HasNode(indexOfHead));
          for(auto tail = list.begin(); tail != list.end(); tail++)
          {
              assert(HasNode(*tail));
              assert(indexOfHead != *tail);

              node_list[indexOfHead].in_degree.insert(*tail);
              node_list[*tail].out_degree.insert(indexOfHead);
          }
          return true;
      }

      inline virtual bool AddEdge(iterator head, size_t indexOfTail)
      {
          assert(HasNode(indexOfTail));

          head->in_degree.insert(indexOfTail);
          node_list[indexOfTail].out_degree.insert(*head);

          return true;
      }

      inline virtual bool AddEdge(iterator head, IndexList& list)
      {

          for(auto tail = list.begin(); tail != list.end(); tail++)
          {
              assert(HasNode(*tail));

              head->in_degree.insert(*tail);
              node_list[*tail].out_degree.insert(*head);
          }

          return true;
      }


      inline virtual bool AddEdge(iterator head, iterator tail)
      {

          head->in_degree.insert(*tail);
          tail->out_degree.insert(*head);
          return true;
      }


      inline virtual bool RemoveEdge(size_t indexOfHead, size_t indexOfTail)
      {
          assert(HasNode(indexOfHead));
          assert(HasNode(indexOfTail));

          node_list[indexOfHead].in_degree.erase(indexOfTail);
          node_list[indexOfTail].out_degree.erase(indexOfTail);

          return true;
      }


      inline virtual bool RemoveEdge(iterator head, size_t indexOfTail)
      {
          assert(HasNode(indexOfTail));

          head->in_degree.erase(indexOfTail);
          node_list[indexOfTail].out_degree.erase(*head);
          return true;
      }

      inline virtual bool RemoveEdge(iterator head, iterator tail)
      {
          head->in_degree.erase(*tail);
          tail->out_degree.erase(*head);
          return true;
      }

      inline virtual bool HasEdge(size_t indexOfHead, size_t indexOfTail)
      {
          assert(HasNode(indexOfHead));
          assert(HasNode(indexOfTail));

          if(node_list[indexOfHead].in_degree.find(indexOfTail) ==
                  node_list[indexOfHead].in_degree.end())
              return false;
          else
              return true;
      }
      
      inline virtual bool HasEdge(iterator head, size_t indexOfTail)
      {
          assert(HasNode(indexOfTail));

          if(head->in_degree.find(indexOfTail) ==
                  head->in_degree.end())
              return false;
          else
              return true;
      }

      virtual bool HasEdge(iterator head, iterator tail)
      {
          if(head->in_degree.find(*tail) ==
                  head->in_degree.end())
              return false;
          else
              return true;
      }
   };
};
