/*
 * PROJECT:         ReactOS
 * LICENSE:         GNU LGPL by FSF v2.1 or any later
 * FILE:            dll/win32/winent.drv/ogldrv.c
 * PURPOSE:         OpenGL driver for ReactOS/Windows
 * PROGRAMMERS:     Kamil Hornicek
 */

/* INCLUDES ***************************************************************/

#include "winent.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(rosogldrv);

typedef INT  (WINAPI *CHOOSEPIXELFMT) (HDC, CONST PIXELFORMATDESCRIPTOR *);
typedef INT  (WINAPI *DESCRIBEPIXELFMT) (HDC, INT, UINT, PIXELFORMATDESCRIPTOR *);
typedef INT  (WINAPI *GETPIXELFMT) (HDC);
typedef BOOL (WINAPI *SETPIXELFMT) (HDC, INT, CONST PIXELFORMATDESCRIPTOR *);
typedef BOOL (WINAPI *SWAPBUFFERS) (HDC);

static CHOOSEPIXELFMT   glChoosePixelFormat   = NULL;
static DESCRIBEPIXELFMT glDescribePixelFormat = NULL;
static GETPIXELFMT      glGetPixelFormat      = NULL;
static SETPIXELFMT      glSetPixelFormat      = NULL;
static SWAPBUFFERS      glSwapBuffers         = NULL;

/* FUNCTIONS **************************************************************/

BOOL InitOGL(VOID)
{
    HMODULE hOGL;

    hOGL = LoadLibraryW(L"OPENGL32.DLL");

    if (!hOGL)
        return FALSE;

    glChoosePixelFormat = GetProcAddress(hOGL, "wglChoosePixelFormat");
    glDescribePixelFormat = GetProcAddress(hOGL, "wglDescribePixelFormat");
    glGetPixelFormat = GetProcAddress(hOGL, "wglGetPixelFormat");
    glSetPixelFormat = GetProcAddress(hOGL, "wglSetPixelFormat");
    glSwapBuffers = GetProcAddress(hOGL, "wglSwapBuffers");

    if (!glChoosePixelFormat || !glDescribePixelFormat || !glGetPixelFormat ||
        !glSetPixelFormat || !glSwapBuffers)
    {
        FreeLibrary(hOGL);
        ERR("Failed to load required wgl* functions from opengl32\n");
        return FALSE;
    }

    return TRUE;
}

INT CDECL RosDrv_ChoosePixelFormat(PDC_ATTR pdcattr,
                                   CONST PIXELFORMATDESCRIPTOR *ppfd)
{
    if (!glChoosePixelFormat)
        if (!InitOGL())
            return 0;

    return glChoosePixelFormat(pdcattr->hdc, ppfd);
}

INT CDECL RosDrv_GetPixelFormat(PDC_ATTR pdcattr)
{
    if (!glGetPixelFormat)
        if (!InitOGL())
            return 0;

    return glGetPixelFormat(pdcattr->hdc);
}

INT CDECL RosDrv_DescribePixelFormat(PDC_ATTR pdcattr,
                                     INT iPixelFormat,
                                     UINT nBytes,
                                     PIXELFORMATDESCRIPTOR *ppfd)
{
    if (!glDescribePixelFormat)
        if (!InitOGL())
            return 0;

    return glDescribePixelFormat(pdcattr->hdc, iPixelFormat, nBytes, ppfd);
}

BOOL CDECL RosDrv_SetPixelFormat(PDC_ATTR pdcattr,
                                 INT iPixelFormat,
                                 CONST PIXELFORMATDESCRIPTOR *ppfd)
{
    if (!glSetPixelFormat)
        if (!InitOGL())
            return 0;

    return glSetPixelFormat(pdcattr->hdc, iPixelFormat, ppfd);
}


BOOL CDECL RosDrv_SwapBuffers(PDC_ATTR pdcattr)
{
    if (!glSwapBuffers)
        if (!InitOGL())
            return 0;

    return glSwapBuffers(pdcattr->hdc);
}

/* EOF */