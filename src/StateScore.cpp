#include "StateScore.h"

#include "StateLevel.h"
#include "gameDefines.h"
#include "sparrowCore.h"
#include "UtilityFunctions.h"

#include <time.h>

#define SCORE_FONT_SIZE 32
#define SCORE_CARET_BLINK_TIME 600

StateScore::StateScore( StateLevel *level ) :
	StateBase(),
	file( FOLDER_DATA "/" FILE_HIGHSCORE_NORMAL )
{
	SDL_Surface *temp = spCreateSurface( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT );
	killFrame = spCreateSurface( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT );
	spSelectRenderTarget( temp );
	level->render( temp );
	SDL_SetAlpha( temp, SDL_SRCALPHA, 128 );
	SDL_BlitSurface( temp, NULL, killFrame, NULL );
	spSelectRenderTarget( spGetWindowSurface() );
	spDeleteSurface( temp );

	score = level->scoreKeeper.getScore();
	scoreText = spFontLoad( FONT_GENERAL, SCORE_FONT_SIZE );
	if ( scoreText )
	{
		spFontAdd( scoreText, SP_FONT_GROUP_ALPHABET SP_FONT_GROUP_GERMAN ".:!\"_", -1 );
		spFontAdd( scoreText, SP_FONT_GROUP_NUMBERS, spGetRGB( 255, 128, 0 ) );
	}

	if ( level->run->playing )
	{
		state = 1;
		caret = false;
		strcpy( name, level->run->info.name.c_str() );
		run = level->run;
		level->run = NULL;
	}
	else
	{
		caret = true;
		state = 0;
		strcpy( name, "player" );
		spPollKeyboardInput( name, 100, NULL );
		run = level->run;
		level->run = NULL;
	}

	spGetInput()->button[SP_BUTTON_START] = 0;

	caretTimer.start( SCORE_CARET_BLINK_TIME );
	timers.push_back( &caretTimer );
	timecode = time( NULL );

	type = stScore;
}

StateScore::~StateScore()
{
	spDeleteSurface( killFrame );
	spFontDelete( scoreText );
	delete run;
}


///--- PUBLIC ------------------------------------------------------------------

int StateScore::update( Uint32 delta )
{
	StateBase::update( delta );

	if ( caretTimer.isStopped() && state == 0 )
	{
		caret = !caret;
		caretTimer.start( SCORE_CARET_BLINK_TIME );
	}

	if ( spGetInput()->button[SP_BUTTON_START] )
	{
		if ( state == 0 )
		{
			spGetInput()->button[SP_BUTTON_START] = 0;
			spStopKeyboardInput( );
			state = 1;
			caret = false;
			file.addScore( name, score, timecode );
			if ( run )
			{
				run->info.name = name;
				run->info.score = score;
				run->info.timecode = timecode;
				run->info.version = REPLAY_VERSION;
				run->saveToFile( FOLDER_REPLAY "/" + Utility::numToStr( timecode ) +
									EXTENSION_REPLAY );
			}

		}
		else
		{
			spGetInput()->button[SP_BUTTON_START] = 0;
			return stMenu;
		}
	}

	return 0;
}

void StateScore::render( SDL_Surface *target )
{
	SDL_BlitSurface( killFrame, NULL, spGetWindowSurface(), NULL );
	if ( scoreText )
	{
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 - SCORE_FONT_SIZE * 2, -1, "You died!", scoreText );
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 - SCORE_FONT_SIZE, -1, ("Score:  " + Utility::numToStr( score )).c_str(), scoreText );
		if ( run && run->playing )
			spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 , -1, "Name of this player:", scoreText );
		else
			spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 , -1, "Enter your name:", scoreText );
		char temp[strlen(name) + caret];
		strcpy( temp, name );
		if ( caret )
			strcat( temp, "_\0" );
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 + SCORE_FONT_SIZE, -1, temp, scoreText);
		spFontDrawMiddle( APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 + SCORE_FONT_SIZE * 3, -1, "Press \""SP_BUTTON_START_NAME"\" to go again...", scoreText );
	}

}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
