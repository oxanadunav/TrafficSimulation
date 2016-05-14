#include "header.h"
#include "Car.h"
#include <windows.h>

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
static traffic_lights TL[4];
char szClassName[ ] = "WindowsApp";
static Car *car[50];
bool LoadAndBlitBitmaph(char* szFileName, HDC hWinDC,int x, int y)
{
	// Load the bitmap image file
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
	// Verify that the image was loaded
	if (hBitmap == NULL) {
		::MessageBox(NULL, TEXT("LoadImage Failed"), TEXT("Error"), MB_OK);
		return false;
	}

	// Create a device context that is compatible with the window
	HDC hLocalDC;
	hLocalDC = ::CreateCompatibleDC(hWinDC);
	// Verify that the device context was created
	if (hLocalDC == NULL) {
		::MessageBox(NULL, TEXT("CreateCompatibleDC Failed"), TEXT("Error"), MB_OK);
		return false;
	}

	// Get the bitmap's parameters and verify the get
	BITMAP qBitmap;
	int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP),
		reinterpret_cast<LPVOID>(&qBitmap));
	if (!iReturn) {
		::MessageBox(NULL, TEXT("GetObject Failed"), TEXT("Error"), MB_OK);
		return false;
	}

	// Select the loaded bitmap into the device context
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
	if (hOldBmp == NULL) {
		::MessageBox(NULL, TEXT("SelectObject Failed"), TEXT("Error"), MB_OK);
		return false;
	}

	// Blit the dc which holds the bitmap onto the window's dc
	BOOL qRetBlit = ::BitBlt(hWinDC, x, y, qBitmap.bmWidth, qBitmap.bmHeight,
		hLocalDC, 0, 0, SRCCOPY);
	if (!qRetBlit) {
		::MessageBox(NULL, TEXT("Blit Failed"), TEXT("Error"), MB_OK);
		return false;
	}

	// Unitialize and deallocate resources
	::SelectObject(hLocalDC, hOldBmp);
	::DeleteDC(hLocalDC);
	::DeleteObject(hBitmap);
	return true;
}
int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    wincl.hbrBackground =NULL;
    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,
           szClassName,
           "Laboratory 5",
           WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           1150,
           682,
           HWND_DESKTOP,
           NULL,
           hThisInstance,
           NULL
           );

    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc,hdcMem,hdccMem;
    static PAINTSTRUCT ps;
    static RECT rect;
    static HBRUSH hBrush;
    static HBITMAP hbmMem,hBitmap01;

    static HANDLE oldBitmap;

    static int timerSpeed = 5000;

     RECT   rcClient;
    static BITMAP bitmap;
    static HBITMAP filll = NULL ;
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE);


    static int tspeed=30,nrobjects=0;
    POINT coord;
    double a[4]={365,780,365,780};
    double b[4]={200,200,500,500};
    static bool tl[4]={FALSE,TRUE,TRUE,FALSE};
    switch (message)
    {
        case WM_CREATE:
        {
          hdc = GetDC(hwnd);
          GetClientRect(hwnd,&rect);

          for (int i=0;i<4;i++)
          {
              coord.x=a[i];
              coord.y=b[i];
              TL[i].setTL(coord,tl[i]);
          }
          SetTimer(hwnd, ID_TIMER1, timerSpeed, NULL);
          SetTimer(hwnd,ID_TIMER,tspeed,NULL);
        break;
        }

        case WM_MOUSEWHEEL:
        {
            if((short)HIWORD(wParam)<0)
            {
                timerSpeed+=100;
            }
            else
            {
                timerSpeed-=100;
                if (timerSpeed<0)
                {
                    timerSpeed=1;
                }
            }
            KillTimer(hwnd,ID_TIMER1);
            SetTimer(hwnd,ID_TIMER1,timerSpeed,NULL);
        break;
        }

        case WM_LBUTTONDOWN:
            if (nrobjects<50){
            POINT newobj;
            int cnt=0;
            GetClientRect(hwnd, &rcClient);
            for (int i=0;i<nrobjects;i++){
                    if(car[i]->center.x<150){
                        cnt=1;
                    }
            }
            if (cnt==0){
                newobj.x = 50;
                newobj.y = (rcClient.bottom - rcClient.top)/2+65;

                car[nrobjects]=new Car(newobj,5+newobj.x%5);

                nrobjects++;
            }
            }
            break;

        case WM_PAINT:
        {
             hdc = BeginPaint(hwnd,&ps);

            LoadAndBlitBitmaph(TEXT("map.bmp"), hdc, 0,0);

            hdcMem = CreateCompatibleDC(hdc);
            oldBitmap = SelectObject(hdcMem, hBitmap01);

            GetObject(hBitmap01, sizeof(bitmap), &bitmap);
            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem,oldBitmap);

            DeleteDC(hdcMem);

            EndPaint(hwnd,&ps);

        break;
        }

        case WM_TIMER:
        {
            RECT rectangle;
            hdc=GetDC(hwnd);
            switch (wParam)
            {
            case ID_TIMER1:
                for (int i=0;i<4;i++){
                    TL[i].SwitchTL(hdc,hBrush);
                }
                 return 0;

            case ID_TIMER:
            for(int i = 0; i < nrobjects; i++)
            {
                if((car[i]->right==2) || (car[i]->center.x<550)&& (car[i]->semafor==true)){
                rectangle.bottom=car[i]->center.y+33;
                rectangle.left=car[i]->center.x-25;
                rectangle.top=car[i]->center.y-33;
                rectangle.right=car[i]->center.x-20;
                InvalidateRect(hwnd,&rectangle,true);
                }
                if((car[i]->right==0) && (car[i]->center.x-26>(rect.right-1)/2)&& (car[i]->semafor==true)){
                rectangle.bottom=car[i]->center.y+50;
                rectangle.left=car[i]->center.x-77;
                rectangle.top=car[i]->center.y+35;
                rectangle.right=car[i]->center.x+33;
                InvalidateRect(hwnd,&rectangle,true);
                }
                if((car[i]->right==1) && (car[i]->center.x+56>(rect.right-1)/2)&& (car[i]->semafor==true)){
                rectangle.bottom=car[i]->center.y-15;
                rectangle.left=car[i]->center.x-33;
                rectangle.top=car[i]->center.y-20;
                rectangle.right=car[i]->center.x+33;
                InvalidateRect(hwnd,&rectangle,true);
                }

                car[i]->Move(hdc, rect, hBrush);


                for(int i = 0; i<nrobjects-1; i++)
                {
                    for(int j = i+1; j < nrobjects; j++)
                    {
                        car[i]->Coll(*car[j]);
                    }
                }
                if ((car[i]->center.x<TL[2].position.x-20) && (car[i]->center.x>TL[2].position.x-90) && (TL[2].tls==false)){
                    car[i]->SetSemafor(false);
                }
                else{
                    car[i]->SetSemafor(true);
                }
            }
                return 0;
            }
            ReleaseDC(hwnd, hdc);
        break;
        }

        case WM_DESTROY:
        {
            KillTimer(hwnd,ID_TIMER1);
            KillTimer(hwnd,ID_TIMER);

            PostQuitMessage (0);
        break;
        }

        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}

