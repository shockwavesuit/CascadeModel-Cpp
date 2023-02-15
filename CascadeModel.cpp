#pragma once
using namespace std;
#include <iostream>
#include <thread>
#include <random>



//Number of Synapses
	 const int Ns = 1000;
//Number of Hidden Layers
	 const int n = 3;
//Probability multiplier
	 float x = 0.5f;
//Storing the internal hidden state of each synapse
	int hiddenLayer[Ns];
//Storing the binary state of the synapse	
	bool synapse [Ns];
//Storing the potentiation(true) and depression(false) for the memory storage
	bool memorySignal[Ns];
//Values of the plastic and metaplastic probabilities plasticProbability [i-1]=q_i 
	float plasticProbability [n];
	float metaPlasticProbability [n];
//The equilibrium signal value before memory storage
	int initialUnchangedSignal;
//The signal output
	float Signal = 0,OutputSignal=0;
	bool processing = true;
	void main()
	{
		//q_i=x^{i-1} //p_i=x^{i}/1-x
		plasticProbability[0] = 1;
		metaPlasticProbability[0] = x / (1 - x);
		for (int i = 1; i < n - 1; i++)
		{
			plasticProbability[i] = plasticProbability[i - 1] * x;
			metaPlasticProbability[i] = metaPlasticProbability[i - 1] * x;

		}
		plasticProbability[n - 1] = plasticProbability[n - 2] * x;
		for (int i = 0; i < Ns; i++)
		{
		 // Picking the uniform distribution across all hidden layers for optimal results
			hiddenLayer[i] = RandomInteger(0,n-1);
				
		//Picking a random configuration from the equilibrium distribution 
		   synapse[i] = RandomInteger(0,1) == 1 ? true : false;
		  //Picking a random memory which potentiates or depresses the i'th synapse
			memorySignal[i] =RandomInteger(0,1) == 1 ? true : false;
			//Summing all statesfrom the equilibrium which are potentiated when strong or depressed when weak from the memory signal
			if (!memorySignal[i] ^ synapse[i])
			{
				initialUnchangedSignal += 1;
			}
		}
		//Storing the memory signal as a first input 
		FireInput(true);
		//Thread to perform a random background modification of the synapses
		new thread(UpdateMemories);
	}

	// Update is called once per frame
	void UpdateMemories()
	{
		while (processing)
		{	//Subsequent Background Inputs
			FireInput(false);
			OutputSignal = Signal;
			Signal = 0;
			//Rate of Subsequent events
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
			
		
	}
	
	int GetOutputSignal()
	{
		return OutputSignal;
	}
	void FireInput(bool storage)
	{
		for (int i = 0; i < Ns; i++)
		{
			//Random Potentation or Depression Event
			bool Potentiation =RandomFloat(0, 2) == 1 ? true : false;
		//First Input and Storage
			if (storage)
				Potentiation = memorySignal[i];

			if (synapse[i] ^ Potentiation)//A plastic transition
			{
				if (RandomFloat(0.0f, 1.0f) < plasticProbability[hiddenLayer[i]])
				{
					//Switch State and Bring to the first Layer
					synapse[i] = !synapse[i];
					hiddenLayer[i] = 0;
				}


			}
			else // A metaplastic transition
			{
				//Move Down The Layers
				if (hiddenLayer[i] < n - 1)
					hiddenLayer[i] = RandomFloat(0.0f, 1.0f) > metaPlasticProbability[hiddenLayer[i]] ? hiddenLayer[i] : hiddenLayer[i]++;

				

			}
			//Count number of  memory potentiated states in high states and  memory depressed states in low states
			if (!memorySignal[i] ^ synapse[i])
			{
				Signal += 1;
			}


		}
		//Find only the excess number of states from equilibrium
		Signal -= initialUnchangedSignal;
	}

	void StopProcessing()
	{
		processing = false;
	}

float RandomFloat(float min, float max)
{
	
	 
	return min + (((float)rand()) / (float)RAND_MAX) * (max - min);
}
int RandomInteger(int min, int max)
{

	return min + (rand() % (max - min + 1))
}
