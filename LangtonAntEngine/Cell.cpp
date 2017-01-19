#include "Stdafx.h"
#include "Cell.h"
using namespace lant;



Cell::Cell(ID2D1RenderTarget* pRenderTarget, const D2D1_POINT_2F& pt, float size)
	: shape(D2D1::RectF(pt.x, pt.y, pt.x + size, pt.y + size))
{
	// create the fill
	ID2D1SolidColorBrush* pSolidBrush = nullptr;
	auto hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &pSolidBrush);

	// check operation's outcome
	if (FAILED(hr))
		throw hr;

	fill = pSolidBrush;

	// create stroke
	hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pSolidBrush);

	// check operation's outcome
	if (FAILED(hr))
		throw hr;

	stroke = pSolidBrush;
}


void Cell::draw(ID2D1RenderTarget* pRenderTarget, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept
{
	assert(pRenderTarget);
	pRenderTarget->SetTransform(transform * worldTransform);

	if (fill)
		pRenderTarget->FillRectangle(shape, fill);
	if (stroke)
		pRenderTarget->DrawRectangle(shape, stroke);
}
