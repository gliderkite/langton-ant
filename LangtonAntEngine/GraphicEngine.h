#pragma once

#include "Graphic.h"

#include <vector>
#include <memory>


namespace lant
{
	/// <summary>
	/// Graphic engine.
	/// </summary>
	class GraphicEngine final
	{

	public:


		/// <summary>
		/// Returns true if there is an intersection between the two rectangles.
		/// </summary>
		/// <param name="r1">First rectangle.</param>
		/// <param name="r2">Second rectangle.</param>
		/// <returns></returns>
		constexpr static bool intersection(const D2D_RECT_F& r1, const D2D_RECT_F& r2) noexcept
		{
			return !(r2.left > r1.right || r2.right < r1.left || r2.top > r1.bottom || r2.bottom < r1.top);
		}

		/// <summary>
		/// Returns true if this node contains the given bounds.
		/// </summary>
		/// <param name="outer">Outer bounds.</param>
		/// <param name="inner">Inner bounds.</param>
		/// <returns></returns>
		constexpr static bool contains(const D2D_RECT_F& outer, const D2D_RECT_F& inner) noexcept
		{
			return (outer.left <= inner.left && outer.right >= inner.right && outer.top <= inner.top && outer.bottom >= inner.bottom);
		}



		/// <summary>
		/// Default constructor.
		/// </summary>
		GraphicEngine();

		/// <summary>
		/// Destructor.
		/// </summary>
		~GraphicEngine() noexcept;


		/// <summary>
		/// Delete move constructor.
		/// </summary>
		GraphicEngine(GraphicEngine&&) = delete;

		/// <summary>
		/// Delete move assignment operator.
		/// </summary>
		GraphicEngine& operator=(GraphicEngine&&) = delete;



		/// <summary>
		/// Initializes DirectX resources.
		/// </summary>
		/// <returns></returns>
		HRESULT initialize() noexcept;
		
		/// <summary>
		/// Creates the device resources.
		/// </summary>
		/// <param name="hwndHost">Host window.</param>
		/// <returns></returns>
		HRESULT create_device_resources(HWND hwndHost) noexcept;

		/// <summary>
		/// Resizes the swap chain.
		/// </summary>
		/// <param name="width">Width.</param>
		/// <param name="height">Height.</param>
		/// <returns>Returns the operation outcome.</returns>
		HRESULT resize(UINT width = 0, UINT height = 0) noexcept;

		/// <summary>
		/// Gets the device context.
		/// </summary>
		/// <returns></returns>
		ID2D1DeviceContext* get_device_context() const noexcept;

		/// <summary>
		/// Returns true only if the instance has been properly initialized,
		/// false otherwise.
		/// </summary>
		/// <returns></returns>
		bool is_valid() const noexcept;

		/// <summary>
		/// Refreshes the viewport.
		/// </summary>
		/// <returns>Returns the operation's outcome.</returns>
		HRESULT refresh(const std::vector<std::unique_ptr<Graphic>>& graphics) const noexcept;

		/// <summary>
		/// Reset world transformation.
		/// </summary>
		/// <returns></returns>
		void reset() noexcept;

		/// <summary>
		/// Sets the viewport offset.
		/// </summary>
		/// <param name="x">New abscissa.</param>
		/// <param name="y">New ordinate.</param>
		/// <returns></returns>
		void set_offset(float x, float y) noexcept;

		/// <summary>
		/// Gets the current viewport offset;
		/// </summary>
		/// <returns></returns>
		D2D1_POINT_2F get_offset() const noexcept;

		/// <summary>
		/// Sets the viewport zoom.
		/// </summary>
		/// <param name="zoom">Zoom.</param>
		/// <returns></returns>
		void set_zoom(float zoom) noexcept;

		/// <summary>
		/// Sets the viewport zoom.
		/// </summary>
		/// <param name="zoom">Zoom.</param>
		/// <param name="center">Zoom center point.</param>
		/// <returns></returns>
		void set_zoom(float zoom, const D2D1_POINT_2F& center) noexcept;

		/// <summary>
		/// Gets the current viewport zoom.
		/// </summary>
		/// <returns></returns>
		float get_zoom() const noexcept;

		/// <summary>
		/// Gets the viewport transformation.
		/// </summary>
		/// <returns></returns>
		D2D1_MATRIX_3X2_F get_world_transform() const noexcept;

		/// <summary>
		/// Returns true only if the viewport grid is visible.
		/// </summary>
		/// <returns></returns>
		bool is_grid_visible() const noexcept;

		/// <summary>
		/// Hides the viewport grid.
		/// </summary>
		/// <returns></returns>
		void hide_grid() noexcept;

		/// <summary>
		/// Shows the viewport grid.
		/// </summary>
		/// <returns></returns>
		void show_grid() noexcept;

		/// <summary>
		/// Gets the current visible area according to zoom and offset.
		/// </summary>
		/// <returns></returns>
		D2D1_RECT_F get_visible_area() const noexcept;




	private:



		/// <summary>
		/// Initialize factories.
		/// </summary>
		/// <returns></returns>
		HRESULT create_factories() noexcept;

		/// <summary>
		/// Initializes the Direct3D device.
		/// </summary>
		HRESULT create_d3d_device() noexcept;

		/// <summary>
		/// Initializes the Direct2D device.
		/// </summary>
		/// <returns></returns>
		HRESULT create_d2d_device() noexcept;

		/// <summary>
		/// Creates the swap chain for the given window
		/// </summary>
		/// <param name="hwnd">Pointer to the window.</param>
		/// <returns></returns>
		CComPtr<IDXGISwapChain1> create_swap_chain(HWND hwnd) const noexcept;

		/// <summary>
		/// Draw the grid.
		/// </summary>
		/// <param name="color">Stroke color.</param>
		/// <param name="space">Space between lines.</param>
		/// <param name="thickness">Stroke thickness.</param>
		/// <returns></returns>
		HRESULT draw_grid(const D2D1_COLOR_F& color, float space, float thickness) const noexcept;




		CComPtr<ID2D1Factory1> d2dFactory;				// Direct2D factory
		CComPtr<IDWriteFactory> writeFactory;			// DWrite factory
		CComPtr<IWICImagingFactory> imgFactory;			// Windows Imaging Component (WIC) factory
		CComPtr<ID3D11Device> d3dDevice;				// Direct3D device
		CComPtr<ID2D1Device> d2dDevice;					// Direct2D device
		CComPtr<ID3D11DeviceContext> immediateContext;	// Direct3D immediate context
		CComPtr<IDXGIAdapter> dxgiAdapter;				// DXGI adapter
		D3D_FEATURE_LEVEL FeatureLevel;					// Direct3D feature level
		D3D_DRIVER_TYPE DriverType;						// Direct3D driver type

		CComPtr<IDXGISwapChain1> swapChain;				// Swap Chain
		ID2D1DeviceContext* d2dContext;					// Direct2D device context
		ID2D1Bitmap1* d2dTargetBitmap;					// Direct2D target bitmap
		float dpiX, dpiY;								// Screen DPI

		float zoom;										// Current zoom
		D2D1_POINT_2F offset;							// Current offset

		bool isGridVisible;								// Flag true only if the viewport grid is visible
	};
}