//////////////////////////////////////////////////
//  Wrappers.h
//  Simple wrappers for various things
//  Note: we intentionally don't use the "C" prefix for class names here
//  because we're mostly outside of Win32++ code and we'd potentially
//  be creating name conflicts with existing Win32++ class names.

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d9.h>
enum PixelFmt {
	RGB32,
	RGB24,
	RGB16,
	YUY2,
	NV12,
	Y8
};
// lookup table for bits per pixel of each PixelFmt value
const int pixFmtBpp[] = { 32, 24, 16, 16, 12, 8 };
// lookup table for fourCC code of each PixelFmt value
const DWORD pixFmtFCC[] = { BI_BITFIELDS, BI_BITFIELDS, BI_BITFIELDS, '2YUY', '21VN', 'YERG' };
// lookup table for D3D9 texture format of each PixelFmt value
const D3DFORMAT pixFmtD3D[] = { D3DFMT_X8R8G8B8, D3DFMT_R8G8B8, D3DFMT_R5G6B5, D3DFMT_YUY2, (D3DFORMAT)'21VN', D3DFMT_L8 };
// lookup table for bitfield mask of each PixelFmt value
const struct {
	COLORREF Mask[3];
}  pixFmtMask[] = {
	{ 0x00ff0000, 0x0000ff00, 0x000000ff, },
{ 0x00ff0000, 0x0000ff00, 0x000000ff, },
{ 0x0000f800, 0x000007e0, 0x0000001f, },
{ 0x00000000, 0x00000000, 0x00000000, },
{ 0x00000000, 0x00000000, 0x00000000, },
{ 0x00000000, 0x00000000, 0x00000000, },
};

// simple RAII mutex class
class Mutex {
public:
	Mutex();
	void lock();
	void unlock();
	CRITICAL_SECTION data;
};

// less boilerplatey version of IDirect3DSurface9
class D3D9Surface {
public:
	D3D9Surface(unsigned int width, unsigned int height, PixelFmt format);
	D3D9Surface();
	~D3D9Surface();
	void map();
	void unmap();
	UINT width, height;
	PixelFmt pixFormat;
	void* pBuffer;
	HBITMAP hBitmap;
	BITMAPINFO bmpInfo;
	IDirect3DSurface9* pData;
	IDirect3DTexture9* pParentTexture;
};

class D3D9Context {
public:
	D3D9Context();
	D3D9Context(HWND hWnd);
	~D3D9Context();
	D3D9Surface* createSurface(unsigned int width, unsigned int height, PixelFmt format);
	D3D9Surface* createSurfaceFromFile(LPCTSTR lpFile);
	bool blit(D3D9Surface* pSrc, D3D9Surface* pDest, bool clip = false);
	void present();
	HWND hWnd;
	IDirect3D9Ex* pD3D9;
	IDirect3DDevice9Ex* pD3D9Device;
	D3D9Surface backBuf;
};