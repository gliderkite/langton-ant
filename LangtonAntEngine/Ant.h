#pragma once


#include "Shape.h"
#include "Direction.h"


namespace lant
{
	/// <summary>
	/// The ant graphics.
	/// </summary>
	class Ant final : public gengine::Shape
	{
	public:



		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="pRenderTarget">Render target used to create the fill and the stroke.</param>
		explicit Ant(ID2D1RenderTarget* pRenderTarget);


		/// <summary>
		/// Draws the graphic element.
		/// </summary>
		/// <param name="pRenderTarget">The render target.</param>
		/// <param name="worldTransform">The viewport matrix transformation.</param>
		/// <returns></returns>
		void draw(ID2D1RenderTarget* pRenderTarget, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept override;
		
		/// <summary>
		/// Sets the ant offset.
		/// </summary>
		/// <param name="dx">Delta abscissa.</param>
		/// <param name="dy">Delta ordinate.</param>
		/// <returns></returns>
		void set_offset(float dx, float dy) noexcept;

		/// <summary>
		/// Gets the current ant offset.
		/// </summary>
		/// <returns></returns>
		D2D1_POINT_2F get_offset() const noexcept;

		/// <summary>
		/// Gets the current ant direction.
		/// </summary>
		/// <returns></returns>
		eDirection get_direction() const noexcept;

		/// <summary>
		/// Turns right.
		/// </summary>
		/// <returns></returns>
		void turn_right() noexcept;

		/// <summary>
		/// Turns left.
		/// </summary>
		/// <returns></returns>
		void turn_left() noexcept;

		/// <summary>
		/// Move forward according to the ant offset and direction.
		/// </summary>
		/// <param name="dx">Delta abscissa.</param>
		/// <param name="dy">Delta ordinate.</param>
		/// <returns></returns>
		void move_forward(float dx, float dy) noexcept;



	private:

		/// <summary>
		/// The ant shape.
		/// </summary>
		D2D1_ELLIPSE shape;

		/// <summary>
		/// Current ant direction.
		/// </summary>
		eDirection direction;
	};
}