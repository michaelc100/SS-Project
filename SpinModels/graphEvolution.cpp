#include <iostream>
#include "SpinGraphs.h"
#include <cstdlib>
#include <math.h>

//returns variance of array of given size
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

double mean(double arr[], int size)
{
	double avg = 0.0; 
	for (int i = 0; i < size; i++)
	{
		avg += arr[i];
	}
	return avg/double(size);
}

double degreeAverage(int v, int e)
{
	return (2*double(e))/v;
}

double degreeVariance(SpinGraph<Ising> rand)
{
	std::set<int> degrees;

	for (int i = 0; i < rand.vertexDegrees().size(); i++)
		degrees.insert(rand.vertexDegrees()[i]);
	
	double probs[degrees.size()] = {0.0};

	{
		int j = 0;
		for (std::set<int>::iterator it = degrees.begin(); it != degrees.end(); ++it)
		{
			for (int k = 0; k < rand.vertexDegrees().size(); k++)
			{
				if (rand.vertexDegrees()[k] == *it)
					probs[j] = probs[j] + double(1)/rand.vertexDegrees().size();
			}
			j++;
		}
	}

	double degVar;
	double sndMoment = 0;

	{
		int j = 0;
		for (std::set<int>::iterator it = degrees.begin(); it != degrees.end(); it++)
		{
			sndMoment = sndMoment + probs[j]*(*it)*(*it);
			j++;
		}
	}
	
	degVar = sndMoment - degreeAverage(rand.order(),rand.edge_order())*degreeAverage(rand.order(),rand.edge_order());
	return degVar;
}


//use jackknife technique to return errorbars for a given data set
double jackKnife(double array[], int size)
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

//generates random graph of given (V, E), performs mc simulation and writes results to file
int main(int argc, char * argv[])
{
	std::cout.setf(std::ios_base::fixed);
	int factor, vertices;
	vertices = 1000;
	factor = 4;
	
	SpinGraph<Ising> rg = generateRandomIsingGraph(vertices, int((vertices*factor)/2), false);
	
	/*
	double accept = 4.5;
	double degvar = 0.0;
	while ((accept < degvar) || (accept > degvar+0.1))	
	{
		rg = generateRandomIsingGraph(vertices, int((vertices*factor)/2), false);
		degvar = degreeVariance(rg);
	}		
	*/
	

	double t0 = 0.1; //initial temp
	std::cout << t0 << "\n";
	int iterations = 50; //num of unique temps to sample
	double temp_step = 0.08;
	double temps[iterations];
	temps[0] = t0;
	
	//output temperature array
	for (int i = 1; i < iterations; i++)
	{
		temps[i] = temps [i-1] + temp_step;
		std::cout << temps[i] << "\n";
	}
	std::cout << "---\n";

	
	//output magnetisation array
	for (int i = 0; i < iterations; i++)
	{
		int markov_updates = 2000; //let graph evolve at each new temp
		int snapshots = 1000;
		double mag_stores[snapshots] = {0.0}; //at end of each temp iteration, compute variance of this array
		for (int k = 0; k < snapshots; k++)
		{
			for (int j = 0; j < markov_updates; j++)
				rg.update(1/(temps[i]));

			double current_mag = rg.absMagnetisation();						
			mag_stores[k] = current_mag;
		}
			
		for (int l = 0; l < snapshots; l++)
			std::cout << mag_stores[l] << "\n";
		double mean_mag = mean(mag_stores, snapshots);
		std::cout << "Mean magnetisation: " << mean_mag << "\n";
		double var_mag = variance(mag_stores, snapshots);
		std::cout << "Variance: " << var_mag << "\n";
		double std_err = sqrt(variance(mag_stores, snapshots))/sqrt(double(snapshots));
		std::cout << "Standard Error is: " << std_err << "\n";
		double var_error = jackKnife(mag_stores, snapshots);
		std::cout << "Jack Knife obtained error is: " << var_error << "\n";
		std::cout << "---\n";	
	}	

	return 0;
}

