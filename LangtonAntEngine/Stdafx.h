// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once


// Exclude rarely-used items from Windows headers.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Windows Header Files:
#include <Windows.h>
#include <Windowsx.h>
#include <wincodec.h>
#include <atlbase.h>

// DirectX Header Files:
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <d2d1helper.h>
#include <d2d1_2helper.h>
#include <dwrite.h>
#include <d3d11_1.h>

// STD Header Files:
#include <cassert>
#include <memory>

// Libraries:
#pragma comment (lib, "User32.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "Dwrite.lib")
#pragma comment (lib, "Windowscodecs.lib")
#pragma comment (lib, "Ole32.lib")
#pragma comment (lib, "Shlwapi.lib")
#pragma comment (lib, "d3d11.lib")