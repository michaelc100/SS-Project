#include <iostream>
#include <set>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <math.h>
#include "SpinGraphs.h"

const double pi = atan(1.0)*4; 

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

		adj_matrix.push_back(temp);
	}
	adj_list.reserve(n);
}

void Graph::printMatrix()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			std::cout << adj_matrix[i][j] << " ";
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
	adj_matrix[i-1][j-1] = 1;
	adj_matrix[j-1][i-1] = 1;
	
	adj_list[i-1].push_back(j);
	adj_list[j-1].push_back(i);
	
	++edge_count;
}

bool Graph::hasEdge(int i, int j)
{
	if (adj_matrix[i-1][j-1] == 1)
		return 1;
	else
		return 0;
}

void Graph::removeEdge(Edge del_edge)
{
	int i, j;	
	int k = 0;

	for (std::set<int>::iterator it = del_edge.edge.begin(); it != del_edge.edge.end(); ++it)
	{
		if (k%2 == 0)
			i = *it;
		else
			j = *it;
		k++;
	}

	if (Graph::hasEdge(i, j))
	{
		adj_matrix[i-1][j-1] = 0;
		adj_matrix[j-1][i-1] = 0;
	
		adj_list[i-1].erase(std::remove(adj_list[i-1].begin(), adj_list[i-1].end(), j), adj_list[i-1].end());
		adj_list[j-1].erase(std::remove(adj_list[j-1].begin(), adj_list[j-1].end(), i), adj_list[j-1].end());
	}
}

std::vector<int> Graph::connectedVertices(int vertex) //given a vertex on graph, returns a vector of connected vertices
{	
	/*
	std::vector<int> vertices;

	for (int i = 0; i < n; i ++)
	{
		if (adj_matrix[vertex - 1][i] == 1)
			vertices.push_back(i+1);
	}
	return vertices;
	*/

	return adj_list[vertex-1];
}

std::vector<int> Graph::vertexDegrees() //returns vector which contains degree of each of these vertices
{
	std::vector<int> degrees;
	
	for (int i = 1; i < n+1; i++)
	{
		int degree = 0;
		degree = connectedVertices(i).size();		
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
		adj_matrix[i-1][j-1] = 0;
		adj_matrix[j-1][i-1] = 0;
	else
		adj_matrix[i-1][j-1] = 1;
		adj_matrix[j-1][i-1] = 1;	
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

std::vector<int> Graph::BFSsearch(int vertex)//bfs the whole graph to return distances to all vertices
{
	int a = std::numeric_limits<int>::max();
	std::vector<int> distance(n, a); //set all distances to inf
	std::vector<bool> visited(n, false);
	std::queue<int> Q;
	Q.push(vertex);
	distance[vertex-1] = 0;
	visited[vertex-1] = true;
	while (!Q.empty())
	{
		int v = Q.front();
		Q.pop();
		std::vector<int> children = connectedVertices(v);
		for (int i = 0; i < children.size(); i++)
		{
			if (visited[children[i]-1] == false)	
			{	
				Q.push(children[i]);
				distance[children[i]-1] = distance[v-1] + 1;
				visited[children[i]-1] = true;
			}
		}
	}
	return distance;
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

Ising::Ising(int n, bool randomize)
{
	if (randomize == true)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator (seed);
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double prob = 0.5;
		for (int i = 0; i < n; i++)
		{
			double r = dist(generator);
			if (r < prob)
				spins.push_back(1.0);
			else
				spins.push_back(-1.0);
		}
	}

	else
	{
		for (int i = 0; i < n; i++)
			spins.push_back(1);
	}		
}

int Ising::getSpin(int vertex)
{
	return spins[vertex-1];
}

void Ising::printSpins()
{
	for (int i = 0; i < spins.size(); i++)
		std::cout << spins[i] << "\n";
}

Potts::Potts(int n, int q, bool randomize)
{
	
	for (int spin = 1; spin <= q; spin++)
	{
		double sval = cos(2*pi*spin/double(q));
		possSpins.push_back(sval);
	}

	if (randomize == true)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator (seed);
		std::uniform_int_distribution<int> dist(0, possSpins.size()-1);
		for (int i = 0; i < n; i++)
		{
			int assignedSpinIndex = dist(generator);
			spins.push_back(possSpins.at(assignedSpinIndex));
		}
	}
	
	else
	{
		for (int i = 0; i < n; i++)
			spins.push_back(1);
	}	
}

int Potts::getSpin(int vertex)
{
	return spins[vertex-1];
}

void Potts::printSpins()
{
	for (int i = 0; i < spins.size(); i++)
		std::cout << spins[i] << "\n";
}

	
template <>
SpinGraph<Ising>::SpinGraph(int n, int q, bool randomize):Graph(n), Ising(n, randomize)
{

}

template <>
SpinGraph<Potts>::SpinGraph(int n, int q, bool randomize):Graph(n), Potts(n, q, randomize)
{

}

/*if another model is used another actionCont will need to be defined

template<>
double SpinGraph<AnotherModel>::actionCont(int vertex)
{
}
*/

template<>
double SpinGraph<Ising>::actionCont(int vertex, std::vector<double> field)
{
	std::vector<int> nhbrs = connectedVertices(vertex);
	double action = 0.0;
	for (int i = 0; i < nhbrs.size(); i++)
	{	
		/*
		if (field[vertex - 1] != field[nhbrs[i] - 1])
			action += 0.5; 
		*/
		action += -field[vertex-1]*field[nhbrs[i]-1];//assuming here interaction constant J = 1
	}	
	return action;
}

template<>
double SpinGraph<Potts>::actionCont(int vertex, std::vector<double> field)
{
	std::vector<int> nhbrs = connectedVertices(vertex);
	double action = 0.0;
	for (int i = 0; i < nhbrs.size(); i++)
	{	
		if (field[vertex-1] == field[nhbrs[i]-1])
			action += -1;
	}	
	return action;
}

template <class Model>
double SpinGraph<Model>::magnetisation()
{
	double mag = 0.0;
	int size = order();
	for (int i = 0; i < size; i++)
		mag += Model::spins[i];
	return mag/size;
}

template <class Model>
double SpinGraph<Model>::absMagnetisation()
{
	double mag = magnetisation();
	return fabs(mag);
}

template <class Model>
double SpinGraph<Model>::action()
{
	double totalAction = 0.0;
	for (int i = 1; i <= order(); i++)
		totalAction += actionCont(i, Model::spins);
	return totalAction/2; //when calculated over the whole graph, must divide by two because of double counting
}

template <class Model>
bool SpinGraph<Model>::update(double beta)
{
	std::vector<double> temp = Model::spins;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine generator (seed);
	std::uniform_int_distribution<int> dist(1, order());
	int i = dist(generator); //dist(generator);
	temp[i-1] = -temp[i-1];
	
	double oldAction = actionCont(i, Model::spins);
	double newAction = actionCont(i, temp);
	if (oldAction > newAction)
	{
		Model::spins = temp;
		return 1;
	}

	else
	{
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		double r = dist(generator); //dist(generator);
		double prob = exp(-beta*(newAction - oldAction));
		if (r < prob)
			Model::spins = temp;
			return 1;
	}	
	return  0;
} 

template <class Model>
void SpinGraph<Model>::wolffUpdate(double beta) //handy modification of BFS code
{
	int size = order();
	double prob = 1.0 - exp(-2.0*beta);
	std::vector<double> temp = Model::spins;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine generator (seed);
	std::uniform_int_distribution<int> dist(1, size);
	int source = dist(generator);//dist(generator);
	std::vector<bool> visited(size, false);
	std::vector<int> clustered;
	std::queue<int> Q;
	Q.push(source);
	while (!Q.empty())
	{
		int v = Q.front();
		Q.pop();
		std::vector<int> children = connectedVertices(v);
		for (int i = 0; i < children.size(); i++)
		{
			if (visited[children[i]-1] == false)	
			{	
				std::uniform_real_distribution<double> dist(0.0, 1.0);
				double r = dist(generator);
				if (Model::spins[v-1] == Model::spins[children[i]-1])
				{
					if (r < prob)
					{
						Q.push(children[i]);
						clustered.push_back(children[i]-1);
						visited[children[i]-1] = true;
					}	
				}				
			}
		}
	}
	for (int i = 0; i < clustered.size(); i++) 
		Model::spins[clustered[i]] *= -1;
}


Graph generateSquareLattice(int length)//hence order is length squared
{
	int order = length*length;
	Graph squareGraph(order);
	for (int i = 1; i < order+1; i++)
	{
		if ((i % length  != 0) and (order - i >= length)) //normal case
		{ 
			Edge newAcross(i, i+1);
			squareGraph.addEdge(newAcross);
			Edge newDown(i, i + length);
			squareGraph.addEdge(newDown);
		}
		else if ((i % length == 0) and (order - i >= length)) //middle edge
		{
			Edge newAcross(i, i+1-length);
			squareGraph.addEdge(newAcross);
			Edge newDown(i, i + length);
			squareGraph.addEdge(newDown);
		}
		else if ((i % length != 0) and (order - i <= length)) //last row, not corner
		{
			Edge newAcross(i, i+1);
			squareGraph.addEdge(newAcross);
			Edge newDown(i, i -order + length);
			squareGraph.addEdge(newDown);
		}
		else if ((i % length == 0) and (order - i <= length))//bottom corner
		{
			Edge newAcross(i, i+1-length);
			squareGraph.addEdge(newAcross);
			Edge newDown(i, i - order + length);
			squareGraph.addEdge(newDown);
		}
	}
	
	return squareGraph;
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

Graph generateParametrisedGraph(int length, int disconnects)//generates square lattice initially which is then randomised to some degree
{
	Graph paramGraph = generateSquareLattice(length);
	
	int vertices = length*length;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine generator (seed);
	std::uniform_int_distribution<int> dist(1, vertices);

	for (int i = 0; i < disconnects; i++)
	{
		int j, k, l;
		j = dist(generator); //completely new vertex
		l = dist(generator); //access adj_list for this vector, k rand in that list
		std::vector<int> adj_list = paramGraph.connectedVertices(l);
		if (adj_list.size() != 0)
		{
			int randomIndex = rand() % adj_list.size();
			k = adj_list[randomIndex];
			paramGraph.removeEdge(Edge(l, k));
			paramGraph.addEdge(Edge(j, l));
		}
	}
	return paramGraph;
}
	


SpinGraph<Ising> generateRandomIsingGraph(int n, int edges, bool randomize)
{
	SpinGraph<Ising> randomGraph(n, randomize);
	
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

SpinGraph<Ising> generateSquareIsingGraph(int length, int randomize)
{
	int order = length*length;
	SpinGraph<Ising> squareGraph(order, randomize);
	for (int i = 1; i < order+1; i++)
	{
		if ((i % length  != 0) and (order - i >= length)) //normal case
		{ 
			Edge newAcross(i, i+1);
			squareGraph.addEdge(newAcross);
			Edge newDown(i, i + length);
			squareGraph.addEdge(newDown);
		}
		else if ((i % length == 0) and (order - i >= length)) //middle edge
		{
			Edge newAcross(i, i+1-length);
			squareGraph.addEdge(newAcross);
			Edge newDown(i, i + length);
			squareGraph.addEdge(newDown);
		}
		else if ((i % length != 0) and (order - i <= length)) //last row, not corner
		{
			Edge newAcross(i, i+1);
			squareGraph.addEdge(newAcross);
			Edge newDown(i, i -order + length);
			squareGraph.addEdge(newDown);
		}
		else if ((i % length == 0) and (order - i <= length))//bottom corner
		{
			Edge newAcross(i, i+1-length);
			squareGraph.addEdge(newAcross);
			Edge newDown(i, i - order + length);
			squareGraph.addEdge(newDown);
		}
	}
	
	return squareGraph;
}

//didn't use potts stuff and it caused me to write some sloppy template stuff so will probably just remove		
SpinGraph<Potts> generateRandomPottsGraph(int n, int edges, int q, bool randomize)
{
	SpinGraph<Potts> randomGraph(n, q, randomize);
	
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

//function to create a graph which can be square, fully random or some mix, ideally no. of vertices is a square number
SpinGraph<Ising> generateParametrisedIsingGraph(int length, int edges, int disconnects, bool randomize)
{
	SpinGraph<Ising> paramGraph = generateSquareIsingGraph(length, randomize);
	
	int vertices = length*length;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  	std::default_random_engine generator (seed);
	std::uniform_int_distribution<int> dist(1, vertices);

	for (int i = 0; i < disconnects; i++)
	{
		int j, k, l;
		j = dist(generator); //completely new vertex
		l = dist(generator); //access adj_list for this vector, k rand in that list
		std::vector<int> adj_list = paramGraph.connectedVertices(l);
		if (adj_list.size() != 0)
		{
			int randomIndex = rand() % adj_list.size();
			k = adj_list[randomIndex];
			paramGraph.removeEdge(Edge(l, k));
			paramGraph.addEdge(Edge(j, l));
		}
	}
	
	return paramGraph;
}

/*
Most of the following are sloppily written (no function templates) and in general not used but I'll leave them here because they cause no harm
In general I just used numpy/scipy or my own python fns where possible for sem, variance and jackknife
*/


//returns variance of array of given size
double variance(int arr[], int size)
{
	double avg = 0.0;
	double var = 0.0;
	
	for (int i = 0; i < size; i++)
	{
		avg += arr[i];
	}
	avg = avg/size;	
	
	for (int i = 0; i < size; i++)
	{
		var += (arr[i] - avg)*(arr[i] - avg);
	}
	
	return var/double(size-1);
}

double variance(double arr[], int size)
{
	double avg = 0.0;
	double var = 0.0;
	
	for (int i = 0; i < size; i++)
	{
		avg += arr[i];
	}
	avg = avg/size;	
	
	for (int i = 0; i < size; i++)
	{
		var += (arr[i] - avg)*(arr[i] - avg);
	}
	
	return var/double(size-1);
}

double varianceVector(std::vector<int> arr)
{
	double avg = 0.0;
	double var = 0.0;
	double size = arr.size();	

	for (int i = 0; i < size; i++)
	{
		avg += arr.at(i);
	}
	avg = avg/size;	
	
	for (int i = 0; i < size; i++)
	{
		var += (arr.at(i) - avg)*(arr.at(i) - avg);
	}
	
	return var/double(size-1);
}


double mean(double arr[], int size)
{
	double avg = 0.0; 
	for (int i = 0; i < size; i++)
	{
		avg += arr[i];
	}
	return avg/double(size);
}

double mean(int arr[], int size)
{
	double avg = 0.0; 
	for (int i = 0; i < size; i++)
	{
		avg += arr[i];
	}
	return avg/double(size);
}

double meanVector(std::vector<int> vec)
{
	double avg = 0.0;
	int size = vec.size();
	for (int i = 0; i < size; i++)
	{
		avg += vec.at(i);
	}
	return avg/double(size);
}

double jackKnifeMean(int array[], int size)
{
	double base_mean = mean(array, size);
	double sum_mean = 0.0;
	for (int i = 0; i < size; i++)
	{
		double sample[size-1];
		int change = 0;
		for (int j = 0; j < size; j++)
		{
			if (j == i)
				change++;
			else
				sample[j] = array[j+change];
		}
				
		double sample_mean = mean(sample, size - 1);
		sum_mean += (sample_mean - base_mean)*(sample_mean - base_mean);
	}
	return sqrt(double(sum_mean*(size-1))/double(sqrt(size)));
}

double jackKnifeMean(double array[], int size)
{
	//double base_mean = mean(array, size);
	double sum_mean = 0.0;
	double base_mean = 0.0;
	double jackValues[size];
	for (int i = 0; i < size; i++)
	{
		double sample[size-1];
		int change = 0;
		for (int j = 0; j < size; j++)
		{
			if (j == i)
				change++;
			else
				sample[j] = array[j+change];
		}
				
		double sample_mean = mean(sample, size - 1);
		jackValues[i] = sample_mean;
		base_mean += sample_mean;
	}
	
	base_mean = base_mean/double(size);
	for (int i = 0; i < size; i++)
	{
		sum_mean += (jackValues[i] - base_mean)*(jackValues[i] - base_mean);
	}
	return sqrt(double(sum_mean*(size-1)))/double(sqrt(size));
}


//use jackknife technique to return errorbars for a given data set
double jackKnifeVariance(int array[], int size)
{
	double sum_variance = 0.0;
	double base_variance = 0.0;
	double jackValues[size];
	for (int i = 0; i < size; i++)
	{
		double sample[size-1];
		int change = 0;
		for (int j = 0; j < size; j++)
		{
			if (j == i)
				change++;
			else
				sample[j] = array[j+change];
		}
				
		double sample_variance = variance(sample, size - 1);
		jackValues[i] = sample_variance;
		base_variance += sample_variance;
	}
	
	base_variance = base_variance/double(size);
	for (int i = 0; i < size; i++)
	{
		sum_variance += (jackValues[i] - base_variance)*(jackValues[i] - base_variance);
	}
	return sqrt(double(sum_variance*(size-1)))/double(sqrt(size));
}

double jackKnifeVariance(double array[], int size)
{
	double base_variance = variance(array, size);
	double base_mean = mean(array, size);
	double sum_vars = 0.0;
	for (int i = 0; i < size; i++)
	{
		double sample[size-1];
		int change = 0;
		for (int j = 0; j < size; j++)
		{
			if (j == i)
				change++;
			else
				sample[j] = array[j+change];
		}
				
		double sample_variance = variance(sample, size - 1);
		sum_vars += (sample_variance - base_variance)*(sample_variance - base_variance);
	}
	
	//sum_vars = sum_vars/double(size-1);
	//return sum_vars; will return variance
	// more useful will be stderror
	return sqrt(sum_vars)/double(sqrt(size));

}
































