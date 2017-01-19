#pragma once


#include "Graphic.h"
#include <atlbase.h>


namespace gengine
{
	/// <summary>
	/// Graphic element with fill.
	/// </summary>
	class FilledGraphic : public virtual Graphic
	{
	public:


		/// <summary>
		/// Default constructor.
		/// </summary>
		FilledGraphic() noexcept
			: fill{ nullptr }
		{
		}

		/// <summary>
		/// Destructor.
		/// </summary>
		virtual ~FilledGraphic() noexcept = default;


		/// <summary>
		/// Delete move constructor.
		/// </summary>
		FilledGraphic(FilledGraphic&&) = delete;

		/// <summary>
		/// Delete move assignment operator.
		/// </summary>
		FilledGraphic& operator=(FilledGraphic&&) = delete;



	protected:


		/// <summary>
		/// The brush used to paint the graphic element's interior.
		/// </summary>
		CComPtr<ID2D1Brush> fill;

	};
}