#include <iostream>
#include <set>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "graphs.h"

Edge::Edge (int x, int y)
{
	if (x == y)
		y = -1; //if a loop is accidentally entered set one of the vertices to -1 to signify this

	edge.insert(x);
	edge.insert(y);
}

bool Edge::isLoop() 
{
	for (std::set<int>::iterator it = edge.begin(); it != edge.end(); ++it)
		{
			if ( *it == -1)
				return 1;
		}
	return 0;
}

	
Graph::Graph(int n)
{
	this->n = n;
	edge_count = 0;
	for (int i = 0; i < n; i++)
	{
		std::vector<int> temp;
		for (int j = 0; j < n; j++)
			temp.push_back(0);

		adj.push_back(temp);
	}
}

void Graph::printMatrix()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			std::cout << adj[i][j] << " ";
		std::cout << "\n";
	}
}

void Graph::addEdge(Edge new_edge)
{
	if (new_edge.isLoop())
		return;

	int i, j;
	int k = 0;
	for (std::set<int>::iterator it = new_edge.edge.begin(); it != new_edge.edge.end(); ++it)
		{
			if (k%2 == 0)
				i = *it;
			else
				j = *it;
			k++;
		}
	adj[i-1][j-1] = 1;
	adj[j-1][i-1] = 1;
	
	++edge_count;
}

bool Graph::hasEdge(int i, int j)
{
	if (adj[i-1][j-1] == 1)
		return 1;
	else
		return 0;
}

std::vector<int> Graph::connectedVertices(int vertex) //given a vertex on graph, returns a vector of connected vertices
{	
	std::vector<int> vertices;

	for (int i = 0; i < n; i ++)
	{
		if (adj[vertex - 1][i] == 1)
			vertices.push_back(i+1);
	}
	return vertices;
}

std::vector<int> Graph::vertexDegrees() //returns vector which contains degree of each of these vertices
{
	std::vector<int> degrees;
	
	for (int i = 0; i < n; i++)
	{
		int degree = 0;
		for (int j = 0; j < n; j++)
		{	
			if (adj[i][j] == 1)
				degree += 1;
		}
		
		degrees.push_back(degree);
	}
	
	std::sort(degrees.begin(), degrees.begin() + n);
	
	return degrees;
}
				

		
/*
Perhaps it may be easier to not use an edge class and simply pass the connected vertices as integers.
This is done by the alternative addEdge below

void Graph::addEdge(int i, int j)
{
	if (i == j)
		//no loops
		adj[i-1][j-1] = 0;
		adj[j-1][i-1] = 0;
	else
		adj[i-1][j-1] = 1;
		adj[j-1][i-1] = 1;	
}
*/	
int Graph::order()
{
	return n;
}

int Graph::edge_order()
{
	return edge_count;
}

void Graph::DFSsearch(int vertex, bool visited[])
{
	visited[vertex-1] = 1;
	std::vector<int> connects = connectedVertices(vertex);
	int i;
	for (i = 0; i < connects.size(); i++)
		if (!visited[connects[i]-1])
			DFSsearch(connects[i], visited);
}

bool Graph::isConnected(int vertex)
{
	bool *visited;
	visited = new bool[n];
	for (int k = 0; k<n; ++k)
		visited[k] = 0;

	DFSsearch(vertex, visited); 
	
	for (int j = 0; j < n; ++j)
		if (visited[j] == 0)
			return 0;

	delete [] visited;
	return 1;
}  


Graph generateRandomGraph(int n, int edges) //randomly generates a graph given number of vertices
{
	
	Graph randomGraph(n);
	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine generator (seed);
	std::uniform_int_distribution<int> dist(1, n);

	int edgeCount = 0;
	while (edgeCount < edges)
	{
		int i, j;
		i = dist(generator);
		j = dist(generator);
		Edge newEdge(i, j);

		if (!newEdge.isLoop() && !randomGraph.hasEdge(i, j))
		{
			randomGraph.addEdge(newEdge);
			++edgeCount;
		}
	}
	
	return randomGraph;
}
