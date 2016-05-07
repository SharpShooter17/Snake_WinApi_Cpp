#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <deque>
#include <ctime>
#include <cstdlib>
#include <mmsystem.h>

static const unsigned WIDTH = 700;
static const unsigned HEIGHT = 500;

void Ruch();
void UaktualnijObraz(HWND hwnd);
void tlo(HWND hwnd);
bool skucha();
void papu(HWND hwnd);
bool bAktywneOkno = true;
void Reset(HWND hwnd);
bool gra = true;

HINSTANCE hInst;

std::deque <POINT> Wspolrzedne_Weza;
POINT jedzenie;
int Size_Snake = 1;
int kierunek = VK_DOWN;

HPEN YellowPen = CreatePen(PS_SOLID, 10,0x00FFFF);
HPEN PapuPen = CreatePen(PS_SOLID, 10, 0x00FF00);
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, TEXT("snake.ico"));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;
    hInst = hThisInstance;
    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, //WS_SYSMENU|WS_CAPTION|WS_SIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           WIDTH,                 /* The programs width */
           HEIGHT,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);


    Reset(hwnd);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
        if (bAktywneOkno){
            if (gra){
                if (!skucha()){
                    Ruch();
                    papu(hwnd);
                    UaktualnijObraz(hwnd);
                    Sleep(10);
                }
            } else {
                /*menu*/
            }
        }
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
    case WM_PAINT:
        break;
    case WM_ACTIVATE:
        if(LOWORD(wParam) == WA_INACTIVE)
           bAktywneOkno = false;
        else bAktywneOkno = true;
        break;
    case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
    default:    /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


void Ruch()
{
    if (GetAsyncKeyState(VK_UP) && kierunek != VK_UP){
        kierunek = VK_UP;
    }
    else if (GetAsyncKeyState(VK_DOWN) && kierunek != VK_DOWN){
        kierunek = VK_DOWN;
    }
    else if (GetAsyncKeyState(VK_LEFT) && kierunek != VK_LEFT){
        kierunek = VK_LEFT;
    }
    else if (GetAsyncKeyState(VK_RIGHT) && kierunek != VK_RIGHT){
        kierunek = VK_RIGHT;
    }

    POINT temp = Wspolrzedne_Weza[Wspolrzedne_Weza.size()-1];

    switch(kierunek)
    {
    case VK_UP:
        temp.y -=1;
        Wspolrzedne_Weza.push_back( temp );
        break;
    case VK_DOWN:
        temp.y +=1;
        Wspolrzedne_Weza.push_back( temp );
        break;
    case VK_LEFT:
        temp.x -=1;
        Wspolrzedne_Weza.push_back( temp );
        break;
    case VK_RIGHT:
        temp.x +=1;
        Wspolrzedne_Weza.push_back( temp );
        break;

    }
}

void UaktualnijObraz(HWND hwnd)
{
    Wspolrzedne_Weza.pop_front();
    tlo(hwnd);
    HDC hdc = GetDC(hwnd);
    SelectObject(hdc, YellowPen);
    for (int i = 0; i < Wspolrzedne_Weza.size(); i++ )
    {
        MoveToEx(hdc,Wspolrzedne_Weza[i].x,Wspolrzedne_Weza[i].y,NULL);
        LineTo(hdc, Wspolrzedne_Weza[i].x,Wspolrzedne_Weza[i].y);
    }
    SelectObject(hdc, PapuPen);
    Ellipse(hdc, jedzenie.x,jedzenie.y,jedzenie.x+3,jedzenie.y+3);
    ReleaseDC(hwnd,hdc);
}

void tlo(HWND hwnd)
{
    HDC hdcOkno = GetDC(hwnd);
    MoveToEx(hdcOkno, 0,0,NULL);
    HPEN RedPioro = CreatePen(PS_SOLID, 10, 0x0000FF);
    SelectObject(hdcOkno,RedPioro);

    POINT TLO[4];
    TLO[0].x =0;
    TLO[0].y =0;

    TLO[1].x =WIDTH-20;
    TLO[1].y =0;

    TLO[2].x =WIDTH-20;
    TLO[2].y =HEIGHT-40;

    TLO[3].x =0;
    TLO[3].y =HEIGHT-40;

    HBRUSH PedzelBlack = CreateSolidBrush(0x000000); //czarny pedzel xD
    SelectObject(hdcOkno, PedzelBlack);
    Polygon(hdcOkno, TLO, 4);
    DeleteObject(RedPioro);
    DeleteObject(PedzelBlack);

    ReleaseDC(hwnd,hdcOkno);
}

bool skucha()
{
    POINT tmp = Wspolrzedne_Weza[Wspolrzedne_Weza.size()-1];
    if (tmp.x < 10 || tmp.x > WIDTH - 30 || tmp.y < 10 || tmp.y > HEIGHT - 50 ){
        gra = false;
        return true;
    }

    POINT TEMP;
    for (int i = 0; i< (Wspolrzedne_Weza.size()-1); i++){
        TEMP = Wspolrzedne_Weza[i];
        if ( TEMP.x == tmp.x && TEMP.y == tmp.y ){
            gra = false;
            return true;
        }
    }

    return false;
}

void papu(HWND hwnd)
{
    POINT tmp = Wspolrzedne_Weza[Wspolrzedne_Weza.size()-1];
    HDC Ekran = GetDC(hwnd);
    COLORREF kolor = GetPixel(Ekran,tmp.x,tmp.y);

    if( (GetRValue( kolor ) == 0x0 &&  GetGValue( kolor ) == 0xFF && GetBValue( kolor ) == 0x0) ){
        //PlaySound((LPCSTR) "snake.wav", NULL, SND_FILENAME);
        tmp.x = 0;
        tmp.y = 0;
       for (int i =0;i<10;i++)
        Wspolrzedne_Weza.push_front( tmp );
        srand((unsigned) time(NULL));
        jedzenie.x = 20 + rand() % WIDTH;
        jedzenie.y = 20 + rand() % HEIGHT;
    }

    if (jedzenie.x > WIDTH-40 || jedzenie.x < 10 || jedzenie.y > HEIGHT-50 || jedzenie.y <10){
        srand((unsigned) time(NULL));
        jedzenie.x = 15 + rand() % WIDTH;
        jedzenie.y = 15 + rand() % HEIGHT;
    }
}

void Reset(HWND hwnd)
{
        /*reset ogona weza*/
    for (int i = 0; i<Wspolrzedne_Weza.size(); i++)
        Wspolrzedne_Weza.pop_back();

        /*TLO GRY*/
    tlo(hwnd);
        //jedzenie
    srand(time(NULL));
    jedzenie.x = rand() % (WIDTH-20) + 20;
    jedzenie.y = rand() % (HEIGHT-20) + 20;

        /* ustawienie weza*/
    POINT tmp;
    tmp.x = WIDTH/2;
    tmp.y = HEIGHT/2;

    Wspolrzedne_Weza.push_back( tmp );
}
