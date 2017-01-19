#pragma once


#include <d2d1_2.h>



namespace gengine
{
	/// <summary>
	/// Base graphic element.
	/// </summary>
	class Graphic
	{
	public:


		/// <summary>
		/// Default constructor.
		/// </summary>
		Graphic() noexcept
			: transform(D2D1::IdentityMatrix())
		{
		}

		/// <summary>
		/// Destructor.
		/// </summary>
		virtual ~Graphic() noexcept = default;


		/// <summary>
		/// Draws the graphic element.
		/// </summary>
		/// <param name="pRenderTarget">The render target.</param>
		/// <param name="worldTransform">The viewport matrix transformation.</param>
		/// <returns></returns>
		virtual void draw(ID2D1RenderTarget* pRenderTarget, const D2D1_MATRIX_3X2_F& worldTransform) const noexcept = 0;



	protected:


		/// <summary>
		/// Matrix transformation of the graphic element.
		/// </summary>
		D2D1_MATRIX_3X2_F transform;

	};
}