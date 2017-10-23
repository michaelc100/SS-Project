#ifndef SPINGRAPHS_H
#define SPINGRAPHS_H

class Edge;
class Graph;
class Ising;

#include <set>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>

class Edge
{
    public:
	Edge(int, int);
	std::set<int> edge;
	bool isLoop();
};

class Graph
{
    private:
	int n;
	std::vector<std::vector<int>> adj;
	int edge_count;
    public:
	Graph(int);
	void printMatrix();
	void addEdge(Edge);
	bool hasEdge(int, int);
	std::vector<int> connectedVertices(int);
	std::vector<int> vertexDegrees();
	int order();
	int edge_order();
	void DFSsearch(int, bool*);
	bool isConnected(int);
};

class Ising
{
    public:
	Ising(int, bool randomize = false);
	std::vector<int> spins;
	void printSpins();
};


template <class Model>
class SpinGraph : public Graph, public Model
{
    public:
	SpinGraph(int, bool randomize = false);
	double actionCont(int);
	double action();
};


Graph generateRandomGraph(int, int);
SpinGraph<Ising> generateRandomIsingGraph(int, int, int randomize = true);

#include "SpinGraphs.cpp"

#endif

