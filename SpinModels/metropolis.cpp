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

	double stateCount[5] = {0.0}; //to hold counts of each time system is in a given macrostate
	int runtime = 2000000;
	double beta = 1.0;

	for (int i = 0; i < runtime; i++)
	{
		graph1.update(beta);
		stateCount[getMacroState(graph1.spins)] += 1.0;
	}		

	for (int i = 0; i < 5; i++)
		std::cout << "MacroState: " << i << " Prob: " << stateCount[i]/runtime << "\n";

	return 0;
}
