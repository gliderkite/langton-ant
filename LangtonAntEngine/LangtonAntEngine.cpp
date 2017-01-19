#include "stdafx.h"
#include "LangtonAntEngine.h"
using namespace lant;



LAntEngine::LAntEngine(IntPtr hostPtr)
	: pGraphicEngine{ nullptr },
	hwndHost{ reinterpret_cast<HWND>(hostPtr.ToPointer()) }
{
	// create and initialize the graphic engine
	pGraphicEngine = new GraphicEngine();
	auto hres = pGraphicEngine->initialize();

	// check if initialization failed
	if (FAILED(hres))
		throw gcnew ExternalException("Graphic engine initialization error", hres);

	// create DirectX device resources
	assert(pGraphicEngine->is_valid());
	hres = pGraphicEngine->create_device_resources(hwndHost);

	// check if initialization failed
	if (FAILED(hres))
		throw gcnew ExternalException("Graphic engine initialization error", hres);
}



LAntEngine::~LAntEngine()
{
}



HRESULT LAntEngine::Resize()
{
	if (pGraphicEngine)
		return pGraphicEngine->resize();

	return E_FAIL;
}


HRESULT LAntEngine::Resize(UINT width, UINT height)
{
	if (pGraphicEngine)
		return pGraphicEngine->resize(width, height);

	return E_FAIL;
}


HRESULT LAntEngine::Refresh()
{
	if (pGraphicEngine)
		return pGraphicEngine->refresh();

	return E_FAIL;
}


void LAntEngine::ClearViewport()
{
	if (pGraphicEngine)
		pGraphicEngine->reset();
}


void LAntEngine::SetOffset(float x, float y)
{
	if (pGraphicEngine)
		pGraphicEngine->set_offset(x, y);
}


void LAntEngine::GetOffset(float% x, float% y)
{
	if (pGraphicEngine)
	{
		const auto offset = pGraphicEngine->get_offset();
		x = offset.x;
		y = offset.y;
	}
	else
		x = y = 0;
}


void LAntEngine::SetZoom(float zoom, float cx, float cy)
{
	if (pGraphicEngine)
		pGraphicEngine->set_zoom(zoom, D2D1::Point2F(cx, cy));
}


float LAntEngine::GetZoom()
{
	if (pGraphicEngine)
		return pGraphicEngine->get_zoom();

	return 1.f;
}


bool LAntEngine::IsGridVisible()
{
	if (pGraphicEngine)
		return pGraphicEngine->is_grid_visible();

	return false;
}


void LAntEngine::HideGrid()
{
	if (pGraphicEngine)
		return pGraphicEngine->hide_grid();
}


void LAntEngine::ShowGrid()
{
	if (pGraphicEngine)
		return pGraphicEngine->show_grid();
}




void LAntEngine::Step()
{

}