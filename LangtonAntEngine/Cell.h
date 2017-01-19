#pragma once


#include "Shape.h"


namespace lant
{
	/// <summary>
	/// The ant graphics.
	/// </summary>
	class Cell final : public Shape
	{
	public:



		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="pRenderTarget">Render target used to create the fill and the stroke.</param>
		/// <param name="pt">Offset.</param>
		/// <param name="size">Cell size.</param>
		explicit Cell(ID2D1RenderTarget* pRenderTarget, const D2D1_POINT_2F& pt, float size);


		/// <summary>
		/// Draws the graphic element.
		/// </summary>
		/// <param name="pRenderTarget">The render target.</param>
		/// <param name="worldTransform">The viewport matrix transformation.</param>
		/// <returns></returns>
		void draw(ID2D1RenderTarget* pRenderTarget, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept override;




	private:

		/// <summary>
		/// The cell shape.
		/// </summary>
		D2D1_RECT_F shape;
	};
}