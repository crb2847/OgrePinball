#ifndef __SinglePlayerState_h_
#define __SinglePlayerState_h_

#include "GameState.h"

class SinglePlayerState : public virtual GameState {
	public:
		SinglePlayerState(Paddle* p);
		~SinglePlayerState();
		void createState();
		void frameStartedState(const Ogre::FrameEvent& evt);
		void frameRenderState(const Ogre::FrameEvent& evt);
		void frameEnded(const Ogre::FrameEvent& evt);
		void keyPressedState(const OIS::KeyEvent& evt);
		void keyReleasedState(const OIS::KeyEvent& evt);


	Paddle *mPaddle;
};

#endif // #ifndef __SinglePlayerState_h_