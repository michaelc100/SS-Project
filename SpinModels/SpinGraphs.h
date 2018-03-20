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
#include <queue>
#include <random>
#include <math.h>

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
	std::vector<std::vector<int>> adj_matrix;
	std::vector<std::vector<int>> adj_list;
	int edge_count;
    public:
	Graph(int);
	void printMatrix();
	void addEdge(Edge);
	bool hasEdge(int, int);
	void removeEdge(Edge);
	std::vector<int> connectedVertices(int);
	std::vector<int> vertexDegrees();
	int order();
	int edge_order();
	void DFSsearch(int, bool*);
	std::vector<int> BFSsearch(int);
	bool isConnected(int);
};

std::vector<int> dijkstra(Graph, int);

class Ising
{
    public:
	Ising(int, bool randomize = false);
	std::vector<double> spins;
	int getSpin(int);
	void printSpins();
};

class Potts
{
    public:
	Potts(int, int, bool randomize = false);
	std::vector<double> possSpins;
	std::vector<double> spins;
	int getSpin(int);
	void printSpins();
};

template <class Model>
class SpinGraph : public Graph, public Model
{
    public:
	SpinGraph(int, int q = 2, bool randomize = false);
	double magnetisation();
	double absMagnetisation();
	double actionCont(int , std::vector<double>);
	double action();
	bool update(double);
	void wolffUpdate(double);
};

template<> SpinGraph<Ising>::SpinGraph(int, int, bool); 
template<> SpinGraph<Potts>::SpinGraph(int, int, bool);
template<> double SpinGraph<Ising>::actionCont(int, std::vector<double>);
template<> double SpinGraph<Potts>::actionCont(int, std::vector<double>);

Graph generateSquareLattice(int);
Graph generateRandomGraph(int, int);
Graph generateParametrisedGraph(int, int); //implicitly conn 4 as we start with sqaure lattice
SpinGraph<Ising> generateSquareIsingGraph(int, int);
SpinGraph<Ising> generateRandomIsingGraph(int, int, bool randomize = true);
SpinGraph<Potts> generateRandomPottsGraph(int, int, int, bool randomize = true); //didn't use potts stuff and it caused me to write some sloppy template stuff so will probably just remove
SpinGraph<Ising> generateParametrisedIsingGraph(int, int, int, bool randomize = true);

//numerical analysis declarations
//really should write function templates for these
double variance(double, int);
double varianceVector(std::vector<int>); //really shouldnt need int parameter here, will remedy later
double mean(double, int);
double meanVector(std::vector<int>);
double jackKnifeMean(double, int);
double jackKnifeMean(int, int);
double jackKnifeVariance(double, int);
double jackKnifeVariance(double, int);

#include "SpinGraphs.cpp"

#endif


