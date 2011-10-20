#define NUM_INPUTS 10 * 20
#define NUM_OUTPUTS 4
#define NUM_HIDDEN_LAYERS 4
#define	NEURONS_PER_LAYER 20
#define DBIAS -1
#define ACTIVATION_RESPONSE 1

#include "NeuralNet.h"

//*************************** methods for Neuron **********************
//
//---------------------------------------------------------------------
Neuron::Neuron(int NumInputs): mNumInputs(NumInputs+1)										
{
	//we need an additional weight for the bias hence the +1
	for (int i=0; i<NumInputs+1; ++i)
	{
		//set up the weights with an initial random value
		mWeights.push_back(RandomClamped());
	}
}

//************************ methods for NeuronLayer **********************

//-----------------------------------------------------------------------
//	ctor creates a layer of neurons of the required size by calling the 
//	SNeuron ctor the rqd number of times
//-----------------------------------------------------------------------
NeuronLayer::NeuronLayer(int numNeurons, 
                           int numInputsPerNeuron):	mNumNeurons(numNeurons)
{
	for (int i=0; i<numNeurons; ++i)

		mNeurons.push_back(Neuron(numInputsPerNeuron));
}

//************************ methods forCNeuralNet ************************

//------------------------------default ctor ----------------------------
//
//	creates a ANN based on the defines
//-----------------------------------------------------------------------
NeuralNet::NeuralNet() 
{
	mNumInputs	           =	NUM_INPUTS;
	mNumOutputs		       =	NUM_OUTPUTS;
	mNumHiddenLayers	   =	NUM_HIDDEN_LAYERS;
	mNeuronsPerHiddenLayer =	NEURONS_PER_LAYER;

	CreateNet();
}

//------------------------------createNet()------------------------------
//
//	this method builds the ANN. The weights are all initially set to 
//	random values -1 < w < 1
//------------------------------------------------------------------------
void NeuralNet::CreateNet()
{
	//create the layers of the network
	if (mNumHiddenLayers > 0)
	{
		//create first hidden layer
	  mLayers.push_back(NeuronLayer(mNeuronsPerHiddenLayer, mNumInputs));
    
    for (int i=0; i<mNumHiddenLayers-1; ++i)
    {

			mLayers.push_back(NeuronLayer(mNeuronsPerHiddenLayer,
                                         mNeuronsPerHiddenLayer));
    }

    //create output layer
	  mLayers.push_back(NeuronLayer(mNumOutputs, mNeuronsPerHiddenLayer));
	}

  else
  {
	  //create output layer
	  mLayers.push_back(NeuronLayer(mNumOutputs, mNumInputs));
  }
}

//---------------------------------GetWeights-----------------------------
//
//	returns a vector containing the weights
//
//------------------------------------------------------------------------
vector<double> NeuralNet::GetWeights() const
{
	//this will hold the weights
	vector<double> weights;
	
	//for each layer
	for (int i=0; i<mNumHiddenLayers + 1; ++i)
	{

		//for each neuron
		for (int j=0; j<mLayers[i].mNumNeurons; ++j)
		{
			//for each weight
			for (int k=0; k<mLayers[i].mNeurons[j].mNumInputs; ++k)
			{
				weights.push_back(mLayers[i].mNeurons[j].mWeights[k]);
			}
		}
	}

	return weights;
}

//-----------------------------------PutWeights---------------------------
//
//	given a vector of doubles this function replaces the weights in the NN
//  with the new values
//
//------------------------------------------------------------------------
void NeuralNet::PutWeights(vector<double> &weights)
{
	int cWeight = 0;
	
	//for each layer
	for (int i=0; i<mNumHiddenLayers + 1; ++i)
	{

		//for each neuron
		for (int j=0; j<mLayers[i].mNumNeurons; ++j)
		{
			//for each weight
			for (int k=0; k<mLayers[i].mNeurons[j].mNumInputs; ++k)
			{
				mLayers[i].mNeurons[j].mWeights[k] = weights[cWeight++];
			}
		}
	}

	return;
}

//---------------------------------GetNumberOfWeights---------------------
//
//	returns the total number of weights needed for the net
//
//------------------------------------------------------------------------
int NeuralNet::GetNumberOfWeights() const
{

	int weights = 0;
	
	//for each layer
	for (int i=0; i<mNumHiddenLayers + 1; ++i)
	{

		//for each neuron
		for (int j=0; j<mLayers[i].mNumNeurons; ++j)
		{
			//for each weight
			for (int k=0; k<mLayers[i].mNeurons[j].mNumInputs; ++k)
			
				weights++;
			
		}
	}

	return weights;
}

//-------------------------------Update-----------------------------------
//
//	given an input vector this function calculates the output vector
//
//------------------------------------------------------------------------
vector<double> NeuralNet::Update(vector<double> &inputs)
{
	//stores the resultant outputs from each layer
	vector<double> outputs;

	int cWeight = 0;
	
	//first check that we have the correct amount of inputs
	if (inputs.size() != mNumInputs)
  {
		//just return an empty vector if incorrect.
		return outputs;
  }
	
	//For each layer....
	for (int i=0; i<mNumHiddenLayers + 1; ++i)
	{		
		if ( i > 0 )
    {
			inputs = outputs;
    }

		outputs.clear();
		
		cWeight = 0;

		//for each neuron sum the (inputs * corresponding weights).Throw 
		//the total at our sigmoid function to get the output.
		for (int j=0; j<mLayers[i].mNumNeurons; ++j)
		{
			double netinput = 0;

			int	NumInputs = mLayers[i].mNeurons[j].mNumInputs;
			
			//for each weight
			for (int k=0; k<NumInputs - 1; ++k)
			{
				//sum the weights x inputs
				netinput += mLayers[i].mNeurons[j].mWeights[k] * 
                    inputs[cWeight++];
			}

			//add in the bias
			netinput += mLayers[i].mNeurons[j].mWeights[NumInputs-1] * 
                  DBIAS;

			//we can store the outputs from each layer as we generate them. 
			//The combined activation is first filtered through the sigmoid 
			//function
			outputs.push_back(Sigmoid(netinput,
                                ACTIVATION_RESPONSE));

			cWeight = 0;
		}
	}

	return outputs;
}

//-------------------------------Sigmoid function-------------------------
//
//------------------------------------------------------------------------
double NeuralNet::Sigmoid(double netinput, double response)
{
	return ( 1 / ( 1 + exp(-netinput / response)));
}