#include "stdafx.h"
#include "LangtonAntEngine.h"
#include "Ant.h"
#include "Cell.h"
using namespace lant;
using namespace gengine;
using namespace std;



LAntEngine::LAntEngine(IntPtr hostPtr)
	: pGraphicEngine{ nullptr },
	hwndHost{ reinterpret_cast<HWND>(hostPtr.ToPointer()) },
	pGraphics{ nullptr },
	pAnt{ nullptr }
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

	// create the list of graphics
	pGraphics = new vector<unique_ptr<Graphic>>();
	pGraphics->reserve(0xFFFF);
	// add the ant
	pGraphics->emplace_back(new Ant(pGraphicEngine->get_device_context(), eDirection::Right, 10));
	pAnt = dynamic_cast<Ant*>(pGraphics->front().get());
	assert(pAnt);
	// create the cells map
	pCells = new map<pair<float, float>, Graphic*>();
}


LAntEngine::~LAntEngine()
{
	delete pGraphics;
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
	{
		if (pGraphics)
			return pGraphicEngine->refresh(*pGraphics);
	}

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


void LAntEngine::Step()
{
	if (!pAnt)
		return;

	/* The "ant" moves according to the rules below:
	At a white square, turn 90° right, flip the color of the square, move forward one unit
	At a black square, turn 90° left, flip the color of the square, move forward one unit */

	static const auto delta = 30.f;
	static const auto deltadiv2 = 15.f;

	// get the ant offset
	auto offset = pAnt->get_offset();
	offset.x -= deltadiv2;
	offset.y -= deltadiv2;
	const auto key = make_pair(offset.x, offset.y);
	const auto it = pCells->find(key);

	if (it == pCells->end())
	{
		// white (empty) square
		pAnt->turn_right();
		pAnt->move_forward(delta, delta);

		// add the cell to the map
		pGraphics->emplace(pGraphics->begin(), new Cell(pGraphicEngine->get_device_context(), offset, delta));
		const auto cell = dynamic_cast<Cell*>(pGraphics->front().get());
		assert(cell);
		pCells->emplace(key, cell);
	}
	else
	{
		// black square
		pAnt->turn_left();
		pAnt->move_forward(delta, delta);

		// find the cell graphic element
		auto i = pGraphics->begin();
		for (; i != pGraphics->end(); ++i)
		{
			if (i->get() == it->second)
				break;
		}

		assert(i != pGraphics->end());
		// remove the cell
		pGraphics->erase(i);
		pCells->erase(it);
	}
}


void LAntEngine::Reset()
{
	pGraphicEngine->reset();
	pCells->clear();

	if (pGraphics->size() > 1)
	{
		// remove all the cells but the ant
		pGraphics->erase(pGraphics->begin(), pGraphics->end() - 1);
	}

	assert(pAnt);
	pAnt->reset_transform();
}