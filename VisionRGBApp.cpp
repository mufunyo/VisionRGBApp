//////////////////////////////////////////////
// VisionRGBApp.cpp

#include "stdafx.h"
#include "VisionRGBApp.hpp"
#include "Error.hpp"

// Global Variables
static HRGBDLL gHRGBDLL = NULL;
HRGB ghRGB = NULL;

void VisionRGBApp::recvSample(PRGBFRAMEDATA pFrameData) {
	mutex.lock();
	if (bCapturing) {
		for (unsigned long i = 0; i < buffers; i++) { // find which buffer we have been passed
			if (pSurfaces[i]->pBuffer == pFrameData->PBitmapBits) {
				CapturedFrame frame;
				pSurfaces[i]->unmap();

				frame.pSurface = pSurfaces[i];
				frame.newWidth = pFrameData->PBitmapInfo->biWidth;
				frame.newHeight = pFrameData->PBitmapInfo->biHeight;

				qFrames.push(frame);
				break;
			}
		}
	}
	mutex.unlock();
}

void VisionRGBApp::drawSample() {
	mutex.lock();
	if (qFrames.size() > NUM_BUFFERS - 1) { // wait for the queue to fill
		for (unsigned long i = 0; i < buffers; i++) { // find which surface index is at the front of the queue
			if (pSurfaces[i] == qFrames.front().pSurface) {
				CapturedFrame* frame = &qFrames.front();
				m_d3D9.blit(pSurfaces[i], &m_d3D9.backBuf);

				if ((pSurfaces[i]->width != frame->newWidth) || pSurfaces[i]->height != frame->newHeight) {
					delete pSurfaces[i];
					pSurfaces[i] = m_d3D9.createSurface(frame->newWidth, frame->newHeight, format);
				}
				pSurfaces[i]->map();

				RGBChainOutputBuffer(ghRGB, &pSurfaces[i]->bmpInfo, pSurfaces[i]->pBuffer);
				break;
			}
		}
		qFrames.pop();
		m_d3D9.present();
	}
	mutex.unlock();
}

bool VisionRGBApp::startCapture(int width, int height, PixelFmt pixFormat) {
	// allocate textures
	buffers = 0;
	for (int i = 0; i < NUM_BUFFERS; i++) {
		pSurfaces[i] = m_d3D9.createSurface(width, height, pixFormat);
		pSurfaces[i]->map();

		if (pSurfaces[i]->pBuffer)
			buffers++;
		else
			warning(TEXT("startCapture: Could not create surface"));
	}
	if (RGBERROR_NO_ERROR == RGBOpenInput(0, &ghRGB)) {
		if (RGBERROR_NO_ERROR == RGBSetFrameDropping(ghRGB, 0)) {
			PRGBFRAMECAPTUREDFNEX callback = [](HWND hWnd, HRGB hRGB, PRGBFRAMEDATA pFrameData, ULONG_PTR userData) -> void { reinterpret_cast<VisionRGBApp*>(userData)->recvSample(pFrameData); };
			if (RGBERROR_NO_ERROR == RGBSetFrameCapturedFnEx(ghRGB, callback, (ULONG_PTR)this)) {
				if (buffers) {
					for (unsigned long i = 0; i < buffers; i++) {
						if (RGBERROR_NO_ERROR != RGBChainOutputBuffer(ghRGB, &pSurfaces[i]->bmpInfo, pSurfaces[i]->pBuffer))
							// note to Datapath: RGBChainOutputBuffer could do with more error checks.
							// even a slightly malformed bmpInfo will cause the callback to never be called, with no errors or warnings generated.
							break;
					}
					if (RGBERROR_NO_ERROR == RGBUseOutputBuffers(ghRGB, TRUE))
						bCapturing = true;
				}
				if (RGBERROR_NO_ERROR == RGBStartCapture(ghRGB))
					return true;
			}
		}
	}

	warning(TEXT("Could not open Vision input %i"), 1);
	return false;
}

void VisionRGBApp::stopCapture() {
	mutex.lock();
	RGBUseOutputBuffers(ghRGB, FALSE);
	bCapturing = false;
	mutex.unlock();

	RGBStopCapture(ghRGB);
	RGBCloseInput(ghRGB);
}

VisionRGBApp::VisionRGBApp() {
	format = RGB32;
	RGBLoad(&gHRGBDLL);

    //Create the Window
    m_frame.Create();
    m_d3D9 = D3D9Context(m_frame.GetView());

	// yes, it's stupid to put this code here, in the constructor, but putting it anywhere else causes a mysterious race condition.
	// feel free to fix.
	if (!startCapture(1920, 1080, format))
		crash(TEXT("Could not start capturing!"));
}

BOOL VisionRGBApp::InitInstance() {
    return TRUE;
}

int VisionRGBApp::MessageLoop() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    D3D9Surface* testImage = m_d3D9.createSurfaceFromFile(TEXT("..\\..\\VisionRGBApp.png"));
    m_d3D9.blit(testImage, &m_d3D9.backBuf);
	m_d3D9.present();

    while (1) {
        // for some reason, present() is slow (even when not presenting anything at all),
        // so for now let's only present if there are no messages.
        if (!::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			drawSample();
            Sleep(0);
            continue;
        }
        if (msg.message == WM_QUIT)
            break;

        if (!PreTranslateMessage(msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }
    return LOWORD(msg.wParam);
}
