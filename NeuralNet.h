#include <vector>
#include <math.h>
#include "utils.h"

using namespace std;

struct Neuron
{
   //the number of inputs into the neuron
   int mNumInputs;

   //the weights for each input
   vector<double> mWeights;

   //ctor
   Neuron(int numInputs);

};

struct NeuronLayer
{
  //the number of neurons in this layer
  int mNumNeurons;

  //the layer of neurons
  vector<Neuron> mNeurons;

  NeuronLayer(int numNeurons, int numInputsPerNeuron);

};

class NeuralNet
{
private:
  int mNumInputs;
  int mNumOutputs;
  int mNumHiddenLayers;
  int mNeuronsPerHiddenLayer;

  //storage for each layer of neurons including the output layer
  vector<NeuronLayer> mLayers;

 
public:
  NeuralNet();

  void CreateNet();

  //gets the weights from the NN
  vector<double> GetWeights()const;

  //returns the total number of weights in the net
  int GetNumberOfWeights()const;

  //replaces the weights with new ones
  void PutWeights(vector<double> &weights);

  //calculates the outputs from a set of inputs
  vector<double> Update(vector<double> &inputs);

  //sigmoid response curve
  inline double Sigmoid(double activation, double response);

};