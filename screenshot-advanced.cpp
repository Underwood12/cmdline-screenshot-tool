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
 *
 *  Requires GDI+. All linker dependencies are specified explicitly in this
 *  file, so you can compile screenshot-advanced.exe by simply running:
 *    cl screenshot-advanced.cpp
 *
 *  To compile with g++/MinGW32 run:
 *    g++ -Wall -O3 screenshot-advanced.cpp -s -static -o screenshot-advanced.exe -municode -lgdi32 -lgdiplus
 */

#ifndef __GNUC__
#  pragma comment(lib, "user32.lib")
#  pragma comment(lib, "gdi32.lib")
#  pragma comment(lib, "gdiplus.lib")
#endif

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include "screenshot-win32.hpp"
#include "screenshot-win32.cpp"

Gdiplus::Bitmap *ResizeBitmap(Gdiplus::Bitmap *source, int percentage);

int wmain(int argc, wchar_t **argv)
{
  bool defaultfn = true;
  const wchar_t *filename = L"screenshot.png";
  wchar_t format[5] = L"png";
  wchar_t encoder[16] = L"image/png";
  long quality = -1;
  int resize = -1;
  int wait = -1;

  for (int i=1; i < argc; i++) {
    if (wcscmp(argv[i], L"-help") == 0 || wcscmp(argv[i], L"--help") == 0 ||
        wcscmp(argv[i], L"-h") == 0 || wcscmp(argv[i], L"-?") == 0) {
      std::wcout <<
        "Usage:\n\n"
        "  -h, -help                 display this help and exit\n"
        "  -f, -filename \"out.png\"   file name (default: screenshot.png)\n"
        "  -e, -encoder png          file encoder: bmp/jpeg/gif/tiff/png (default: png)\n"
        "  -q, -quality 100          file quality for jpeg (between 0 and 100)\n"
        "  -r, -resize 50            image size, % of the original size (between 1 and 99)\n"
        "  -w, -wait 3               seconds to wait before taking a screenshot (bewteen 1 and 30)\n\n"
        "Copyright (c) 2009, The Mozilla Foundation\n";
      return 0;
    }
  }

  for (int i=1; i < argc-1; i++) {
    if (wcscmp(argv[i], L"-filename") == 0 || wcscmp(argv[i], L"-f") == 0) {
      defaultfn = false;
      filename = argv[++i];
    }
    else if (wcscmp(argv[i], L"-encoder") == 0 || wcscmp(argv[i], L"-e") == 0) {
      wmemcpy(format, argv[++i], 5);
    }
    else if (wcscmp(argv[i], L"-quality") == 0 || wcscmp(argv[i], L"-q") == 0) {
      quality = _wtol(argv[++i]);
    }
    else if (wcscmp(argv[i], L"-resize") == 0 || wcscmp(argv[i], L"-r") == 0) {
      resize = _wtoi(argv[++i]);
    }
    else if (wcscmp(argv[i], L"-wait") == 0 || wcscmp(argv[i], L"-w") == 0) {
      wait = _wtoi(argv[++i]);
    }
  }

  if (wcscmp(format, L"jpg") == 0) {
    wmemcpy(format, L"jpeg", 5);
  }
  else if (wcscmp(format, L"tif") == 0) {
    wmemcpy(format, L"tiff", 5);
  }

  if (wcscmp(format, L"bmp") != 0 && wcscmp(format, L"jpeg") != 0 &&
      wcscmp(format, L"gif") != 0 && wcscmp(format, L"tiff") != 0 && 
      wcscmp(format, L"png") != 0) {
    std::wcout << "warning: unkown file encoder! png will be used instead.\n";
    wmemcpy(format, L"png", 5);
  }

  if (defaultfn == true) {
    if (wcscmp(format, L"bmp") == 0) {
      filename = L"screenshot.bmp";
    }
    else if (wcscmp(format, L"jpeg") == 0) {
      filename = L"screenshot.jpg";
    }
    else if (wcscmp(format, L"gif") == 0) {
      filename = L"screenshot.gif";
    }
    else if (wcscmp(format, L"tiff") == 0) {
      filename = L"screenshot.tif";
    }
  }

  wmemcpy(encoder+wcslen(L"image/"), format, 16-wcslen(L"image/"));

  screenshotInitGdiplus();

  if (wait > 0 && wait < 30) {
    std::wcout << "taking screenshot in " << wait << " seconds ...\n";
    Sleep(wait*1000);
  }

  Gdiplus::Bitmap *b = getScreenshot();

  if (resize > 0 && resize < 100 && b) {
    Gdiplus::Bitmap *temp = ResizeBitmap(b, resize);
    delete b;
    b = temp;
  }

  int ret = screenshotSaveBitmap(b, filename, encoder, quality);

  screenshotFreeGdiplus();
  return ret;
}

Gdiplus::Bitmap *ResizeBitmap(Gdiplus::Bitmap *source, int percentage)
{
  int w = source->GetWidth() * percentage / 100;
  if (w < 1) { w = 1; }

  int h = source->GetHeight() * percentage / 100;
  if (h < 1) { h = 1; }

  Gdiplus::Bitmap *b = new Gdiplus::Bitmap(w, h, source->GetPixelFormat());
  if (b) {
    Gdiplus::Graphics *g = Gdiplus::Graphics::FromImage(b);
    if (g) {
      g->DrawImage(source, 0, 0, w, h);
      delete g;
    } else {
      delete b;
      b = NULL;
    }
  }

  return b;
}
