#ifndef CGENALG_H
#define CGENALG_H
//------------------------------------------------------------------------
//
//	Name: CGenAlg.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Genetic algorithm class.This is based for manipulating std::vectors 
//  of *real* numbers. Used to adjust the weights in a feedforward neural
//  network.
//
//------------------------------------------------------------------------

#include <vector>
#include <algorithm>
#include <iostream>

#include "utils.h"

using namespace std;

//-----------------------------------------------------------------------
//
//	create a structure to hold each genome
//-----------------------------------------------------------------------
struct Genome
{
	vector <double>	weights;

	double          fitness;

	Genome():fitness(0){}

	Genome( vector <double> w, double f): weights(w), fitness(f){}

	//overload '<' used for sorting
	friend bool operator<(const Genome& lhs, const Genome& rhs)
	{
		return (lhs.fitness < rhs.fitness);
	}
};

//-----------------------------------------------------------------------
//
//	the genetic algorithm class
//-----------------------------------------------------------------------
class GenAlg
{
private:
	
	//this holds the entire population of chromosomes
	vector <Genome>	 mPop;

	//size of population
	int mPopSize;
	
	//amount of weights per chromo
	int mChromoLength;

	//total fitness of population
	double mTotalFitness;

	//best fitness this population
	double mBestFitness;

	//average fitness
	double mAverageFitness;

	//worst
	double mWorstFitness;

	//keeps track of the best genome
	int		mFittestGenome;

	//probability that a chromosones bits will mutate.
	//Try figures around 0.05 to 0.3 ish
	double mMutationRate;

	//probability of chromosones crossing over bits
	//0.7 is pretty good
	double mCrossoverRate;

	//generation counter
	int	  mGeneration;


	void    Crossover(const vector<double> &mum,
                    const vector<double> &dad,
                    vector<double>       &baby1,
                    vector<double>       &baby2);
	

	void    Mutate(vector<double> &chromo);
	
	Genome	GetChromoRoulette();

	//use to introduce elitism
  void	  GrabNBest(int	            NBest,
	                  const int       NumCopies,
	                  vector<Genome>	&vecPop);


	void	  CalculateBestWorstAvTot();

	void	  Reset();



public:
		
	GenAlg(int 		popsize,
			    double	MutRat,
			    double	CrossRat,
			    int	  	numweights);

	//this runs the GA for one generation.
	vector<Genome>	Epoch(vector<Genome> &old_pop);


	//-------------------accessor methods
	vector<Genome>	GetChromos()const{return mPop;}
	double			    AverageFitness()const{return mTotalFitness / mPopSize;}
	double		    	BestFitness()const{return mBestFitness;}

	
};

#endif

