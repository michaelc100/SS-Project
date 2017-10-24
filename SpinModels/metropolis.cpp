#include "SpinGraphs.h"
#include <iostream>

int getMacroState(std::vector<int> states)
{
	int upCount = 0;
	int downCount = 0;
	
	for (int i = 0; i < states.size(); i++)
	{
		if (states[i] == 1)
			upCount++;
		else 
			downCount++;
	}
	
	if (upCount == 4)
		return 0;
	else if (upCount == 3 & downCount == 1)
		return 1;
	else if (upCount == 2 & downCount == 2)
		return 2;
	else if (upCount == 1 & downCount == 3)
		return 3;
	else 
		return 4;
}



int main()
{
	SpinGraph<Ising> graph1 = SpinGraph<Ising>(4);

	//adding edges to make graph square type
	Edge e1(1,2);
	Edge e2(2,3);
	Edge e3(3,4);
	Edge e4(1,4);
	graph1.addEdge(e1);
	graph1.addEdge(e2);
	graph1.addEdge(e3);
	graph1.addEdge(e4);

	double stateCountg1[5] = {0.0}; //to hold counts of each time system is in a given macrostate
	int runtime = 2000000;
	double beta = 1.0;

	for (int i = 0; i < runtime; i++)
	{
		graph1.update(beta);
		stateCountg1[getMacroState(graph1.spins)] += 1.0;
	}		

	std::cout << "Square type graph\n";
	for (int i = 0; i < 5; i++)
		std::cout << "MacroState: " << i << " Prob: " << stateCountg1[i]/runtime << "\n";

	SpinGraph<Ising> graph2 = SpinGraph<Ising>(4);
	
	//adding edges to make graph weird type
	Edge f1(1,2);
	Edge f2(2,3);
	Edge f3(2,4);
	Edge f4(3,4);
	graph2.addEdge(f1);
	graph2.addEdge(f2);
	graph2.addEdge(f3);
	graph2.addEdge(f4);

	double stateCountg2[5] = {0.0};	
	for (int i = 0; i < runtime; i++)
	{
		graph2.update(beta);
		stateCountg2[getMacroState(graph2.spins)] += 1.0;
	}		

	std::cout << "Weird type graph\n";
	for (int i = 0; i < 5; i++)
		std::cout << "MacroState: " << i << " Prob: " << stateCountg2[i]/runtime << "\n";


	return 0;
}
