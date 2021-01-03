#include "stdafx.h"
#include "Wrappers.hpp"
#include "Error.hpp"
#include "d3dx9.h"

Mutex::Mutex() {
	InitializeCriticalSection(&data);
}

void Mutex::lock() {
	EnterCriticalSection(&data);
}

void Mutex::unlock() {
	LeaveCriticalSection(&data);
}

D3D9Context::D3D9Context() {}

D3D9Context::D3D9Context(HWND hWnd) : hWnd(hWnd) {
	HRESULT hr = E_UNEXPECTED;

	if FAILED(hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &pD3D9))
		warning(TEXT("D3D9Context: Direct3DCreate9Ex failed, result = 0x%08lX"), hr);

	D3DPRESENT_PARAMETERS params;
	ZeroMemory(&params, sizeof(D3DPRESENT_PARAMETERS));
	params.Windowed = TRUE;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = hWnd;
	params.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	if FAILED(hr = pD3D9->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &params, NULL, &pD3D9Device))
		warning(TEXT("D3D9Context: CreateDeviceEx failed, result = 0x%08lX"), hr);

	D3DSURFACE_DESC desc;
	if FAILED(hr = pD3D9Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuf.pData))
		warning(TEXT("D3D9Context: GetBackBuffer failed, result = 0x%08lX"), hr);
	if FAILED(hr = backBuf.pData->GetDesc(&desc))
		warning(TEXT("D3D9Context: GetDesc failed, result = 0x%08lX"), hr);
	backBuf.width = desc.Width;
	backBuf.height = desc.Height;
}
D3D9Context::~D3D9Context() {

}

D3D9Surface* D3D9Context::createSurface(unsigned int width, unsigned int height, PixelFmt format)
{
	HRESULT hr;
	D3D9Surface* pSurface = new D3D9Surface(width, height, format);

	if FAILED(hr = pD3D9Device->CreateOffscreenPlainSurface(width, height, pixFmtD3D[format], D3DPOOL_DEFAULT, &pSurface->pData, NULL))
		warning(TEXT("createSurface: CreateOffscreenPlainSurface failed, result = 0x%08lX"), hr);

	return pSurface;
}

D3D9Surface* D3D9Context::createSurfaceFromFile(LPCTSTR lpFile)
{
	HRESULT hr;
	D3DSURFACE_DESC desc;
	D3D9Surface* pSurface = new D3D9Surface();

	if FAILED(hr = D3DXCreateTextureFromFileEx(pD3D9Device, lpFile, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0, NULL, NULL, &pSurface->pParentTexture))
		warning(TEXT("createSurfaceFromFile: D3DXCreateTextureFromFile failed, result = 0x%08lX"), hr);

	if FAILED(hr = pSurface->pParentTexture->GetSurfaceLevel(0, &pSurface->pData))
		warning(TEXT("createSurfaceFromFile: GetSurfaceLevel failed, result = 0x%08lX"), hr);

	if FAILED(hr = pSurface->pData->GetDesc(&desc))
		warning(TEXT("createSurfaceFromFile: GetDesc failed, result = 0x%08lX"), hr);
	pSurface->width = desc.Width;
	pSurface->height = desc.Height;

	return pSurface;
}
void D3D9Context::present()
{
	pD3D9Device->Present(NULL, NULL, NULL, NULL);
}

bool D3D9Context::blit(D3D9Surface* pSrc, D3D9Surface* pDest) {
	return hrAssert(pD3D9Device->StretchRect(pSrc->pData, nullptr, pDest->pData, nullptr, D3DTEXF_LINEAR), ASSERT_WARNING);
}

D3D9Surface::D3D9Surface(unsigned int width, unsigned int height, PixelFmt format)
	: width(width), height(height), pixFormat(pixFormat), pBuffer(NULL), pParentTexture(NULL), pData(NULL) {
	HDC hDC = GetDC(NULL);

	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = height;
	bmpInfo.bmiHeader.biBitCount = pixFmtBpp[format];
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biCompression = pixFmtFCC[format];
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 3000;
	bmpInfo.bmiHeader.biYPelsPerMeter = 3000;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;
	bmpInfo.bmiHeader.biSizeImage = width * height * pixFmtBpp[format] / 8;

	memcpy(&bmpInfo.bmiColors, &pixFmtMask[pixFormat], sizeof(pixFmtMask[format]));
	hBitmap = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, &pBuffer, NULL, 0);
}

D3D9Surface::D3D9Surface()
	: pParentTexture(nullptr), pData(nullptr), hBitmap(nullptr) {

}

D3D9Surface::~D3D9Surface() {
	if (pData)
		pData->Release();
	if (pParentTexture)
		pParentTexture->Release();
	if (hBitmap)
		DeleteObject(hBitmap);
}

void D3D9Surface::map()
{
	D3DLOCKED_RECT lockedRect;
	hrAssert(pData->LockRect(&lockedRect, NULL, D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK), ASSERT_NOTICE);
	pBuffer = lockedRect.pBits;
	bmpInfo.bmiHeader.biSizeImage = lockedRect.Pitch * height;
}

void D3D9Surface::unmap()
{
	hrAssert(pData->UnlockRect(), ASSERT_NOTICE);
}