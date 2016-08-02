/*
 * Copyright (c) 2009, The Mozilla Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Mozilla Foundation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY The Mozilla Foundation ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL The Mozilla Foundation BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Contributors:
 *   Ted Mielczarek <ted.mielczarek@gmail.com>
 *   RaMMicHaeL <http://rammichael.com>
 *   djcj <djcj@gmx.de>
 *
 * Original sources:
 *  https://github.com/RaMMicHaeL/cmdline-screenshot-maker
 *  https://blog.mozilla.org/ted/2009/02/05/command-line-screenshot-tool-for-windows/
 */

#ifndef SCREENSHOT_WIN32_CPP
#define SCREENSHOT_WIN32_CPP

#include <windows.h>
#include <gdiplus.h>
#if defined(__MSYS__) || defined(__CYGWIN__)
#  include <wchar.h>
#endif
#include "screenshot-win32.hpp"

ULONG_PTR screenshotGdiplusToken;

void screenshotInitGdiplus()
{
  Gdiplus::GdiplusStartupInput screenshotGdiplusStartupInput;
  GdiplusStartup(&screenshotGdiplusToken, &screenshotGdiplusStartupInput, NULL);
}

int takeScreenshot(const wchar_t *filename, const wchar_t *format, long quality)
{
  if (filename == NULL) {
    return -1;  /* Failure */
  }
  screenshotInitGdiplus();
  Gdiplus::Bitmap *b = getScreenshot();
  int ret = screenshotSaveBitmap(b, filename, format, quality);
  screenshotFreeGdiplus();
  return ret;
}

Gdiplus::Bitmap *getScreenshot()
{
  HWND    desktop   = GetDesktopWindow();
  HDC     desktopdc = GetDC(desktop);
  HDC     compatdc  = CreateCompatibleDC(desktopdc);
  int     width     = GetSystemMetrics(SM_CXSCREEN);
  int     height    = GetSystemMetrics(SM_CYSCREEN);
  HBITMAP newbmp    = CreateCompatibleBitmap(desktopdc, width, height);
  HBITMAP oldbmp    = (HBITMAP)SelectObject(compatdc, newbmp);
  BitBlt(compatdc, 0, 0, width, height, desktopdc, 0, 0, SRCCOPY|CAPTUREBLT);
  SelectObject(compatdc, oldbmp);

  Gdiplus::Bitmap *b = Gdiplus::Bitmap::FromHBITMAP(newbmp, NULL);

  ReleaseDC(desktop, desktopdc);
  DeleteObject(newbmp);
  DeleteDC(compatdc);

  return b;
}

int screenshotSaveBitmap(Gdiplus::Bitmap *b, const wchar_t *filename, const wchar_t *format, long quality)
{
  if (filename == NULL) {
    return -1;  /* Failure */
  }

  CLSID encoderClsid;
  Gdiplus::EncoderParameters encoderParameters;
  Gdiplus::Status stat = Gdiplus::GenericError;

  if (b) {
    if (getEncoderClsid(format, &encoderClsid) != -1) {
      if (quality >= 0 && quality <= 100 && wcscmp(format, L"image/jpeg") == 0) {
        encoderParameters.Count = 1;
        encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
        encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
        encoderParameters.Parameter[0].NumberOfValues = 1;
        encoderParameters.Parameter[0].Value = &quality;
        stat = b->Save(filename, &encoderClsid, &encoderParameters);
      } else {
        stat = b->Save(filename, &encoderClsid, NULL);
      }
    }
    delete b;
  }

  return (stat == Gdiplus::Ok) ? 0 : 1;
}

/* From http://msdn.microsoft.com/en-us/library/ms533843%28VS.85%29.aspx
 */
int getEncoderClsid(const wchar_t *format, CLSID *pClsid)
{
  UINT num = 0;   /* number of image encoders */
  UINT size = 0;  /* size of the image encoder array in bytes */

  Gdiplus::ImageCodecInfo *pImageCodecInfo = NULL;

  Gdiplus::GetImageEncodersSize(&num, &size);
  if (size == 0) {
    return -1;  /* Failure */
  }

  pImageCodecInfo = (Gdiplus::ImageCodecInfo *)(malloc(size));
  if (pImageCodecInfo == NULL) {
    return -1;  /* Failure */
  }

  GetImageEncoders(num, size, pImageCodecInfo);

  for (UINT j = 0; j < num; ++j) {
    if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0 ) {
      *pClsid = pImageCodecInfo[j].Clsid;
      free(pImageCodecInfo);
      return j;  /* Success */
    }
  }

  free(pImageCodecInfo);
  return -1;  /* Failure */
}

void screenshotFreeGdiplus()
{
  Gdiplus::GdiplusShutdown(screenshotGdiplusToken);
}

#endif  /* !SCREENSHOT_WIN32_CPP */
