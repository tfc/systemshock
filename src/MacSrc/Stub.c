#include <lg_types.h>
#include <Carbon/Carbon.h>

// Stub out all of the functions that we haven't implemented yet!

Boolean gTuneDone;

void StopMovie(void * x) { (void)x; }
void StartMovie(void * m) { (void)m; }
Boolean IsMovieDone(void * x) { (void)x; return true; }

void HideCursor(void) { }
void ShowCursor(void) { }
ushort GetOSEvent(short eventMask, EventRecord *theEvent)
{
    (void)eventMask;
    (void)theEvent;

    return false;
}

void AdvanceProgress(void) { }
void EndProgressDlg(void) { }

//Boolean ShockAlertFilterProc(DialogPtr dlog, EventRecord *evt, short *itemHit) { return false; }

void MoviesTask(void *m, int n) { (void)m; (void)n; }

void DisposHandle(void *h) { (void)h; }
void DisposeMovie(void *m) { (void)m; }
void DisposCTable(void *c) { (void)c; }

void BlitLargeAlign(uchar *draw_buffer, int dstRowBytes, void *dstPtr, long w, long h, long modulus)
{
    (void)draw_buffer;
    (void)dstRowBytes;
    (void)dstPtr;
    (void)w;
    (void)h;
    (void)modulus;
}

void BlitLargeAlignSkip(uchar *draw_buffer, int dstRowBytes, void *dstPtr, long w, long h, long modulus)
{
    (void)draw_buffer;
    (void)dstRowBytes;
    (void)dstPtr;
    (void)w;
    (void)h;
    (void)modulus;
}
