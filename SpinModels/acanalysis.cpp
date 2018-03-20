#include <iostream>
#include "SpinGraphs.h"
#include <cstdlib>
#include <math.h>

int main(int argc, char * argv[])
{
	std::cout.setf(std::ios_base::fixed);
	int factor, vertices;
	int length = 50;
	vertices = 1000;
	factor = 4;
	//int dc = atoi(argv[1]);
	SpinGraph<Ising> rg = generateRandomIsingGraph(vertices, int((vertices*factor)/2), true);
	//SpinGraph<Ising> rg = generateSquareIsingGraph(length, true);
	//SpinGraph<Ising> rg = generateParametrisedIsingGraph(length, vertices*factor/2, dc, true);
	double t0 = 3.0; //initial temp
	int iterations = 10; //num of unique temps to sample
	double temp_step = 0.05;
	double temps[iterations];
	temps[0] = t0;
	int checkUpdates = 250;
	std::vector<int> updates(10);
	int updateBase = 50;
	for (int i = 0; i < updates.size(); i++) updates[i] = updateBase*(i+1);
	int snapshots = 2048;
	int spinValues[snapshots]; //will start by simply measuring correlation between two vertices
	std::cout << "Info: |V| = " << vertices << " Conn. = " << factor << " Snapshots = " << snapshots << "\n";	
	
	std::cout << t0 << "\n";
	//output temperature array
	for (int i = 1; i < iterations; i++)
	{
			
		temps[i] = temps [i-1] + temp_step;
		std::cout << temps[i] << "\n";
	}
	std::cout << "---\n";
	
	double tc = temps[0];
	double sus_max = -1.0;
	//initial sweep to find rough tc
	for (int i = 0; i < iterations; i++)
		{
			double mag_stores[snapshots];
			double abs_mag_stores[snapshots]; //at end of each temp iteration, compute variance of this array
			for (int k = 0; k < snapshots; k++)
			{
				for (int j = 0; j < checkUpdates; j++)
					rg.update(1/(temps[i]));

				double current_mag = rg.magnetisation();						
				mag_stores[k] = current_mag;
				abs_mag_stores[k] = abs(current_mag);
				//std::cout << "Mag: " << mag_stores[k] << "\n";
			}
		
			//want to calculate and output spin-spin correlation for each temp
	
			double mean_mag = mean(mag_stores, snapshots);
			double var_mag = variance(mag_stores, snapshots);
			double sus = var_mag/temps[i];
			if (sus > sus_max) tc = temps[i]; 
		}
	
	std::cout << "Splitting file now\n";
	//output mag array to get tint times
	for (int u = 0; u < updates.size(); u++)
	{
		double mag_stores[snapshots];
		double abs_mag_stores[snapshots]; //at end of each temp iteration, compute variance of this array
		for (int k = 0; k < snapshots; k++)
		{
			for (int j = 0; j < updates[u]; j++)
				rg.wolffUpdate(1/(tc));

			double current_mag = rg.magnetisation();						
			mag_stores[k] = current_mag;
			abs_mag_stores[k] = abs(current_mag);
			std::cout << "Mag: " << mag_stores[k] << "\n";
		}
	
		//want to calculate and output spin-spin correlation for each temp

		double mean_mag = mean(mag_stores, snapshots);
		std::cout << "Mean magnetisation: " << mean_mag << "\n";
		std::cout << "---\n";	
		
	}
	return 0;
}

