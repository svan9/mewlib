/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_GRAPH_LIB_SO2U
#define MEW_GRAPH_LIB_SO2U
#include "mewlib.h"
#include "mewmath.hpp"
#include "mewstack.hpp"

namespace mew {
template<typename T, typename K>
class graph;

template<typename T, typename K>
class graph_edge {
private:
  const K _M_props;
  graph<T, K>* _M_v1;
  graph<T, K>* _M_v2;
public:
  ////////////////////////////////////////////////////////////
  graph_edge(const K& properties, graph<T, K>* vertice1, graph<T, K>* vertice2)
		: _M_props(properties)
		, _M_v1(vertice1)
		, _M_v2(vertice2) {}

  ////////////////////////////////////////////////////////////
  const graph<T, K>* getVertice1() const { return _M_v1; }

  ////////////////////////////////////////////////////////////
	const graph<T, K>* getVertice2() const { return _M_v2; }
  
  ////////////////////////////////////////////////////////////
	const K* getProperties() const { return &_M_props; }
};

template<typename T, typename K>
class graph {
public:
  typedef graph<T, K>* pointer_vert; 
  typedef graph_edge<T, K>* pointer_edge; 
private:
  const T _M_props;
  mew::stack<graph_edge<T, K>*, 8U> _M_edges;
public:
  ////////////////////////////////////////////////////////////
  graph(const T& props): _M_props(props) { }

  ////////////////////////////////////////////////////////////
  const T* props() const { return &_M_props; }

  ////////////////////////////////////////////////////////////
	const std::vector<graph_edge<T, K>*>* getEdges() const { return &_M_edges; }

  ////////////////////////////////////////////////////////////
	void addOrderedEdge(const K& properties, graph<T, K>* target) {
		graph_edge<T, K>* edge = new graph_edge<T, K>(properties, target, nullptr);
		_M_edges.push(edge);
	}

  ////////////////////////////////////////////////////////////
	void addEdge(const K& properties, graph<T, K>* target) {
		graph_edge<T, K>* edge = new graph_edge<T, K>(properties, target, this);
		_M_edges.push(edge);
		target->_M_edges.push(edge);
	}

  ////////////////////////////////////////////////////////////
  bool search(const pointer_vert vertice, const K& prop, mew::stack<const pointer_vert>* visited, int* cost) {
	if (visited.indexOf(vertice) != (size_t)(-1)) {
		return false;
	}
	visited->push(vertice);
	if (*vertice->pops() == prop) {
		return true;
	}
	for (const Edge<std::string, int>* edge : *vertice->getEdges()) {
		const Vertice<std::string, int>* next = edge->getVertice1() == vertice || edge->getVertice1() == nullptr ? edge->getVertice2() : edge->getVertice1();
		if (search(next, name, visited, cost)) {
			*cost += *edge->getProperties();
			return true;
		}
	}
	visited->pop_back();
	return false;
}
};

}

#endif