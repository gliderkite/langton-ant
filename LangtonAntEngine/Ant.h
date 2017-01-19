#pragma once


#include "Shape.h"


namespace lant
{
	/// <summary>
	/// The ant graphics.
	/// </summary>
	class Ant final : public Shape
	{
	public:



		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="pRenderTarget">Render target used to create the fill and the stroke.</param>
		Ant(ID2D1RenderTarget* pRenderTarget);


		/// <summary>
		/// Draws the graphic element.
		/// </summary>
		/// <param name="pRenderTarget">The render target.</param>
		/// <param name="worldTransform">The viewport matrix transformation.</param>
		/// <returns></returns>
		void draw(ID2D1RenderTarget* pRenderTarget, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept override;


		/// <summary>
		/// Move the ant by changing its transformation matrix via translation.
		/// </summary>
		/// <param name="dx">Delta abscissa.</param>
		/// <param name="dy">Delta ordinate.</param>
		/// <returns></returns>
		void move(float dx, float dy) noexcept;



	private:

		/// <summary>
		/// The organism's shape.
		/// </summary>
		D2D1_ELLIPSE shape;
	};
}