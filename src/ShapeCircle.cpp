#include "ShapeCircle.h"

#include "ShapeRect.h"
#include "ShapeRay.h"
#include "UtilityFunctions.h"

#include <cmath>
#include "sparrowPrimitives.h"

ShapeCircle::ShapeCircle() :
	ShapeBase(),
	radius( 0.0f )
{
	//
}

ShapeCircle::ShapeCircle( const Vector2d<float> &newPos, const float &newRadius ) :
	ShapeBase( newPos ),
	radius( newRadius )
{
	//
}

ShapeCircle::~ShapeCircle()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

bool ShapeCircle::checkCollision( ShapeBase const *const other ) const
{
	return other->checkCollision( this );
}

bool ShapeCircle::checkCollision( ShapeRect const *const other ) const
{
	return other->checkCollision( this );
}

bool ShapeCircle::checkCollision( ShapeRay const *const other ) const
{
	return other->checkCollision( this );
}

bool ShapeCircle::checkCollision( ShapeCircle const *const other ) const
{
	if ( Utility::sqr<float>( other->pos.x - pos.x ) + Utility::sqr<float>( other->pos.y - pos.y ) <
			Utility::sqr<float>( radius + other->radius ) )
		return true;
	return false;
}

void ShapeCircle::render( SDL_Surface *target, Uint32 colour )
{
	spEllipseBorder( pos.x, pos.y, -1, radius, radius, 1, 1, colour );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
