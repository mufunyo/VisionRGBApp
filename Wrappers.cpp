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
	HRESULT hr;
	D3DLOCKED_RECT lockedRect;
	hrAssert(pData->LockRect(&lockedRect, NULL, D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK), ASSERT_NOTICE);
	pBuffer = lockedRect.pBits;
	bmpInfo.bmiHeader.biSizeImage = lockedRect.Pitch * height;
}

void D3D9Surface::unmap()
{
	HRESULT hr;
	hrAssert(hr = pData->UnlockRect(), ASSERT_NOTICE);
}