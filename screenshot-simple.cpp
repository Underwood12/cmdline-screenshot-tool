/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 * Contributors:
 *   2016 djcj <djcj@gmx.de>
 */

/*
 * screenshot-simple.cpp: Save a screenshot of the Windows desktop in .bmp format.
 *  If a filename is specified as the first argument on the commandline,
 *  then the image will be saved to that filename. Otherwise, the image will
 *  be saved as "screenshot.bmp" in the current working directory.
 *
 *  Requires GDI+. All linker dependencies are specified explicitly in this
 *  file, so you can compile screenshot-simple.exe by simply running:
 *    cl screenshot-simple.cpp
 *
 *  To compile with g++/MinGW32 run:
 *    g++ -Wall -O3 screenshot-simple.cpp -s -static -o screenshot-simple.exe -municode -lgdi32 -lgdiplus
 *
 *  For Cygwin or Msys use:
 *    g++ -Wall -O3 screenshot-simple.cpp -s -o screenshot-simple.exe -lgdi32 -lgdiplus
 */

#ifndef __GNUC__
#  pragma comment(lib, "user32.lib")
#  pragma comment(lib, "gdi32.lib")
#  pragma comment(lib, "gdiplus.lib")
#endif

#include <windows.h>
#include <gdiplus.h>
#include "screenshot-win32.hpp"
#include "screenshot-win32.cpp"

#undef _tmain
#undef _TCHAR
#if defined(__MSYS__) || defined(__CYGWIN__)
#  include <locale.h>
#  include <wchar.h>
#  include <stdlib.h>
#  define _tmain main
#  define _TCHAR char
#else
#  define _tmain wmain
#  define _TCHAR wchar_t
#endif

int _tmain(int argc, _TCHAR **argv)
{
#if defined(__MSYS__) || defined(__CYGWIN__)
  setlocale(LC_ALL, "en_US.utf8");
#endif

  const wchar_t *filename = L"screenshot.bmp";

  if (argc > 1) {
#if defined(__MSYS__) || defined(__CYGWIN__)
    const char *orig = argv[1];
    mbstate_t state;
    memset(&state, 0, sizeof state);
    int len = mbsrtowcs(NULL, &orig, 0, &state) + 1;
    wchar_t wstr[len];
    mbsrtowcs(&wstr[0], &orig, len, &state);
    filename = wstr;
#else
    filename = argv[1];
#endif
  }

  return takeScreenshot(filename);
}
