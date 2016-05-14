#include "Car.h"
bool LoadAndBlitBitmap(char* szFileName, HDC hWinDC,int x, int y)
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

Car::Car(POINT center,const int &speed) {
    const char arr[3] = {0, 1, 2};
    right = arr[rand()%3];
    this->center=center;
    this->xSpeed=speed;
    this->ySpeed=speed;
    clr=RGB(255,0,0);

}
bool Car::Accelerate(const int& dSpeedX,const int& dSpeedY) {
    xSpeed=dSpeedX;
    ySpeed=dSpeedY;
    return TRUE;
}

bool Car::Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush) {
    hBrush=CreateSolidBrush(clr);
    SelectObject(hdc,hBrush);
    Collision(rect);
    if (semafor==false){
        center.x=center.x;
    } else {
        center.x+=xSpeed;
    }
    if (xSpeed==0 && right==0){
        center.y+=-abs(ySpeed);
       LoadAndBlitBitmap(TEXT("images/CarUp.bmp"), hdc, center.x-25, center.y-25);

    } else if (xSpeed==0 && right==1){
        center.y+=abs(ySpeed);
        LoadAndBlitBitmap(TEXT("images/CarDown.bmp"), hdc, center.x-25, center.y-25);

    } else if (xSpeed==0 && right==2){
        xSpeed=4;
        center.x+=xSpeed;
    LoadAndBlitBitmap(TEXT("images/car.bmp"), hdc, center.x-25, center.y-25);

    }

    if (xSpeed > 0){
    LoadAndBlitBitmap(TEXT("images/car.bmp"), hdc, center.x-25, center.y-25);
    }

    SelectObject(hdc,GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
    return TRUE;

}


void Car::SetSemafor(bool sem){
    semafor=sem;
}

bool Car::Collision(const RECT &rect) {

    if(( (right==0) && (center.x-25>(rect.right-1)/2)) || ((right==1) && (center.x+55>(rect.right-1)/2) )){
      xSpeed=0;
    }

    return TRUE;
}
bool Car::Coll(Car &c2){
    if((semafor==false) && (c2.center.x>center.x-95)){
        c2.SetSemafor(false);
    }
 return TRUE;
}
