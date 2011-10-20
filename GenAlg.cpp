#include "GenAlg.h"

#define MAX_PERTURBATION 0
#define NUM_COPIES_ELITE 0
#define NUM_ELITE 0



//-----------------------------------constructor-------------------------
//
//	sets up the population with random floats
//
//-----------------------------------------------------------------------
GenAlg::GenAlg(int	  popsize,
                 double	MutRat,
                 double	CrossRat,
                 int	  numweights) :	mPopSize(popsize),
                                      mMutationRate(MutRat),
										                  mCrossoverRate(CrossRat),
										                  mChromoLength(numweights),
										                  mTotalFitness(0),
										                  mGeneration(0),
										                  mFittestGenome(0),
										                  mBestFitness(0),
										                  mWorstFitness(99999999),
										                  mAverageFitness(0)
{
	//initialise population with chromosomes consisting of random
	//weights and all fitnesses set to zero
	for (int i=0; i<mPopSize; ++i)
	{
		mPop.push_back(Genome());

		for (int j=0; j<mChromoLength; ++j)
		{
			mPop[i].weights.push_back(RandomClamped());
		}
	}
}


//---------------------------------Mutate--------------------------------
//
//	mutates a chromosome by perturbing its weights by an amount not 
//	greater than MAX_PERTURBATION
//-----------------------------------------------------------------------
void GenAlg::Mutate(vector<double> &chromo)
{
	//traverse the chromosome and mutate each weight dependent
	//on the mutation rate
	for (int i=0; i<chromo.size(); ++i)
	{
		//do we perturb this weight?
		if (RandFloat() < mMutationRate)
		{
			//add or subtract a small value to the weight
			chromo[i] += (RandomClamped() * MAX_PERTURBATION);
		}
	}
}

//----------------------------------GetChromoRoulette()------------------
//
//	returns a chromo based on roulette wheel sampling
//
//-----------------------------------------------------------------------
Genome GenAlg::GetChromoRoulette()
{
	//generate a random number between 0 & total fitness count
	double Slice = (double)(RandFloat() * mTotalFitness);

	//this will be set to the chosen chromosome
	Genome TheChosenOne;
	
	//go through the chromosones adding up the fitness so far
	double FitnessSoFar = 0;
	
	for (int i=0; i<mPopSize; ++i)
	{
		FitnessSoFar += mPop[i].fitness;
		
		//if the fitness so far > random number return the chromo at 
		//this point
		if (FitnessSoFar >= Slice)
		{
			TheChosenOne = mPop[i];

      break;
		}
		
	}

	return TheChosenOne;
}
	
//-------------------------------------Crossover()-----------------------
//	
//  given parents and storage for the offspring this method performs
//	crossover according to the GAs crossover rate
//-----------------------------------------------------------------------
void GenAlg::Crossover(const vector<double> &mum,
                        const vector<double> &dad,
                        vector<double>       &baby1,
                        vector<double>       &baby2)
{
	//just return parents as offspring dependent on the rate
	//or if parents are the same
	if ( (RandFloat() > mCrossoverRate) || (mum == dad)) 
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}

	//determine a crossover point
	int cp = RandInt(0, mChromoLength - 1);

	//create the offspring
	for (int i=0; i<cp; ++i)
	{
		baby1.push_back(mum[i]);
		baby2.push_back(dad[i]);
	}

	for (int i=cp; i<mum.size(); ++i)
	{
		baby1.push_back(dad[i]);
		baby2.push_back(mum[i]);
	}
	
	
	return;
}

//-----------------------------------Epoch()-----------------------------
//
//	takes a population of chromosones and runs the algorithm through one
//	 cycle.
//	Returns a new population of chromosones.
//
//-----------------------------------------------------------------------
vector<Genome> GenAlg::Epoch(vector<Genome> &old_pop)
{
	//assign the given population to the classes population
  mPop = old_pop;

  //reset the appropriate variables
  Reset();

  //sort the population (for scaling and elitism)
  sort(mPop.begin(), mPop.end());

  //calculate best, worst, average and total fitness
	CalculateBestWorstAvTot();
  
  //create a temporary vector to store new chromosones
	vector <Genome> vecNewPop;

	//Now to add a little elitism we shall add in some copies of the
	//fittest genomes. Make sure we add an EVEN number or the roulette
  //wheel sampling will crash
	if (!(NUM_COPIES_ELITE * NUM_ELITE % 2))
	{
		GrabNBest(NUM_ELITE, NUM_COPIES_ELITE, vecNewPop);
	}
	

	//now we enter the GA loop
	
	//repeat until a new population is generated
	while (vecNewPop.size() < mPopSize)
	{
		//grab two chromosones
		Genome mum = GetChromoRoulette();
		Genome dad = GetChromoRoulette();

		//create some offspring via crossover
		vector<double>		baby1, baby2;

		Crossover(mum.weights, dad.weights, baby1, baby2);

		//now we mutate
		Mutate(baby1);
		Mutate(baby2);

		//now copy into vecNewPop population
		vecNewPop.push_back(Genome(baby1, 0));
		vecNewPop.push_back(Genome(baby2, 0));
	}

	//finished so assign new pop back into m_vecPop
	mPop = vecNewPop;

	return mPop;
}


//-------------------------GrabNBest----------------------------------
//
//	This works like an advanced form of elitism by inserting NumCopies
//  copies of the NBest most fittest genomes into a population vector
//--------------------------------------------------------------------
void GenAlg::GrabNBest(int	            NBest,
                        const int	      NumCopies,
                        vector<Genome>	&Pop)
{
  //add the required amount of copies of the n most fittest 
	//to the supplied vector
	while(NBest--)
	{
		for (int i=0; i<NumCopies; ++i)
		{
			Pop.push_back(mPop[(mPopSize - 1) - NBest]);
	  }
	}
}

//-----------------------CalculateBestWorstAvTot-----------------------	
//
//	calculates the fittest and weakest genome and the average/total 
//	fitness scores
//---------------------------------------------------------------------
void GenAlg::CalculateBestWorstAvTot()
{
	mTotalFitness = 0;
	
	double HighestSoFar = 0;
	double LowestSoFar  = 9999999;
	
	for (int i=0; i<mPopSize; ++i)
	{
		//update fittest if necessary
		if (mPop[i].fitness > HighestSoFar)
		{
			HighestSoFar	 = mPop[i].fitness;
			
			mFittestGenome = i;

			mBestFitness	 = HighestSoFar;
		}
		
		//update worst if necessary
		if (mPop[i].fitness < LowestSoFar)
		{
			LowestSoFar = mPop[i].fitness;
			
			mWorstFitness = LowestSoFar;
		}
		
		mTotalFitness	+= mPop[i].fitness;
		
		
	}//next chromo
	
	mAverageFitness = mTotalFitness / mPopSize;
}

//-------------------------Reset()------------------------------
//
//	resets all the relevant variables ready for a new generation
//--------------------------------------------------------------
void GenAlg::Reset()
{
	mTotalFitness		= 0;
	mBestFitness		= 0;
	mWorstFitness		= 9999999;
	mAverageFitness		= 0;
}

