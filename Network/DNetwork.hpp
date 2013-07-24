#pragma once
#include <unordered_map>
#include <unordered_set>
#include <memory>
namespace scn
{
using namespace std;
constexpr size_t NaF = -1;

template<class NodeData = size_t, class EdgeData = size_t>
class DNetwork
{
public:
    typedef _node_iterator iterator;

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

    class _node
    {
    public:
        typedef _edge_iterator in_iterator;
        typedef _edge_iterator out_iterator;
        typedef _edge_undirected_iterator u_itertor;
    public:
        _node(){;}
        ~_node(){;}
    protected:
        std::unordered_map<size_t, EdgeData> in_degree;
        std::unordered_map<size_t, EdgeData> out_degree;
        NodeData  node_data;
    };

    class _node_iterator
    {
    public:
        typedef DNetwork::_node value_type;
        typedef size_t  reference;
        typedef NodeData* pointer;
    public:
        _node_iterator(){;}

        _node_iterator(typename std::unordered_map<size_t, _node>::iterator &&iter)
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

        inline _node::in_iterator in_begin()
        {return _node::in_iterator(in_degree.begin());}

        inline _node::in_iterator in_end()
        {return _node::in_iterator(in_degree.end());}

        inline _node::out_iterator out_begin()
        {return _node::out_iterator(out_degree.begin());}

        inline _node::out_iterator out_end()
        {return _node::out_iterator(out_degree.end());}

        inline size_t GetDegree(){return in_degree.size() + out_degree.size();}
        inline size_t GetInDegree(){return in_degree.size();}
        inline size_t GetOutDegree(){return out_degree.size();}

    protected:
        typename std::unordered_map<size_t, _node>::iterator current;
    };

public://STL way to access network
    inline iterator begin()
    {return iterator(node_list.begin());}

    inline iterator end()
    {return iterator(node_list.end());}

    inline iterator find(size_t indexOfNode)
    {return iterator(node_list.find(indexOfNode));}

protected:
    std::unordered_map<size_t, _node> node_list;
};
}
