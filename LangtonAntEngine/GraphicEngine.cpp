#include "stdafx.h"
#include "GraphicEngine.h"
using namespace lant;
using namespace std;




GraphicEngine::GraphicEngine()
	: d2dFactory(nullptr),
	writeFactory(nullptr),
	imgFactory(nullptr),
	d3dDevice(nullptr),
	immediateContext(nullptr),
	d2dDevice(nullptr),
	dxgiAdapter(nullptr),
	d2dContext(nullptr),
	DriverType(D3D_DRIVER_TYPE_NULL),
	FeatureLevel(D3D_FEATURE_LEVEL_11_0),
	swapChain{ nullptr },
	d2dTargetBitmap{ nullptr },
	dpiX{ 96.f },
	dpiY{ 96.f },
	zoom(1.f),
	offset(D2D1::Point2F()),
	isGridVisible(false)
{	
}


GraphicEngine::~GraphicEngine() noexcept
{
	if (d2dTargetBitmap)
		d2dTargetBitmap->Release();
	if (d2dContext)
		d2dContext->Release();

	if (immediateContext)
		immediateContext->ClearState();

	CoUninitialize();
}


HRESULT GraphicEngine::initialize() noexcept
{
	// check if the istance has been already initialized
	if (is_valid())
		return S_OK;

	// initializes the COM library
	auto hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_SPEED_OVER_MEMORY);

	if (FAILED(hr))
		return hr;

	hr = create_factories();

	if (SUCCEEDED(hr))
	{
		hr = create_d3d_device();

		if (SUCCEEDED(hr))
		{
			hr = create_d2d_device();

			if (SUCCEEDED(hr))
				return hr;
		}
	}

	// at this point something went wrong
	assert(!is_valid());

	return hr;
}


CComPtr<IDXGISwapChain1> GraphicEngine::create_swap_chain(HWND hwnd) const noexcept
{
	if (!hwnd || !IsWindow(hwnd) || !is_valid())
		return nullptr;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// Allocate a descriptor.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	// Get the factory object that created the DXGI device.
	IDXGIFactory2* dxgiFactory;
	auto hr = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

	if (FAILED(hr))
		return nullptr;

	IDXGISwapChain1* swapChain = nullptr;
	assert(d3dDevice);

	// Get the final swap chain for this window from the DXGI factory.
	hr = dxgiFactory->CreateSwapChainForHwnd(
		d3dDevice,
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,		// allow on all displays
		&swapChain);

	if (FAILED(hr))
		return nullptr;

	// Doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	dxgiFactory->Release();

	return swapChain;
}


bool GraphicEngine::is_valid() const noexcept
{
	return d2dFactory && writeFactory && imgFactory &&
		d3dDevice && immediateContext && d2dDevice && dxgiAdapter;
}


HRESULT GraphicEngine::create_factories() noexcept
{
	assert(!is_valid());

	// Initialize device-indpendent resources, such as the Direct2D factory.
	auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &d2dFactory);

	if (SUCCEEDED(hr))
	{
		// create DirectWrite factory
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&writeFactory));

		if (SUCCEEDED(hr))
		{
			// create WIC factory
			hr = CoCreateInstance(
				CLSID_WICImagingFactory,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory,
				(LPVOID*)&imgFactory);
		}
	}

	return hr;
}


HRESULT GraphicEngine::create_d3d_device() noexcept
{
	assert(!is_valid());
	HRESULT hr = S_OK;

	// This flag adds support for surfaces with a different color channel ordering than the API default.
	// You need it for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	const auto nDriverTypes = ARRAYSIZE(driverTypes);

	// This array defines the set of DirectX hardware feature levels this app  supports.
	// The ordering is important and you should  preserve it.
	// Don't forget to declare your app's minimum required feature level in its
	// description.  All apps are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	const auto nFeatureLevels = ARRAYSIZE(featureLevels);

	for (auto i = 0; i < nDriverTypes; i++)
	{
		DriverType = driverTypes[i];

		// Create the DX11 API device object, and get a corresponding context
		hr = D3D11CreateDevice(
			nullptr,                    // specify null to use the default adapter
			DriverType,
			nullptr,
			creationFlags,              // optionally set debug and Direct2D compatibility flags
			featureLevels,              // list of feature levels this app can support
			nFeatureLevels,				// number of possible feature levels
			D3D11_SDK_VERSION,
			&d3dDevice,                 // returns the Direct3D device created
			&FeatureLevel,				// returns feature level of device created
			&immediateContext           // returns the device immediate context
		);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(
				nullptr,
				DriverType,
				nullptr,
				creationFlags,
				&featureLevels[1],
				nFeatureLevels - 1,
				D3D11_SDK_VERSION,
				&d3dDevice,
				&FeatureLevel,
				&immediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}

	return hr;
}


HRESULT GraphicEngine::create_d2d_device() noexcept
{
	assert(!is_valid());

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIDevice* dxgiDevice = nullptr;

	assert(d3dDevice);
	auto hr = d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));

	if (SUCCEEDED(hr))
	{
		// Obtain the Direct2D device for 2-D rendering
		assert(d2dFactory);
		hr = d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);

		if (SUCCEEDED(hr))
		{
			// Identify the physical adapter (GPU or card) this device is runs on.
			hr = dxgiDevice->GetAdapter(&dxgiAdapter);

			if (SUCCEEDED(hr))
				hr = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &d2dContext);
		}

		dxgiDevice->Release();
	}

	return hr;
}


HRESULT GraphicEngine::create_device_resources(HWND hwndHost) noexcept
{
	if (!hwndHost || !IsWindow(hwndHost) || !is_valid())
		return E_FAIL;

	swapChain = create_swap_chain(hwndHost);

	if (!swapChain)
		return E_FAIL;

	// init DPI
	assert(d2dFactory);
	d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

	// Now we set up the Direct2D render target bitmap linked to the swapchain. 
	// Whenever we render to this bitmap, it is directly rendered to the 
	// swap chain associated with the window.
	const auto bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		dpiX,
		dpiY);

	// Direct2D needs the dxgi version of the backbuffer surface pointer.
	IDXGISurface* dxgiBackBuffer;
	auto hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

	if (FAILED(hr))
		return hr;

	// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
	hr = d2dContext->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer,
		&bitmapProperties,
		&d2dTargetBitmap);

	dxgiBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	// Now we can set the Direct2D render target.
	d2dContext->SetTarget(d2dTargetBitmap);

	return hr;
}


HRESULT GraphicEngine::resize(UINT width, UINT height) noexcept
{
	if (!swapChain)
		return E_FAIL;

	assert(is_valid());

	// release previous resources
	d2dTargetBitmap->Release();
	d2dContext->Release();
	
	if (immediateContext)
		immediateContext->ClearState();

	// Changes the swap chain's back buffer size, format, and number of buffers
	HRESULT hr = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

	if (FAILED(hr))
		return hr;

	// Get Direct2D device's corresponding device context object.
	hr = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext);

	if (FAILED(hr))
		return hr;

	// init DPI
	assert(d2dFactory);
	d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

	// Now we set up the Direct2D render target bitmap linked to the swapchain. 
	// Whenever we render to this bitmap, it is directly rendered to the 
	// swap chain associated with the window.
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		dpiX,
		dpiY);

	// Direct2D needs the dxgi version of the backbuffer surface pointer.
	IDXGISurface* dxgiBackBuffer;
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

	if (FAILED(hr))
		return hr;

	// Get a D2D surface from the DXGI back buffer to use as the D2D render target.
	hr = d2dContext->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer,
		&bitmapProperties,
		&d2dTargetBitmap);

	dxgiBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	// set the Direct2D render target.
	d2dContext->SetTarget(d2dTargetBitmap);

	return hr;
}


ID2D1DeviceContext* GraphicEngine::get_device_context() const noexcept
{
	return d2dContext;
}


HRESULT GraphicEngine::refresh(const vector<unique_ptr<Graphic>>& graphics) const noexcept
{
	if (!d2dContext)
		return E_FAIL;

	d2dContext->BeginDraw();
	d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
	const auto t = get_world_transform();
	//const auto a = get_visible_area();	

	if (isGridVisible)
		draw_grid(D2D1::ColorF(D2D1::ColorF::Black), 30.f, 0.5f);

	for (const auto& g : graphics)
		g->draw(d2dContext, t);

	auto hr = d2dContext->EndDraw();

	if (SUCCEEDED(hr))
		hr = swapChain->Present(0, 0);

	return hr;
}


void GraphicEngine::reset() noexcept
{
	zoom = 1.f;
	offset.x = offset.y = 0;
}


HRESULT GraphicEngine::draw_grid(const D2D1_COLOR_F& color, float space, float thickness) const noexcept
{
	CComPtr<ID2D1SolidColorBrush> stroke;
	const auto hr = d2dContext->CreateSolidColorBrush(color, &stroke);

	if (SUCCEEDED(hr))
	{
		d2dContext->SetTransform(get_world_transform());
		const auto clientRect = d2dContext->GetSize();

		assert(zoom > 0);
		const auto linethick = thickness / zoom;

		// draw fixed size grid
		for (float i = -0xfff; i < 0xfff; i += space)
		{
			// draw vertical lines
			d2dContext->DrawLine(D2D1::Point2F(i, -0xfff), D2D1::Point2F(i, 0xfff), stroke, linethick);
			// draw horizzontal lines
			d2dContext->DrawLine(D2D1::Point2F(-0xfff, i), D2D1::Point2F(0xfff, i), stroke, linethick);
		}	
	}

	return hr;
}


void GraphicEngine::set_offset(float x, float y) noexcept
{
	offset.x = x;
	offset.y = y;
}


D2D1_POINT_2F GraphicEngine::get_offset() const noexcept
{
	return offset;
}


void GraphicEngine::set_zoom(float zoom) noexcept
{
	if (!d2dContext)
		return;

	const auto clientRect = d2dContext->GetSize();
	set_zoom(zoom, D2D1::Point2F(clientRect.width / 2.f, clientRect.height / 2.f));
}


void GraphicEngine::set_zoom(float z, const D2D1_POINT_2F& center) noexcept
{
	if (z <= 0)
		return;

	// compute the center position according to the current DPI
	assert(dpiX > 0);
	assert(dpiY > 0);
	const auto c = D2D1::Point2F(center.x * dpiX / 96.f, center.y * dpiY / 96.f);

	// change the offset to keep the focus to the center point
	assert(zoom > 0);
	offset.x += (c.x / z) - (c.x / zoom);
	offset.y += (c.y / z) - (c.y / zoom);
	zoom = z;
}


float GraphicEngine::get_zoom() const noexcept
{
	return zoom;
}


D2D1_MATRIX_3X2_F GraphicEngine::get_world_transform() const noexcept
{
	assert(zoom > 0);
	return D2D1::Matrix3x2F::Translation(offset.x, offset.y) * D2D1::Matrix3x2F::Scale(zoom, zoom);
}


bool GraphicEngine::is_grid_visible() const noexcept
{
	return isGridVisible;
}


void GraphicEngine::hide_grid() noexcept
{
	isGridVisible = false;
}


void GraphicEngine::show_grid() noexcept
{
	isGridVisible = true;
}


D2D1_RECT_F GraphicEngine::get_visible_area() const noexcept
{
	assert(d2dContext);
	const auto size = d2dContext->GetSize();
	D2D1_RECT_F area;
	area.left = -offset.x;
	area.top = -offset.y;
	assert(zoom > 0);
	area.right = (size.width / zoom) - offset.x;
	area.bottom = (size.height / zoom) - offset.y;
	assert(area.right > area.left);
	assert(area.bottom > area.top);

	return area;
}