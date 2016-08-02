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

#ifndef SCREENSHOT_WIN32_HPP
#define SCREENSHOT_WIN32_HPP

#include <windows.h>
#include <gdiplus.h>

/* initialize Gdiplus */
void screenshotInitGdiplus(void);

/* Take a screenshot of the desktop and save it.
 *
 * filename: save screenshot under this filename
 *
 * format: the image file format used to save the screenshot
 *  Available formats are:
 *   L"image/bmp"
 *   L"image/gif"
 *   L"image/jpeg"
 *   L"image/png"
 *   L"image/tiff"
 *  Default: L"image/bmp"
 *
 * quality: file quality for jpeg (between 0 and 100) */
int takeScreenshot(const wchar_t *filename, const wchar_t *format = L"image/bmp", long quality = -1);

/* Take a screenshot of the desktop and return a bitmap. */
Gdiplus::Bitmap *getScreenshot(void);

/* Save a bitmap to a file. See takeScreenshot() for details. */
int screenshotSaveBitmap(Gdiplus::Bitmap *b, const wchar_t *filename, const wchar_t *format = L"image/bmp", long quality = -1);

/* Retrieve the Class Identifier for an encoder. */
int getEncoderClsid(const wchar_t *format, CLSID *pClsid);

/* shutdown Gdiplus */
void screenshotFreeGdiplus(void);

#endif  /* !SCREENSHOT_WIN32_HPP */
