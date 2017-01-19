#include "Stdafx.h"
#include "Shape.h"
#include "GraphicEngine.h"
using namespace gengine;



bool Shape::intersects(const D2D1_RECT_F& rect, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept
{
	return GraphicEngine::intersection(get_bounds(), rect);
}


bool Shape::contains(const D2D1_RECT_F& rect, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept
{
	return GraphicEngine::contains(get_bounds(), rect);
}