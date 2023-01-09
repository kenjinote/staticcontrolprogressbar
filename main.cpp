#define UNICODE
#pragma comment(linker,"/opt:nowin98")
#include<windows.h>

TCHAR szClassName[]=TEXT("Static Control");

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HWND hStatic;
	static int nPos=0;
	switch(msg)
	{
	case WM_CREATE:
		hStatic=CreateWindow(TEXT("Static"),0,WS_CHILD|WS_VISIBLE|WS_BORDER|SS_OWNERDRAW,8,8,256,32,hWnd,(HMENU)100,((LPCREATESTRUCT)(lParam))->hInstance,0);
		SetTimer(hWnd,0x1234,1,0);
		break;
	case WM_TIMER:
		nPos++;
		InvalidateRect(hStatic,NULL,FALSE);
		if(nPos==1000)KillTimer(hWnd,0x1234);
		return 0;
	case WM_DRAWITEM:
		if(wParam==100)
		{
			RECT rcBar=((LPDRAWITEMSTRUCT)lParam)->rcItem;
			int nLower=0,nUpper=1000;
			rcBar.right=((rcBar.right-rcBar.left)*(nPos-nLower))/(nUpper-nLower);
			FillRect(((LPDRAWITEMSTRUCT)lParam)->hDC,&rcBar,(HBRUSH) (COLOR_HIGHLIGHT+1));
			TCHAR szTemp[20];
			RECT rcIn=rcBar;
			RECT rcOut=((LPDRAWITEMSTRUCT)lParam)->rcItem;
			rcOut.left=rcBar.right;
			HRGN rgnIn=CreateRectRgnIndirect(&rcIn);
			HRGN rgnOut=CreateRectRgnIndirect(&rcOut);
			int len=wsprintf(szTemp,TEXT("%d%%"),nPos/10);
			int dx=((LPDRAWITEMSTRUCT)lParam)->rcItem.right;
			int dy=((LPDRAWITEMSTRUCT)lParam)->rcItem.bottom;
			SIZE Size;
			GetTextExtentPoint32(((LPDRAWITEMSTRUCT)lParam)->hDC,szTemp,len,&Size);
			SetBkMode(((LPDRAWITEMSTRUCT)lParam)->hDC,TRANSPARENT);
			SetTextColor(((LPDRAWITEMSTRUCT)lParam)->hDC,RGB(255,255,255));
			SelectClipRgn(((LPDRAWITEMSTRUCT)lParam)->hDC,rgnIn);
			ExtTextOut(((LPDRAWITEMSTRUCT)lParam)->hDC,(dx-Size.cx)/2,(dy-Size.cy)/2,ETO_CLIPPED,&rcIn,szTemp,len,NULL);
			SetTextColor(((LPDRAWITEMSTRUCT)lParam)->hDC,RGB(0,0,0));
			SelectClipRgn(((LPDRAWITEMSTRUCT)lParam)->hDC,rgnOut);
			ExtTextOut(((LPDRAWITEMSTRUCT)lParam)->hDC,(dx-Size.cx)/2,(dy-Size.cy)/2,ETO_OPAQUE|ETO_CLIPPED,&rcOut,szTemp,len,NULL);
			SelectClipRgn(((LPDRAWITEMSTRUCT)lParam)->hDC,NULL);
			DeleteObject(rgnIn);
			DeleteObject(rgnOut);
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd,0x1234);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	return 0;
}

EXTERN_C void __cdecl WinMainCRTStartup()
{
	MSG msg;
	HINSTANCE hInstance=GetModuleHandle(0);
	WNDCLASS wndclass={CS_HREDRAW|CS_VREDRAW,WndProc,0,0,hInstance,0,LoadCursor(0,IDC_ARROW),(HBRUSH)(COLOR_WINDOW+1),0,szClassName};
	RegisterClass(&wndclass);
	HWND hWnd=CreateWindow(szClassName,TEXT("Static Control"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,0,CW_USEDEFAULT,0,0,0,hInstance,0);
	ShowWindow(hWnd,SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(0);
}

#if _DEBUG
void main(){}
#endif