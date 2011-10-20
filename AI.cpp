#include "AI.h"

#define NUM_OUTPUTS 4

//Used to define whether simple or complex scoring will be used (see the site for details)
//#define SIMPLE_SCORING

//Constructor
AI::AI()
   {
	   mMoveDown=0;
	   mMoveLeft=0;
	   mMoveRight=0;
	   mRotate=0;
   }

bool AI::GetDown()
{
	mMoveDown = true;
	return mMoveDown;
}

bool AI::GetLeft()
{
	if (mLeftMoves > 0) {
		mMoveLeft = true;
		mLeftMoves--;
	}
	else
		mMoveLeft = false;
	return mMoveLeft;
}

bool AI::GetRight()
{
	if (mRightMoves > 0) {
		mMoveRight = true;
		mRightMoves--;
	}
	else
		mMoveRight = false;
	return mMoveRight;
}

bool AI::GetRotate()
{
	if (mRotations > 0) {
		mRotate = true;
		mRotations--;
	}
	else
		mRotate = false;
	return mRotate;
}

bool AI::Update(vector<double> inputs)
{  
	//update the brain and get feedback
	vector<double> output = mItsBrain.Update(inputs);

	//make sure there were no errors in calculating the 
	//output
	if (output.size() < NUM_OUTPUTS) 
  {
    return false;
  }

	mMoveDown = output[0];
	mMoveLeft = output[1];
	mMoveRight = output[2];
	mRotate = output[3];
}

void AI::FindMoveSet(Board *gameBoard, int posX, int posY, int piece, int rotation){
	mFewestEdges = 999999;
	mLeftMoves = 0;
	mRightMoves = 0;
	mRotations = 0;
	for (int k=0;k<4; k++){ //Loop handles all possible rotations
		for (int i=0; i<BOARD_WIDTH/2; i++){ //Left hand side moves

			int tempLeftMoves=0;
			int tempRotations=0;
			//copies the current game board into a new planning board
			mPlanningBoard = new Board(*gameBoard);
			mPlanningPosX = posX;
			mPlanningPosY = posY;
			mPlanningPiece = piece;
			mPlanningRotation = rotation;

			//Do the moves for this particular test iteration
			for (int j=0; j<k; j++)
				if (mPlanningBoard->IsPossibleMovement (mPlanningPosX, mPlanningPosY, mPlanningPiece, (mPlanningRotation + 1) % 4))
				{mPlanningRotation = (mPlanningRotation + 1) % 4; tempRotations++;}
			for (int j=0; j<i; j++)
				if (mPlanningBoard->IsPossibleMovement (mPlanningPosX - 1, mPlanningPosY, mPlanningPiece, mPlanningRotation))
				{tempLeftMoves++; mPlanningPosX--;}
			while (mPlanningBoard->IsPossibleMovement(mPlanningPosX, mPlanningPosY, mPlanningPiece, mPlanningRotation)) { mPlanningPosY++; }
			mPlanningBoard->StorePiece (mPlanningPosX, mPlanningPosY - 1, mPlanningPiece, mPlanningRotation);

			int tempEdges = mPlanningBoard->GetBoardWeight();
			//Check to see if this iteration results in the fewest edges
			if(tempEdges < mFewestEdges) {
				mFewestEdges = tempEdges;
				mRightMoves = 0;
				mLeftMoves = tempLeftMoves;
				mRotations = tempRotations;
			}

			//Free that memory before we end up creating a new board
			delete mPlanningBoard;
			
		}
		for (int i=0; i<BOARD_WIDTH/2; i++){

			
			int tempRightMoves=0;
			int tempRotations=0;
			//copies the current game board into a new planning board
			mPlanningBoard = new Board(*gameBoard);
			mPlanningPosX = posX;
			mPlanningPosY = posY;
			mPlanningPiece = piece;
			mPlanningRotation = rotation;

			//Do the moves for this particular test iteration
			for (int j=0; j<k; j++)
				if (mPlanningBoard->IsPossibleMovement (mPlanningPosX, mPlanningPosY, mPlanningPiece, (mPlanningRotation + 1) % 4))
				{mPlanningRotation = (mPlanningRotation + 1) % 4; tempRotations++;}
			for (int j=0; j<i; j++)
				if (mPlanningBoard->IsPossibleMovement (mPlanningPosX + 1, mPlanningPosY, mPlanningPiece, mPlanningRotation))
				{tempRightMoves++; mPlanningPosX++;}
			while (mPlanningBoard->IsPossibleMovement(mPlanningPosX, mPlanningPosY, mPlanningPiece, mPlanningRotation)) { mPlanningPosY++; }
			mPlanningBoard->StorePiece (mPlanningPosX, mPlanningPosY - 1, mPlanningPiece, mPlanningRotation);

			int tempEdges = mPlanningBoard->GetBoardWeight();
			//Check to see if this iteration results in the fewest edges
			if(tempEdges < mFewestEdges) {
				mFewestEdges = tempEdges;
				mLeftMoves = 0;
				mRightMoves = tempRightMoves;
				mRotations = tempRotations;
			}

			//Free that memory before we end up creating a new board
			delete mPlanningBoard;

		
		}
	}

}