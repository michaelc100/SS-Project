#include <iostream>
#include "SpinGraphs.h"
#include <cstdlib>
#include <math.h>

int main(int argc, char * argv[])
{
	std::cout.setf(std::ios_base::fixed);
	int factor, vertices;
	int length = 35;
	vertices = length*length;
	factor = 4;
	int dc = atoi(argv[1]);
	SpinGraph<Ising> rg = generateRandomIsingGraph(vertices, int((vertices*factor)/2), true);
	//SpinGraph<Ising> rg = generateParametrisedIsingGraph(length, vertices*factor/2, dc, true);
	double degvar = varianceVector(rg.vertexDegrees());
	int source = rand() % vertices;
	bool acceptSource = false;
	while (!acceptSource)
	{
		if (rg.connectedVertices(source).size() == 0)
		{
			source = rand() % vertices;
		}
		else
		{
			acceptSource = true;
		}
	}

	int vertexPairs = atoi(argv[2]);
	int targets[vertexPairs]; //vertices dist of (1, 2, 3, 4) away from source
	std::vector<int> meanPaths = rg.BFSsearch(source);
	for (int i = 0; i < meanPaths.size(); i++)
	{
		if (meanPaths[i] == std::numeric_limits<int>::max())
			meanPaths[i] = 0;
	}
	for (int i = 1; i < vertexPairs+1; i++)
	{
		bool accept = false;
		while (!accept)
		{
			int target = rand() % vertices;
			if ((target != source) and (meanPaths[target] == i))
			{
				targets[i-1] = target;
				accept = true;
			}
		}
	}

	double t0 = atof(argv[3]); //initial temp
	int iterations = 25; //num of unique temps to sample
	double temp_step = 0.1;
	double temps[iterations];
	temps[0] = t0;
	int markov_updates = 300; //let graph evolve at each new temp
	int snapshots = 2048;
	int spinValues[snapshots]; //will start by simply measuring correlation between two vertices
	std::cout << "Info: |V| = " << vertices << " Conn. = " << factor << " Degree Variance: " << degvar << " Metropolis updates = " << markov_updates << " Snapshots = " << snapshots << "\n";	
	
	std::cout << t0 << "\n";
	//output temperature array
	for (int i = 1; i < iterations; i++)
	{
			
		temps[i] = temps [i-1] + temp_step;
		std::cout << temps[i] << "\n";
	}
	std::cout << "---\n";

	int sourceSpin[snapshots];
	int targetSpins[vertexPairs][snapshots];
	int spinProduct[vertexPairs][snapshots];
		
	//output magnetisation array
	for (int i = 0; i < iterations; i++)
	{
		double mag_stores[snapshots];
		double abs_mag_stores[snapshots]; //at end of each temp iteration, compute variance of this array
		for (int k = 0; k < snapshots; k++)
		{
			for (int j = 0; j < markov_updates; j++)
				rg.wolffUpdate(1/(temps[i]));

			double current_mag = rg.magnetisation();						
			mag_stores[k] = current_mag;
			abs_mag_stores[k] = abs(current_mag);
			std::cout << "Mag: " << mag_stores[k] << "\n";
			sourceSpin[k] = rg.getSpin(source);
			for (int n = 0; n < vertexPairs; n++)
			{
				targetSpins[n][k] = rg.getSpin(targets[n]+1);
				spinProduct[n][k] = sourceSpin[k]*targetSpins[n][k];	
			}
		}
		
		//want to calculate and output spin-spin correlation for each temp
		
		for (int m = 0; m < vertexPairs; m++)
		{
			double ssCorrelation;
			ssCorrelation = mean(spinProduct[m], snapshots); //removing to test alternate method- mean(sourceSpin, snapshots)*mean(targetSpins[m], snapshots);
			std::cout << "Spin-Spin correlation: " << ssCorrelation << " Dist: " << meanPaths[targets[m]] << " Jack_knife: " << \
			sqrt(variance(spinProduct[m], snapshots))/sqrt(double(snapshots)) << " " << m+1 << "\n";
			//output jacknife error for this value
		}
	
		double mean_mag = mean(mag_stores, snapshots);
		std::cout << "Mean magnetisation: " << mean_mag << "\n";
		//most of this stuff is done in python script, might take out of here
		/*
		double var_mag = variance(mag_stores, snapshots);
		std::cout << "Variance: " << var_mag << "\n";
		double std_err = sqrt(variance(mag_stores, snapshots))/sqrt(double(snapshots));
		std::cout << "Standard Error is: " << std_err << "\n";
		double var_error = jackKnifeVariance(mag_stores, snapshots);
		std::cout << "Jack Knife obtained error is: " << var_error << "\n";
		*/
		std::cout << "---\n";	
		
	}	
	
	return 0;
}

