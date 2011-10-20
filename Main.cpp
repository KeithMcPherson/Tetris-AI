/*****************************************************************************************
/* Desc: Tetris tutorial
/*
/* gametuto.com - Javier López López (javilop.com)
/*
/*****************************************************************************************
/*
/* Creative Commons - Attribution 3.0 Unported
/* You are free:
/*	to Share — to copy, distribute and transmit the work
/*	to Remix — to adapt the work
/*
/* Under the following conditions:
/* Attribution. You must attribute the work in the manner specified by the author or licensor 
/* (but not in any way that suggests that they endorse you or your use of the work).
/*
/*****************************************************************************************/

#include "Game.h"
#ifndef LINUX
#include <windows.h>
#include <iostream>
#endif

//will enable AI, otherwise will be controlled by the user
#define ENABLE_AI

/*
==================
Main
==================
*/
#ifndef LINUX
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)	// Linux users should quit this line
#else
int main()
#endif
// int main ()  // Linux users should use this line instead the previous one
{
	// ----- Vars -----

	// Class for drawing staff, it uses SDL for the rendering. Change the methods of this class
	// in order to use a different renderer
	IO mIO;
	int mScreenHeight = mIO.GetScreenHeight();

	// The computer
	AI mAI;

	// Pieces
	Pieces mPieces;

	// Board
	Board mBoard (&mPieces, mScreenHeight);

	// Game
	Game mGame (&mBoard, &mPieces, &mIO, mScreenHeight);

	// Get the actual clock milliseconds (SDL)
	unsigned long mTime1 = SDL_GetTicks();

	// ----- Main Loop -----

	while (!mIO.IsKeyDown (SDLK_ESCAPE))
	{
		// ----- Draw -----
		mIO.ClearScreen (); 		// Clear screen
		mGame.DrawScene ();			// Draw staff
		mIO.UpdateScreen ();		// Put the graphic context in the screen

		// ----- Input -----

		int mKey = mIO.PollKey();

		#ifdef ENABLE_AI
		vector<double> inputs = mBoard.GetBoardNNInputs();

		//mAI.Update(inputs);
		mAI.FindMoveSet(&mBoard, mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation);

			if (mAI.GetDown())
				mGame.SetSoftDrop(true);
			if (mAI.GetLeft())
				if (mBoard.IsPossibleMovement (mGame.mPosX - 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
					mGame.mPosX--;
			if (mAI.GetRight())
				if (mBoard.IsPossibleMovement (mGame.mPosX + 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
					mGame.mPosX++;
			if (mAI.GetRotate())
				if (mBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY, mGame.mPiece, (mGame.mRotation + 1) % 4))
					mGame.mRotation = (mGame.mRotation + 1) % 4;
		#endif
		#ifndef ENABLE_AI
		switch (mKey)
		{
			case (SDLK_RIGHT): 
			{
				if (mBoard.IsPossibleMovement (mGame.mPosX + 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
					mGame.mPosX++;
					break;
			}

			case (SDLK_LEFT): 
			{
				if (mBoard.IsPossibleMovement (mGame.mPosX - 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
					mGame.mPosX--;	
				break;
			}

			case (SDLK_DOWN):
			{	
				mGame.SetSoftDrop(true);
				break;
			}

			//FIX THIS NEGATIVE TO SOMETHING THAT ISN'T TERRIBLE
			case (-SDLK_DOWN):
			{	
				mGame.SetSoftDrop(false);
				break;
			}

			case (SDLK_x):
			{
				// Check collision from up to down
				while (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation)) { mGame.mPosY++; }
	
				mBoard.StorePiece (mGame.mPosX, mGame.mPosY - 1, mGame.mPiece, mGame.mRotation);

				mGame.IncreaseScore(mBoard.DeletePossibleLines ());

				if (mBoard.IsGameOver())
				{
					mIO.Getkey();
					exit(0);
				}

				mGame.CreateNewPiece();

				break;
			}

			case (SDLK_z):
			{
				if (mBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY, mGame.mPiece, (mGame.mRotation + 1) % 4))
					mGame.mRotation = (mGame.mRotation + 1) % 4;

				break;
			}
		}
		#endif

		// ----- Vertical movement -----

		unsigned long mTime2 = SDL_GetTicks();

		int waitTime = mGame.GetDropSpeed();
		if (mGame.IsSoftDrop())
			waitTime = 15;

		if ((mTime2 - mTime1) > waitTime)
		{
			if (mGame.IsSoftDrop()) {
				mGame.IncreaseScore(1);
			}
			if (mBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
			{
				mGame.mPosY++;
			}
			else
			{
				mBoard.StorePiece (mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation);

				mGame.IncreaseScore(mBoard.DeletePossibleLines ());

				if (mBoard.IsGameOver())
				{
					mIO.Getkey();
					mBoard.InitBoard();
					mGame.ResetScore();
				}
				
				mGame.CreateNewPiece();
			}

			mTime1 = SDL_GetTicks();
		}
	}

	return 0;
}
