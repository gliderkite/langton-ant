#include "Stdafx.h"
#include "Ant.h"
using namespace lant;



Ant::Ant(ID2D1RenderTarget* pRenderTarget)
{
	shape = D2D1::Ellipse(D2D1::Point2F(60, 60), 10, 10);

	// create the fill
	ID2D1SolidColorBrush* pSolidBrush = nullptr;
	auto hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pSolidBrush);

	// check operation's outcome
	if (FAILED(hr))
		throw hr;

	fill = pSolidBrush;

	// create stroke
	hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pSolidBrush);

	// check operation's outcome
	if (FAILED(hr))
		throw hr;

	stroke = pSolidBrush;
}


void Ant::draw(ID2D1RenderTarget* pRenderTarget, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept
{
	assert(pRenderTarget);
	pRenderTarget->SetTransform(transform * worldTransform);

	if (fill)
		pRenderTarget->FillEllipse(shape, fill);
	if (stroke)
		pRenderTarget->DrawEllipse(shape, stroke);
}


void Ant::move(float dx, float dy) noexcept
{
	transform = transform * D2D1::Matrix3x2F::Translation(dx, dy);
}