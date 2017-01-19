#include "Stdafx.h"
#include "Ant.h"
using namespace lant;



Ant::Ant(ID2D1RenderTarget* pRenderTarget)
	: shape(D2D1::Ellipse(D2D1::Point2F(0, 0), 10, 10)),
	direction{ eDirection::Right }
{
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


void Ant::set_offset(float dx, float dy) noexcept
{
	transform = transform * D2D1::Matrix3x2F::Translation(dx, dy);
}


D2D1_POINT_2F Ant::get_offset() const noexcept
{
	return D2D1::Point2F(transform._31, transform._32);
}


eDirection Ant::get_direction() const noexcept
{
	return direction;
}


void Ant::turn_right() noexcept
{
	switch (direction)
	{
	case eDirection::Up:
		direction = eDirection::Right;
		break;
	case eDirection::Right:
		direction = eDirection::Down;
		break;
	case eDirection::Down:
		direction = eDirection::Left;
		break;
	case eDirection::Left:
		direction = eDirection::Up;
		break;
	}
}


void Ant::turn_left() noexcept
{
	switch (direction)
	{
	case eDirection::Up:
		direction = eDirection::Left;
		break;
	case eDirection::Right:
		direction = eDirection::Up;
		break;
	case eDirection::Down:
		direction = eDirection::Right;
		break;
	case eDirection::Left:
		direction = eDirection::Down;
		break;
	}
}


void Ant::move_forward(float dx, float dy) noexcept
{
	float signx = 0;
	float signy = 0;

	switch (direction)
	{
	case eDirection::Up:
		signy = 1;
		break;
	case eDirection::Right:
		signx = 1;
		break;
	case eDirection::Down:
		signy = -1;
		break;
	case eDirection::Left:
		signx = -1;
		break;	
	}

	set_offset(signx * dx, signy * dy);
}