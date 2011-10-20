#include "NeuralNet.h"
#include "Board.h"
#include <vector>

class AI
{
	
public:
	AI();
	bool GetLeft();
	bool GetRight();
	bool GetDown();
	bool GetRotate();
	bool Update(vector<double> inputs);
	void FindMoveSet(Board *gameBoard, int posX, int posY, int piece, int rotation);

private:
	bool mMoveLeft;
	bool mMoveRight;
	bool mMoveDown;
	bool mRotate;

	int mPlanningPosX, mPlanningPosY;				// Position of the piece that is falling down
	int mPlanningPiece, mPlanningRotation;			// Kind and rotation the piece that is falling down
	Board *mPlanningBoard;
	int mFewestEdges;
	int mLeftMoves;
	int mRightMoves;
	int mRotations;

	int mFitness;
	NeuralNet mItsBrain;
};