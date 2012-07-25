#include "StateMenu.h"

#include "sparrowCore.h"
#include "sparrowPrimitives.h"
#include "gameDefines.h"

#define MENU_FONT_SIZE 48

StateMenu::StateMenu() : StateBase()
{
	choice = 0;
	fontDark = spFontLoad( FONT_GENERAL, MENU_FONT_SIZE );
	if ( fontDark )
		spFontAdd( fontDark, SP_FONT_GROUP_ASCII, spGetFastRGB( 0, 0, 0 ) );
	fontBright = spFontLoad( FONT_GENERAL, MENU_FONT_SIZE );
	if ( fontBright )
		spFontAdd( fontBright, SP_FONT_GROUP_ASCII, spGetFastRGB( 255, 255, 255 ) );

	addMenuEntry( "start game", stLevel );
	addMenuEntry( "show scores", stHighscores );
	addMenuEntry( "load replay", stReplayLoader );
#ifdef _DEBUG
	addMenuEntry( "collision test", stCollision );
#endif
	addMenuEntry( "exit", -1 );
}

StateMenu::~StateMenu()
{
	spFontDelete( fontDark );
	spFontDelete( fontBright );
}


///--- PUBLIC ------------------------------------------------------------------

int StateMenu::update(Uint32 delta)
{
	StateBase::update( delta );

	if ( spGetInput()->axis[1] < 0 )
		choice = std::min( --choice, entries.size()-1 );
	else if ( spGetInput()->axis[1] > 0 )
	{
		++choice;
		if ( choice > entries.size()-1 )
			choice = 0;
	}
	spGetInput()->axis[1] = 0;

	if ( spGetInput()->button[SP_BUTTON_B] ||
		 spGetInput()->button[SP_BUTTON_Y] ||
		 spGetInput()->button[SP_BUTTON_START] )
	{
		spResetButtonsState();
		return entries[choice].targetState;
	}

	return 0;
}

void StateMenu::render(SDL_Surface* target)
{
	spClearTarget( spGetRGB(128,0,0) );

	for ( int I = entries.size()-1; I >= 0; --I )
	{
		if ( choice == I )
		{
			spFontDrawRight( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT - MENU_FONT_SIZE * (I+1), -1,
							 entries[I].name.c_str(), fontBright );
		}
		else
		{
			spFontDrawRight( APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT - MENU_FONT_SIZE * (I+1), -1,
							 entries[I].name.c_str(), fontDark );
		}
	}
}

///--- PROTECTED ---------------------------------------------------------------

void StateMenu::addMenuEntry( const std::string &name, const int &targetState )
{
	MenuEntry entry = { name, targetState };
	entries.push_back( entry );
}

///--- PRIVATE -----------------------------------------------------------------
