#pragma once
using namespace System;
using namespace System::Runtime::InteropServices;

#include "GraphicEngine.h"
#include "Ant.h"

#include <map>
#include <utility>


namespace lant 
{
	/// <summary>
	/// Langton's ant engine.
	/// </summary>
	/// <remarks>https://en.wikipedia.org/wiki/Langton's_ant</remarks>
	public ref class LAntEngine sealed
	{

	public:

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="hwndHost">Handle to the window which hosts the viewport.</param>
		explicit LAntEngine(IntPtr hostPtr);


		/// <summary>
		/// Destructor.
		/// </summary>
		~LAntEngine();


		/// <summary>
		/// Resize the viewport.
		/// </summary>
		/// <param name="width">Viewport width.</param>
		/// <param name="height">Viewport height.</param>
		/// <returns>Returns the operation outcome.</returns>
		/// <remarks>Passing zeros allows the engine to automatically resize the
		/// viewport host window.</remarks>
		HRESULT Resize();

		/// <summary>
		/// Resize the viewport.
		/// </summary>
		/// <param name="width">Viewport width.</param>
		/// <param name="height">Viewport height.</param>
		/// <returns>Returns the operation outcome.</returns>
		/// <remarks>Passing zeros allows the engine to automatically resize the
		/// viewport host window.</remarks>
		HRESULT Resize(UINT width, UINT height);

		/// <summary>
		/// Refreshes the viewport.
		/// </summary>
		/// <returns>Returns the operation's outcome.</returns>
		HRESULT Refresh();

		/// <summary>
		/// Clear the viewport.
		/// </summary>
		void ClearViewport();

		/// <summary>
		/// Sets the viewport offset.
		/// </summary>
		/// <param name="x">New abscissa.</param>
		/// <param name="y">New ordinate.</param>
		void SetOffset(float x, float y);

		/// <summary>
		/// Gets the current viewport offset.
		/// </summary>
		/// <param name="x">Abscissa.</param>
		/// <param name="y">Ordinate.</param>
		void GetOffset([Out] float% x, [Out] float% y);

		/// <summary>
		/// Sets the viewport zoom.
		/// </summary>
		/// <param name="zoom">Zoom.</param>
		/// <param name="center">Center point.</param>
		void SetZoom(float zoom, float cx, float cy);

		/// <summary>
		/// Gets the current viewport offset.
		/// </summary>
		/// <returns></returns>
		float GetZoom();

		/// <summary>
		/// Go to the next generation step.
		/// </summary>
		void Step();




	private:


		/// <summary>
		/// Native pointer to the graphic engine.
		/// </summary>
		gengine::GraphicEngine* pGraphicEngine;

		/// <summary>
		/// Viewport host window.
		/// </summary>
		HWND hwndHost;

		/// <summary>
		/// Native pointer to the list of graphics.
		/// </summary>
		std::vector<std::unique_ptr<gengine::Graphic>>* pGraphics;

		/// <summary>
		/// Native pointer to the ant graphics.
		/// </summary>
		Ant* pAnt;

		/// <summary>
		/// Map of cells. The key is the cell location.
		/// </summary>
		std::map<std::pair<float, float>, gengine::Graphic*>* pCells;

	};
}
