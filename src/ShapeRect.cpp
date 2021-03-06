#include "ShapeRect.h"

#include "ShapeRay.h"
#include "ShapeCircle.h"
#include "UtilityFunctions.h"

#include "sparrowPrimitives.h"

ShapeRect::ShapeRect() :
	ShapeBase(),
	size( 0, 0 )
{
	//
}

ShapeRect::ShapeRect( const Vector2d<float> &newPos, const Vector2d<float> &newSize ) :
	ShapeBase( newPos ),
	size( newSize )
{
	//
}

ShapeRect::~ShapeRect()
{
	//
}


///--- PUBLIC ------------------------------------------------------------------

bool ShapeRect::checkCollision( ShapeBase const *const other ) const
{
	return other->checkCollision( this );
}

bool ShapeRect::checkCollision( ShapeRect const *const other ) const
{
	if ( ( ( pos.x - size.x / 2.0f < other->pos.x + other->size.x / 2.0f ) &&
			( other->pos.x - other->size.x / 2.0f < pos.x + size.x / 2.0f ) ) &&
			( ( pos.y - size.y / 2.0f < other->pos.y + other->size.y / 2.0f ) &&
			  ( other->pos.y - other->size.y / 2.0f < pos.y + size.y / 2.0f ) ) )
		return true;
	return false;
}

bool ShapeRect::checkCollision( ShapeRay const *const other ) const
{
	return other->checkCollision( this );
}

void ShapeRect::render( SDL_Surface *target, Uint32 colour )
{
	spRectangleBorder( pos.x, pos.y, -1, size.x, size.y, 1, 1, colour );
	Vector2d<float> c1 = pos - size / 2.0f;
	Vector2d<float> c2 = pos + size / 2.0f;
	spLine( c1.x, c1.y, -1, c2.x, c2.y, -1, colour );
	spLine( c2.x, c1.y, -1, c1.x, c2.y, -1, colour );
}

bool ShapeRect::checkCollision( ShapeCircle const *const other ) const
{
	Vector2d<float> closestPoint( Utility::clamp( other->pos.x, pos.x - size.x / 2.0f, pos.x + size.x / 2.0f ),
								  Utility::clamp( other->pos.y, pos.y - size.y / 2.0f, pos.y + size.y / 2.0f ));
	Vector2d<float> distance = other->pos - closestPoint;

	return distance.lengthSquared() < Utility::sqr( other->radius );
}

///--- PROTECTED ---------------------------------------------------------------

///--- PRIVATE -----------------------------------------------------------------
