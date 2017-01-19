#pragma once


#include "FilledGraphic.h"
#include "StrokedGraphic.h"



namespace lant
{
	/// <summary>
	/// Basic graphic element shape with fill and stroke.
	/// </summary>
	class Shape : public FilledGraphic, public StrokedGraphic
	{
	public:

		/// <summary>
		/// Returns true only if the shape intersects the given rectangle, otherwise returns false.
		/// </summary>
		/// <param name="rect">Rectangle.</param>
		/// <param name="worldTransform">World matrix transformation.</param>
		/// <returns></returns>
		virtual bool intersects(const D2D1_RECT_F& rect, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept;

		/// <summary>
		/// Returns true only if the shape contains the given rectangle, otherwise returns false.
		/// </summary>
		/// <param name="rect">Rectangle.</param>
		/// <param name="worldTransform">World matrix transformation.</param>
		/// <returns></returns>
		virtual bool contains(const D2D1_RECT_F& rect, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept;

		/// <summary>
		/// Gets the shape bounds.
		/// </summary>
		/// <returns></returns>
		virtual D2D1_RECT_F get_bounds() const noexcept
		{
			return D2D1::RectF();
		}
	};
}