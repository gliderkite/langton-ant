#pragma once


#include "Graphic.h"
#include <atlbase.h>


namespace lant
{
	/// <summary>
	/// Graphic element with stroke.
	/// </summary>
	class StrokedGraphic : public virtual Graphic
	{
	public:


		/// <summary>
		/// Default constructor.
		/// </summary>
		StrokedGraphic() noexcept
			: stroke{ nullptr }
		{
		}

		/// <summary>
		/// Destructor.
		/// </summary>
		virtual ~StrokedGraphic() noexcept = default;


		/// <summary>
		/// Delete move constructor.
		/// </summary>
		StrokedGraphic(StrokedGraphic&&) = delete;

		/// <summary>
		/// Delete move assignment operator.
		/// </summary>
		StrokedGraphic& operator=(StrokedGraphic&&) = delete;



	protected:


		/// <summary>
		/// The brush used to paint the graphic element's stroke.
		/// </summary>
		CComPtr<ID2D1Brush> stroke;

	};
}