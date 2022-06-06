#include <tchar.h>
#include <list>
#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include<stack>
#include <cmath>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#pragma comment(lib,"opengl32")
#pragma comment(lib,"glu32")
#define MAXENTRIES 600
using namespace std;
HMENU hMenu;
struct Vertex
{
    double x,y;
    Vertex(int x1=0,int y1=0)
        {
            x=x1;
            y=y1;
        }
};
typedef vector<Vertex> VertexList;
typedef bool (*IsInFunc)(Vertex& v,int edge);
typedef Vertex (*IntersectFunc)(Vertex& v1,Vertex& v2,int edge);
union OutCode
{
    unsigned All:4;
    struct{unsigned left:1,top:1,right:1,bottom:1;};
};
struct Entry
{
    int xmin,xmax;
};
struct EdgeRec
{
   double x;
   double minv;
   int ymax;
   bool operator<(EdgeRec r)
   {
        return x<r.x;
   }
};
typedef list<EdgeRec> EdgeList;
void swap(int& x,int& y);
void ADDMenue(HWND hwnd);
void DrawDDALine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c);
void DrawMidpointLine(HDC hdc,int x1,int y1,int x2,int y2,COLORREF c);
void parametricLine(HDC hdc,int x1,int y1,int x2,int y2,COLORREF c);
void parametricLine(HDC hdc,int x1,int y1,int x2,int y2,COLORREF c);
void Draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color);
void circleDirect(HDC hdc,int xc,int yc, int R,COLORREF color);
void circlePolar(HDC hdc,int xc,int yc, int R,COLORREF color);
void circleIterativePolar(HDC hdc,int xc,int yc, int R,COLORREF color);
void circleBresenham(HDC hdc,int xc,int yc, int R,COLORREF color);
void circleFasterBresenham(HDC hdc,int xc,int yc, int R,COLORREF color);
void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color);
void ellipseDirect(HDC hdc,int xc,int yc, int A, int B, COLORREF color);
void ellipsePolar(HDC hdc,int xc,int yc, int A, int B, COLORREF color);
void ellipseBresenham(HDC hdc,int xs1,int ys1,int rx, int ry,COLORREF c);
void DrawRectangleandSquare(HDC hdc,Vertex p1 ,Vertex p2,Vertex p3,Vertex p4,COLORREF color);
void DrawPolygon(HDC hdc,vector<Vertex> v ,int n,COLORREF color);
void PointClippinguseRectagleWindow(HDC hdc,int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color);
OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom);
void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi);
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi);
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom);
VertexList ClipWithEdge(VertexList p,int edge,IsInFunc In,IntersectFunc Intersect);
bool InLeft(Vertex& v,int edge);
bool InRight(Vertex& v,int edge);
bool InTop(Vertex& v,int edge);
bool InBottom(Vertex& v,int edge);
Vertex VIntersect(Vertex& v1,Vertex& v2,int xedge);
Vertex HIntersect(Vertex& v1,Vertex& v2,int yedge);
void PolygonClip(HDC hdc,vector<Vertex>v,int n,int xleft,int ytop,int xright,int ybottom);
void clippinpointguseCircle(HDC hdc,int x,int y,int xc,int yc,int R,COLORREF c);
void clippingLineuseCircle(HDC hdc,int x1,int y1,int x2,int y2,int xc,int yc,int R);
void NRFloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf);
void FloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf);
void InitEntries(Entry table[]);
void ScanEdge(Vertex v1,Vertex v2,Entry table[]);
void DrawSanLines(HDC hdc,Entry table[],COLORREF color);
void ConvexFill(HDC hdc,vector<Vertex> v,int n,COLORREF color);
EdgeRec InitEdgeRec(Vertex& v1,Vertex& v2);
void InitEdgeTable(vector<Vertex>v,int n,EdgeList table[]);
void GeneralPolygonFill(HDC hdc,vector<Vertex> v,int n,COLORREF c);
void Draw2point1(HDC hdc,int xc,int yc,int x,int y,COLORREF c);
void Draw2point2(HDC hdc,int xc,int yc,int x,int y,COLORREF c);
void Draw2point3(HDC hdc,int xc,int yc,int x,int y,COLORREF c);
void Draw2point4(HDC hdc,int xc,int yc,int x,int y,COLORREF c);
void fillQuarterofcircleuseathorline(HDC hdc,double xc,double yc,int R,int num,COLORREF c);
void fillQuarterofcircleuseathorcircle(HDC hdc,int xc,int yc, int R,int count,COLORREF color);
void DrawHermiteCurve(HDC hdc,Vertex p1,Vertex T1,Vertex p2,Vertex T2,COLORREF c);
void DrawCardinalSpline(HDC hdc,Vertex P[],int n,double c,COLORREF color);
void DrawBezierCurve(HDC hdc,Vertex P0,Vertex P3,COLORREF c);
void loaddata(HDC hdc,HWND hwnd);
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
static int side;
static int n;
static int y;
static int A;
static int B;
static int x;
int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nCmdShow)
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
    wincl.hCursor = LoadCursor (NULL, IDC_HAND);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground =(HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
    if (!RegisterClassEx (&wincl))
        return 0;
    hwnd = CreateWindowEx (0,szClassName,_T("Code::Blocks Template Windows App"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,  CW_USEDEFAULT,2256,  1504,HWND_DESKTOP,NULL,hThisInstance,NULL);
           cout<<"cin square side length"<<endl;
           cin>>side;
           cout<<"cin number of point cardinal splines"<<endl;
           cin>>n;
           cout<<"cin number of point polygon"<<endl;
           cin>>y;
           cout<<"cin the A to ellips"<<endl;
           cin>>A;
           cout<<"cin the B to ellips"<<endl;
           cin>>B;
           cout<<"click :1 to go the screen window"<<endl;
           cin>>x;
           lable:
           if(x==1)
            {
                ShowWindow (hwnd, nCmdShow);
            }
            else
            {
                cin>>x;
                goto lable;
            }
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;
}
static int index1=0;
static int index2=0;
static int index3=0;
static int index4=0;
static int index5=0;
static int index6=0;
static int index7=0;
static int index8=0;
static int index9=0;
static int index10=0;
static int index11=0;
static int index12=0;
static vector<Vertex>DDA1;
static vector<COLORREF>colorDDA1;
static vector<Vertex>Prametric1;
static vector<COLORREF>colorPrametric1;
static vector<Vertex>MidPoint1;
static vector<COLORREF>colorMidPoint1;
static vector<Vertex>CircleDirect1;
static vector<COLORREF>colorCircleDirect1;
static vector<Vertex>CirclePolar1;
static vector<COLORREF>colorCirclePolar1;
static vector<Vertex>CircleIterativePolar1;
static vector<COLORREF>colorCircleIterativePolar1;
static vector<Vertex>CircleBresenham1;
static vector<COLORREF>colorCircleBresenham1;
static vector<Vertex>CircleFasterBresenham1;
static vector<COLORREF>colorCircleFasterBresenham1;
static vector<Vertex>EllipseDirect1;
static vector<COLORREF>colorEllipseDirect1;
static vector<Vertex>EllipsePolar1;
static vector<COLORREF>colorEllipsePolar1;
static vector<Vertex>EllipseBresenham1;
static vector<COLORREF>colorEllipseBresenham1;
static vector<Vertex>DrawRectangle1;
static vector<COLORREF>colorDrawRectangle1;
static vector<Vertex>DrawSquare1;
static vector<COLORREF>colorDrawSquare1;
static vector<Vertex>DrawLine1;
static vector<COLORREF>colorDrawPoint1;
static vector<Vertex>DrawPoint1;
static vector<Vertex>polygonforclipping1;
static vector<COLORREF>colorpolygonforclipping1;
static vector<Vertex>polygonforconvex1;
static vector<COLORREF>colorpolygonforconvex1;
static vector<Vertex>polygonfornonconvex1;
static vector<COLORREF>colorpolygonfornonconvex1;
static vector<COLORREF>colorDrawCircle1;
static vector<Vertex>DrawCircle1;
static vector<COLORREF>colorfillQuruseline1;
static vector<Vertex>fillQuruseline1;
static vector<COLORREF>colorfillQuruseline2;
static vector<Vertex>fillQuruseline2;
static vector<COLORREF>colorfillQuruseline3;
static vector<Vertex>fillQuruseline3;
static vector<COLORREF>colorfillQuruseline4;
static vector<Vertex>fillQuruseline4;
static vector<COLORREF>colorfillQuruseCircle1;
static vector<Vertex>fillQuruseCircle1;
static vector<COLORREF>colorfillQuruseCircle2;
static vector<Vertex>fillQuruseCircle2;
static vector<COLORREF>colorfillQuruseCircle3;
static vector<Vertex>fillQuruseCircle3;
static vector<COLORREF>colorfillQuruseCircle4;
static vector<Vertex>fillQuruseCircle4;
static Vertex*CardinalSplineCurve1=new Vertex[n];
static vector<COLORREF>colorCardinalSplineCurve1;
static vector<Vertex>fillRcursev1;
static vector<COLORREF>colorfillRcursev1;
static vector<Vertex>fillnonRcursev1;
static vector<COLORREF>colorfillnonRcursev1;
static vector<Vertex>clipRectanglepoint1;
static vector<Vertex>clipRectangleLine1;
static vector<Vertex>clipRectanglepolygon1;
static vector<Vertex>clipSquarepoint1;
static vector<Vertex>clipSquareLine1;
static vector<Vertex>clipCirclepoint1;
static vector<Vertex>clipCircleLine1;
static vector<Vertex>filluseHermite1;
static vector<Vertex>filluseBezire1;
static vector<COLORREF>colorfillSquare1;
static vector<COLORREF>colorfillRectengle1;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Vertex p1(500,200);
    static Vertex p2(900,200);
    static Vertex p3(900,400);
    static Vertex p4(500,400);
    static double R;
    static int index=0;
    static HDC hdc;
    static int flage;
    static COLORREF c;
    static Vertex DDA[2];
    static Vertex Prametric[2];
    static Vertex MidPoint[2];
    static Vertex CircleDirect[2];
    static Vertex CirclePolar[2];
    static Vertex CircleIterativePolar[2];
    static Vertex CircleBresenham[2];
    static Vertex CircleFasterBresenham[2];
    static Vertex EllipseDirect[1];
    static Vertex EllipsePolar[1];
    static Vertex EllipseBresenham[1];
    static Vertex DrawRectangle[2];
    static Vertex DrawSquare[1];
    static Vertex DrawLine[2];
    static Vertex DrawPoint[1];
    static Vertex DrawCircle[2];
    static Vertex fillRcursev[1];
    static Vertex fillnonRcursev[1];
    static Vertex clipRectanglepoint[2];
    static Vertex clipRectangleLine[2];
    static Vertex clipRectanglepolygon[2];
    static Vertex clipSquarepoint[2];
    static Vertex clipSquareLine[2];
    static Vertex clipCirclepoint[2];
    static Vertex clipCircleLine[2];
    static Vertex filluseHermite[4];
    static Vertex filluseBezire[2];
    switch (message)
    {
       case WM_LBUTTONDOWN:
            if(flage==0)
            {
                DDA[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                DDA1.push_back(DDA[index]);
                if(index==1)
                {
                    colorDDA1.push_back(c);
                    colorDDA1.push_back(c);
                    hdc=GetDC(hwnd);
                    DrawDDALine(hdc,DDA[0].x,DDA[0].y,DDA[1].x,DDA[1].y,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }
            }
            else if(flage==1)
            {
                MidPoint[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                MidPoint1.push_back(MidPoint[index]);
                if(index==1)
                {
                    colorMidPoint1.push_back(c);
                    colorMidPoint1.push_back(c);
                    hdc=GetDC(hwnd);
                    DrawMidpointLine(hdc,MidPoint[0].x,MidPoint[0].y,MidPoint[1].x,MidPoint[1].y,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }
            }
            else if(flage==2)
            {
                Prametric[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                Prametric1.push_back(Prametric[index]);
                if(index==1)
                {
                    colorPrametric1.push_back(c);
                    colorPrametric1.push_back(c);
                    hdc=GetDC(hwnd);
                    parametricLine(hdc,Prametric[0].x,Prametric[0].y,Prametric[1].x,Prametric[1].y,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }
            }
            else if(flage==3)
            {
                CircleDirect[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                CircleDirect1.push_back(CircleDirect[index]);
                if(index==1)
                {
                    colorCircleDirect1.push_back(c);
                    colorCircleDirect1.push_back(c);
                    R=sqrt(pow((CircleDirect[0].x-CircleDirect[1].x),2)+pow((CircleDirect[0].y-CircleDirect[1].y),2));
                    hdc=GetDC(hwnd);
                    circleDirect(hdc,CircleDirect[0].x,CircleDirect[0].y,R,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }

            }
            else if(flage==4)
            {
                CirclePolar[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                CirclePolar1.push_back(CirclePolar[index]);
                if(index==1)
                {
                    colorCirclePolar1.push_back(c);
                    colorCirclePolar1.push_back(c);
                    R=sqrt(pow((CirclePolar[0].x-CirclePolar[1].x),2)+pow((CirclePolar[0].y-CirclePolar[1].y),2));
                    hdc=GetDC(hwnd);
                    circlePolar(hdc,CirclePolar[0].x,CirclePolar[0].y,R,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }

            }
            else if(flage==5)
            {
                CircleIterativePolar[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                CircleIterativePolar1.push_back(CircleIterativePolar[index]);
                if(index==1)
                {
                    colorCircleIterativePolar1.push_back(c);
                    colorCircleIterativePolar1.push_back(c);
                    R=sqrt(pow((CircleIterativePolar[0].x-CircleIterativePolar[1].x),2)+pow((CircleIterativePolar[0].y-CircleIterativePolar[1].y),2));
                    hdc=GetDC(hwnd);
                    circleIterativePolar(hdc,CircleIterativePolar[0].x,CircleIterativePolar[0].y,R,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }

            }
            else if(flage==6)
            {
                CircleBresenham[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                CircleBresenham1.push_back(CircleBresenham[index]);
                if(index==1)
                {
                    colorCircleBresenham1.push_back(c);
                    colorCircleBresenham1.push_back(c);
                    R=sqrt(pow((CircleBresenham[0].x-CircleBresenham[1].x),2)+pow((CircleBresenham[0].y-CircleBresenham[1].y),2));
                    hdc=GetDC(hwnd);
                    circleBresenham(hdc,CircleBresenham[0].x,CircleBresenham[0].y,R,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }

            }
            else if(flage==7)
            {
                CircleFasterBresenham[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                CircleFasterBresenham1.push_back(CircleFasterBresenham[index]);
                if(index==1)
                {
                    colorCircleFasterBresenham1.push_back(c);
                    colorCircleFasterBresenham1.push_back(c);
                    R=sqrt(pow((CircleFasterBresenham[0].x-CircleFasterBresenham[1].x),2)+pow((CircleFasterBresenham[0].y-CircleFasterBresenham[1].y),2));
                    hdc=GetDC(hwnd);
                    circleFasterBresenham(hdc,CircleFasterBresenham[0].x,CircleFasterBresenham[0].y,R,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }
            }
            else if(flage==8)
            {
                EllipseDirect[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                EllipseDirect1.push_back(EllipseDirect[index]);
                if(index==0)
                {
                    colorEllipseDirect1.push_back(c);
                    hdc=GetDC(hwnd);
                    ellipseDirect(hdc,EllipseDirect[0].x,EllipseDirect[0].y,A,B,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else{
                    index++;
                }
            }
            else if(flage==9)
            {
                EllipsePolar[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                EllipsePolar1.push_back(EllipsePolar[index]);
                if(index==0)
                {
                    colorEllipsePolar1.push_back(c);
                    hdc=GetDC(hwnd);
                    ellipsePolar(hdc,EllipsePolar[0].x,EllipsePolar[0].y,A,B,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else{
                    index++;
                }
            }
            else if(flage==10)
            {
                EllipseBresenham[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                EllipseBresenham1.push_back(EllipseBresenham[index]);
                if(index==0)
                {
                    colorEllipseBresenham1.push_back(c);
                    hdc=GetDC(hwnd);
                    ellipseBresenham(hdc,EllipseBresenham[0].x,EllipseBresenham[0].y,A,B,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else{
                    index++;
                }

            }
            else if(flage==11)
            {
                DrawRectangle[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                DrawRectangle1.push_back(DrawRectangle[index]);
                if(index==1)
                {
                    Vertex p2=Vertex(DrawRectangle[1].x,DrawRectangle[0].y);
                    Vertex p4=Vertex(DrawRectangle[0].x,DrawRectangle[1].y);
                    colorDrawRectangle1.push_back(c);
                    colorDrawRectangle1.push_back(c);
                    hdc=GetDC(hwnd);
                    DrawRectangleandSquare(hdc,DrawRectangle[0],p2,DrawRectangle[1],p4,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }
            }
            else if(flage==12)
            {
                DrawSquare[index]=(Vertex(LOWORD(lParam),HIWORD(lParam)));
                if(index==0)
                {
                    double side1;
                    side1=side/2;
                    Vertex p1=Vertex(DrawSquare[0].x-side,DrawSquare[0].y-side);
                    DrawSquare1.push_back(p1);
                    Vertex p2=Vertex(DrawSquare[0].x+side,DrawSquare[0].y-side);
                    DrawSquare1.push_back(p2);
                    Vertex p3=Vertex(DrawSquare[0].x+side,DrawSquare[0].y+side);
                    DrawSquare1.push_back(p3);
                    Vertex p4=Vertex(DrawSquare[0].x-side,DrawSquare[0].y+side);
                    DrawSquare1.push_back(p4);
                    colorDrawSquare1.push_back(c);
                    colorDrawSquare1.push_back(c);
                    colorDrawSquare1.push_back(c);
                    colorDrawSquare1.push_back(c);
                    hdc=GetDC(hwnd);
                    DrawRectangleandSquare(hdc,p1,p2,p3,p4,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else{
                    index++;
                }
            }
            else if(flage==13)
            {
                DrawLine[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                DrawLine1.push_back(DrawLine[index]);
                if(index==1)
                {
                    index=0;
                }
                else
                {
                    index++;
                }
            }
            else if(flage==14)
            {
                DrawPoint[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                DrawPoint1.push_back(DrawPoint[index]);
                colorDrawPoint1.push_back(c);
                if(index==0)
                {
                    index=0;
                }
                else
                {
                    index++;
                }
            }
            else if(flage==15)
            {
                polygonforclipping1.push_back(Vertex(LOWORD(lParam),HIWORD(lParam)));
                if(index==y-1)
                {
                    index=0;
                }
                else{
                    index++;
                }
            }
            else if(flage==16)
            {
                polygonforconvex1.push_back(Vertex(LOWORD(lParam),HIWORD(lParam)));
                colorpolygonforconvex1.push_back(c);
                if(index==y-1)
                {
                    hdc=GetDC(hwnd);
                    DrawPolygon(hdc,polygonforconvex1,y,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else{
                    index++;
                }
            }
            else if(flage==17)
            {
                polygonfornonconvex1.push_back(Vertex(LOWORD(lParam),HIWORD(lParam)));
                colorpolygonfornonconvex1.push_back(c);
                if(index==y-1)
                {
                    hdc=GetDC(hwnd);
                    DrawPolygon(hdc,polygonfornonconvex1,y,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else{
                    index++;
                }
            }
            else if(flage==18)
            {
                DrawCircle[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                DrawCircle1.push_back(DrawCircle[index]);
                if(index==1)
                {
                    colorDrawCircle1.push_back(c);
                    colorDrawCircle1.push_back(c);
                    R=sqrt(pow((DrawCircle[0].x-DrawCircle[1].x),2)+pow((DrawCircle[0].y-DrawCircle[1].y),2));
                    hdc=GetDC(hwnd);
                    circleDirect(hdc,DrawCircle[0].x,DrawCircle[0].y,R,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else
                {
                    index++;
                }
            }
            else if(flage==19)
            {
                clipRectanglepoint[0]=DrawRectangle1[DrawRectangle1.size()-2];
                clipRectanglepoint1.push_back(clipRectanglepoint[0]);
                clipRectanglepoint[1]=DrawRectangle1[DrawRectangle1.size()-1];
                clipRectanglepoint1.push_back(clipRectanglepoint[1]);
                for(int i=0;i<DrawPoint1.size();i++)
                {
                    hdc=GetDC(hwnd);
                    PointClippinguseRectagleWindow(hdc,DrawPoint1[i].x,DrawPoint1[i].y,clipRectanglepoint[0].x,clipRectanglepoint[0].y,clipRectanglepoint[1].x,clipRectanglepoint[1].y,colorDrawPoint1[i]);
                    ReleaseDC(hwnd,hdc);
                }
            }
            else if(flage==20)
            {
                clipRectangleLine[0]=DrawRectangle1[DrawRectangle1.size()-2];
                clipRectangleLine1.push_back(clipRectangleLine[0]);
                clipRectangleLine[1]=DrawRectangle1[DrawRectangle1.size()-1];
                clipRectangleLine1.push_back(clipRectangleLine[1]);
                for(int i=0;i<DrawLine1.size();i+=2)
                {
                    hdc=GetDC(hwnd);
                    CohenSuth(hdc,DrawLine1[i].x,DrawLine1[i].y,DrawLine1[i+1].x,DrawLine1[i+1].y,clipRectangleLine[0].x,clipRectangleLine[0].y,clipRectangleLine[1].x,clipRectangleLine[1].y);
                    ReleaseDC(hwnd,hdc);
                }
            }
            else if(flage==21)
            {
                clipRectanglepolygon[0]=DrawRectangle1[DrawRectangle1.size()-2];
                clipRectanglepolygon1.push_back(clipRectanglepolygon[0]);
                clipRectanglepolygon[1]=DrawRectangle1[DrawRectangle1.size()-1];
                clipRectanglepolygon1.push_back(clipRectanglepolygon[1]);
                hdc=GetDC(hwnd);
                PolygonClip(hdc,polygonforclipping1,y,clipRectanglepolygon[0].x,clipRectanglepolygon[0].y,clipRectanglepolygon[1].x,clipRectanglepolygon[1].y);
                ReleaseDC(hwnd,hdc);
            }
            else if(flage==22)
            {
                clipSquarepoint[0]=DrawSquare1[DrawSquare1.size()-4];
                clipSquarepoint1.push_back(clipSquarepoint[0]);
                clipSquarepoint[1]=DrawSquare1[DrawSquare1.size()-2];
                clipSquarepoint1.push_back(clipSquarepoint[1]);
                for(int i=0;i<DrawPoint1.size();i++)
                {
                    hdc=GetDC(hwnd);
                    PointClippinguseRectagleWindow(hdc,DrawPoint1[i].x,DrawPoint1[i].y,clipSquarepoint[0].x,clipSquarepoint[0].y,clipSquarepoint[1].x,clipSquarepoint[1].y,colorDrawPoint1[i]);
                    ReleaseDC(hwnd,hdc);
                }

            }
            else if(flage==23)
            {
                clipSquareLine[0]=DrawSquare1[DrawSquare1.size()-4];
                clipSquareLine1.push_back(clipSquareLine[0]);
                clipSquareLine[1]=DrawSquare1[DrawSquare1.size()-2];
                clipSquareLine1.push_back(clipSquareLine[1]);
                for(int i=0;i<DrawLine1.size();i+=2)
                {
                    hdc=GetDC(hwnd);
                    CohenSuth(hdc,DrawLine1[i].x,DrawLine1[i].y,DrawLine1[i+1].x,DrawLine1[i+1].y,clipSquareLine[0].x,clipSquareLine[0].y,clipSquareLine[1].x,clipSquareLine[1].y);
                    ReleaseDC(hwnd,hdc);
                }

            }
            else if(flage==24)
            {
                clipCirclepoint[0]=DrawCircle1[DrawCircle1.size()-2];
                clipCirclepoint1.push_back(clipCirclepoint[0]);
                clipRectanglepoint[1]=DrawCircle1[DrawCircle1.size()-1];
                clipCirclepoint1.push_back(clipCirclepoint[1]);
                R=sqrt(pow((clipCirclepoint[0].x-clipCirclepoint[1].x),2)+pow((clipCirclepoint[0].y-clipCirclepoint[1].y),2));
                for(int i=0;i<DrawPoint1.size();i++)
                {
                    hdc=GetDC(hwnd);
                    clippinpointguseCircle(hdc,DrawPoint1[i].x,DrawPoint1[i].y,clipCirclepoint[0].x,clipCirclepoint[0].y,R,colorDrawPoint1[i]);
                    ReleaseDC(hwnd,hdc);
                }
            }
            else if(flage==25)
            {
                clipCircleLine[0]=DrawCircle1[DrawCircle1.size()-2];
                clipCircleLine1.push_back(clipCircleLine[0]);
                clipCircleLine[1]=DrawCircle1[DrawCircle1.size()-1];
                clipCircleLine1.push_back(clipCircleLine[1]);
                R=sqrt(pow((clipCircleLine[0].x-clipCircleLine[1].x),2)+pow((clipCircleLine[0].y-clipCircleLine[1].y),2));
                for(int i=0;i<DrawLine1.size();i+=2)
                {
                    hdc=GetDC(hwnd);
                    clippingLineuseCircle(hdc,DrawLine1[i].x,DrawLine1[i].y,DrawLine1[i+1].x,DrawLine1[i+1].y,clipCircleLine[0].x,clipCircleLine[0].y,R);
                    ReleaseDC(hwnd,hdc);
                }
            }
            else if(flage==26)
            {
                fillRcursev[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                fillRcursev1.push_back(fillRcursev[index]);
                if(index==0)
                {
                    colorfillRcursev1.push_back(c);
                    hdc=GetDC(hwnd);
                    FloodFill(hdc,fillRcursev[0].x,fillRcursev[0].y,c,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else{
                    index++;
                }
            }
            else if(flage==27)
            {
                fillnonRcursev[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                fillnonRcursev1.push_back(fillnonRcursev[index]);
                if(index==0)
                {
                    colorfillnonRcursev1.push_back(c);
                    hdc=GetDC(hwnd);
                    FloodFill(hdc,fillnonRcursev[0].x,fillnonRcursev[0].y,c,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else{
                    index++;
                }
            }
            else if(flage==28)
            {
                    hdc=GetDC(hwnd);
                    ConvexFill(hdc,polygonforconvex1,y,colorpolygonforconvex1[colorpolygonforconvex1.size()-1]);
                    ReleaseDC(hwnd,hdc);
            }
            else if(flage==29)
            {
                    hdc=GetDC(hwnd);
                    GeneralPolygonFill(hdc,polygonfornonconvex1,y,colorpolygonfornonconvex1[colorpolygonfornonconvex1.size()-1]);
                    ReleaseDC(hwnd,hdc);
            }
            else if(flage==30)
            {
                fillQuruseline1.push_back(CircleDirect1[CircleDirect1.size()-2]);
                fillQuruseline1.push_back(CircleDirect1[CircleDirect1.size()-1]);
                colorfillQuruseline1.push_back(c);
                colorfillQuruseline1.push_back(c);
                if(index==0)
                {
                    R=sqrt(pow((fillQuruseline1[fillQuruseline1.size()-2].x-fillQuruseline1[fillQuruseline1.size()-1].x),2)+pow((fillQuruseline1[fillQuruseline1.size()-2].y-fillQuruseline1[fillQuruseline1.size()-1].y),2));
                    hdc=GetDC(hwnd);
                    fillQuarterofcircleuseathorline(hdc,fillQuruseline1[fillQuruseline1.size()-2].x,fillQuruseline1[fillQuruseline1.size()-2].y,R,1,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
            }
            else if(flage==31)
            {
                fillQuruseline2.push_back(CircleDirect1[CircleDirect1.size()-2]);
                fillQuruseline2.push_back(CircleDirect1[CircleDirect1.size()-1]);
                colorfillQuruseline2.push_back(c);
                colorfillQuruseline2.push_back(c);
                if(index==0)
                {
                    R=sqrt(pow((fillQuruseline2[fillQuruseline2.size()-2].x-fillQuruseline2[fillQuruseline2.size()-1].x),2)+pow((fillQuruseline2[fillQuruseline2.size()-2].y-fillQuruseline2[fillQuruseline2.size()-1].y),2));
                    hdc=GetDC(hwnd);
                    fillQuarterofcircleuseathorline(hdc,fillQuruseline2[fillQuruseline2.size()-2].x,fillQuruseline2[fillQuruseline2.size()-2].y,R,2,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
            }
            else if(flage==32)
            {
                fillQuruseline3.push_back(CircleDirect1[CircleDirect1.size()-2]);
                fillQuruseline3.push_back(CircleDirect1[CircleDirect1.size()-1]);
                colorfillQuruseline3.push_back(c);
                colorfillQuruseline3.push_back(c);
                if(index==0)
                {
                    R=sqrt(pow((fillQuruseline3[fillQuruseline3.size()-2].x-fillQuruseline3[fillQuruseline3.size()-1].x),2)+pow((fillQuruseline3[fillQuruseline3.size()-2].y-fillQuruseline3[fillQuruseline3.size()-1].y),2));
                    hdc=GetDC(hwnd);
                    fillQuarterofcircleuseathorline(hdc,fillQuruseline3[fillQuruseline3.size()-2].x,fillQuruseline3[fillQuruseline3.size()-2].y,R,3,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
            }
            else if(flage==33)
            {
                fillQuruseline4.push_back(CircleDirect1[CircleDirect1.size()-2]);
                fillQuruseline4.push_back(CircleDirect1[CircleDirect1.size()-1]);
                colorfillQuruseline4.push_back(c);
                colorfillQuruseline4.push_back(c);
                if(index==0)
                {
                    R=sqrt(pow((fillQuruseline4[fillQuruseline4.size()-2].x-fillQuruseline4[fillQuruseline4.size()-1].x),2)+pow((fillQuruseline4[fillQuruseline4.size()-2].y-fillQuruseline4[fillQuruseline4.size()-1].y),2));
                    hdc=GetDC(hwnd);
                    fillQuarterofcircleuseathorline(hdc,fillQuruseline4[fillQuruseline4.size()-2].x,fillQuruseline4[fillQuruseline4.size()-2].y,R,4,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
            }
            else if(flage==34)
            {
                fillQuruseCircle1.push_back(CircleDirect1[CircleDirect1.size()-2]);
                fillQuruseCircle1.push_back(CircleDirect1[CircleDirect1.size()-1]);
                colorfillQuruseCircle1.push_back(c);
                colorfillQuruseCircle1.push_back(c);
                if(index==0)
                {
                    R=sqrt(pow((fillQuruseCircle1[fillQuruseCircle1.size()-2].x-fillQuruseCircle1[fillQuruseCircle1.size()-1].x),2)+pow((fillQuruseCircle1[fillQuruseCircle1.size()-2].y-fillQuruseCircle1[fillQuruseCircle1.size()-1].y),2));
                    for(double i=0;i<R;i+=1)
                    {
                        hdc=GetDC(hwnd);
                        fillQuarterofcircleuseathorcircle(hdc,fillQuruseCircle1[fillQuruseCircle1.size()-2].x,fillQuruseCircle1[fillQuruseCircle1.size()-2].y,i,1,c);
                        ReleaseDC(hwnd,hdc);
                    }
                    index=0;
                }
            }
            else if(flage==35)
            {
                fillQuruseCircle2.push_back(CircleDirect1[CircleDirect1.size()-2]);
                fillQuruseCircle2.push_back(CircleDirect1[CircleDirect1.size()-1]);
                colorfillQuruseCircle2.push_back(c);
                colorfillQuruseCircle2.push_back(c);
                if(index==0)
                {
                    R=sqrt(pow((fillQuruseCircle2[fillQuruseCircle2.size()-2].x-fillQuruseCircle2[fillQuruseCircle2.size()-1].x),2)+pow((fillQuruseCircle2[fillQuruseCircle2.size()-2].y-fillQuruseCircle2[fillQuruseCircle2.size()-1].y),2));
                    for(double i=0;i<R;i+=1)
                    {
                        hdc=GetDC(hwnd);
                        fillQuarterofcircleuseathorcircle(hdc,fillQuruseCircle2[fillQuruseCircle2.size()-2].x,fillQuruseCircle2[fillQuruseCircle2.size()-2].y,i,2,c);
                        ReleaseDC(hwnd,hdc);
                    }
                    index=0;
                }
            }
            else if(flage==36)
            {
                fillQuruseCircle3.push_back(CircleDirect1[CircleDirect1.size()-2]);
                fillQuruseCircle3.push_back(CircleDirect1[CircleDirect1.size()-1]);
                colorfillQuruseCircle3.push_back(c);
                colorfillQuruseCircle3.push_back(c);
                if(index==0)
                {
                    R=sqrt(pow((fillQuruseCircle3[fillQuruseCircle3.size()-2].x-fillQuruseCircle3[fillQuruseCircle3.size()-1].x),2)+pow((fillQuruseCircle3[fillQuruseCircle3.size()-2].y-fillQuruseCircle3[fillQuruseCircle3.size()-1].y),2));
                    for(double i=0;i<R;i+=1)
                    {
                        hdc=GetDC(hwnd);
                        fillQuarterofcircleuseathorcircle(hdc,fillQuruseCircle3[fillQuruseCircle3.size()-2].x,fillQuruseCircle3[fillQuruseCircle3.size()-2].y,i,3,c);
                        ReleaseDC(hwnd,hdc);
                    }
                    index=0;
                }
            }
            else if(flage==37)
            {
                fillQuruseCircle4.push_back(CircleDirect1[CircleDirect1.size()-2]);
                fillQuruseCircle4.push_back(CircleDirect1[CircleDirect1.size()-1]);
                colorfillQuruseCircle4.push_back(c);
                colorfillQuruseCircle4.push_back(c);
                if(index==0)
                {
                    R=sqrt(pow((fillQuruseCircle4[fillQuruseCircle4.size()-2].x-fillQuruseCircle4[fillQuruseCircle4.size()-1].x),2)+pow((fillQuruseCircle4[fillQuruseCircle4.size()-2].y-fillQuruseCircle4[fillQuruseCircle4.size()-1].y),2));
                    for(double i=0;i<R;i+=1)
                    {
                        hdc=GetDC(hwnd);
                        fillQuarterofcircleuseathorcircle(hdc,fillQuruseCircle4[fillQuruseCircle4.size()-2].x,fillQuruseCircle4[fillQuruseCircle4.size()-2].y,i,4,c);
                        ReleaseDC(hwnd,hdc);
                    }

                    index=0;
                }
            }
            else if(index11==1)
            {
                filluseHermite[0]=DrawSquare1[DrawSquare1.size()-4];
                filluseHermite1.push_back(filluseHermite[0]);
                filluseHermite[1]=DrawSquare1[DrawSquare1.size()-3];
                filluseHermite1.push_back(filluseHermite[1]);
                filluseHermite[2]=DrawSquare1[DrawSquare1.size()-2];
                filluseHermite1.push_back(filluseHermite[2]);
                filluseHermite[3]=DrawSquare1[DrawSquare1.size()-1];
                filluseHermite1.push_back(filluseHermite[3]);
                colorfillSquare1.push_back(c);
                colorfillSquare1.push_back(c);
                colorfillSquare1.push_back(c);
                colorfillSquare1.push_back(c);
                colorfillSquare1.push_back(c);
                for(int i=filluseHermite[1].y;i<filluseHermite[2].y;i++)
                {
                         Vertex p11(filluseHermite[0].x,i);
                         Vertex p12(filluseHermite[1].x,i);
                         hdc=GetDC(hwnd);
                         DrawHermiteCurve(hdc,p11,(0,0),p12,(0,0),c);
                         ReleaseDC(hwnd,hdc);
                }
            }
            else if(index12==1)
            {
                filluseBezire[0]=DrawRectangle1[DrawRectangle1.size()-2];
                filluseBezire1.push_back(filluseBezire[0]);
                filluseBezire[1]=DrawRectangle1[DrawRectangle1.size()-1];
                filluseBezire1.push_back(filluseBezire[1]);
                colorfillRectengle1.push_back(c);
                colorfillRectengle1.push_back(c);
                for(int i=filluseBezire[0].x;i<filluseBezire[1].x;i++)
                {
                         Vertex p11(i,filluseBezire[0].y);
                         Vertex p12(i,filluseBezire[1].y);
                         hdc=GetDC(hwnd);
                         DrawBezierCurve(hdc,p11,p12,c);
                         ReleaseDC(hwnd,hdc);
                }
            }
            else if(flage==40)
            {
                CardinalSplineCurve1[index]=Vertex(LOWORD(lParam),HIWORD(lParam));
                colorCardinalSplineCurve1.push_back(c);
                if(index==n-1)
                {
                    hdc=GetDC(hwnd);
                    DrawCardinalSpline(hdc,CardinalSplineCurve1,n,0.5,c);
                    ReleaseDC(hwnd,hdc);
                    index=0;
                }
                else{
                    index++;
                }
            }
            break;
        case WM_COMMAND:
            switch(wParam)
            {
                case 1:
                   c=RGB(255,0,0);
                   break;
               case 2:
                   c=RGB(0,255,0);
                   break;
               case 3:
                   c=RGB(0,0,255);
                   break;
               case 4:
                   c=RGB(0,0,0);
                   break;
               case 5:
                   c=RGB(238,130,238);
                   break;
               case 6:
                   c=RGB(255,165,0);
                   break;
               case 7:
                   c=RGB(128,128,128);
                   break;
               case 8:
                   c=RGB(128,128,0);
                   break;
               case 9:
                   c=RGB(255,255,0);
                   break;
               case 10:
                   c=RGB(139,69,19);
                   break;
               case 11:
                    flage=0;
                   break;
               case 12:
                   flage=1;
                   break;
               case 13:
                   flage=2;
                   break;
               case 14:
                   flage=3;
                   break;
               case 15:
                   flage=4;
                   break;
               case 16:
                   flage=5;
                   break;
               case 17:
                   flage=6;
                   break;
               case 18:
                   flage=7;
                   break;
               case 19:
                   flage=8;
                   break;
               case 20:
                   flage=9;
                   break;
               case 21:
                   flage=10;
                   break;
               case 22:
                   flage=11;
                   break;
               case 23:
                   flage=12;
                   break;
               case 24:
                   flage=13;
                   break;
               case 25:
                   flage=14;
                   break;
               case 26:
                   flage=15;
                   break;
               case 27:
                   flage=16;
                   break;
               case 28:
                   flage=17;
                   break;
               case 29:
                   flage=18;
                   break;
               case 30:
                   flage=19;
                   index1=1;
                   break;
               case 31:
                   flage=20;
                   index2=1;
                   break;
               case 32:
                   flage=21;
                   index3=1;
                   break;
               case 33:
                   flage=22;
                   index4=1;
                   break;
               case 34:
                   flage=23;
                   index5=1;
                   break;
               case 35:
                   flage=24;
                   index6=1;
                   break;
               case 36:
                   flage=25;
                   index7=1;
                   break;
               case 37:
                   flage=26;
                   break;
               case 38:
                   flage=27;
                   break;
               case 39:
                   flage=28;
                   index8=1;
                   break;
               case 40:
                   flage=29;
                   index9=1;
                   break;
               case 41:
                   flage=30;
                   break;
               case 42:
                   flage=31;
                   break;
               case 43:
                   flage=32;
                   break;
               case 44:
                   flage=33;
                   break;
               case 45:
                   flage=34;
                   break;
               case 46:
                   flage=35;
                   break;
               case 47:
                   flage=36;
                   break;
               case 48:
                   flage=37;
                   break;
               case 49:
                   flage=38;
                   index11=1;
               case 50:
                   flage=39;
                   index12=1;
               case 51:
                   flage=40;
                   index10=1;
                   break;
               case 52:
                   loaddata(hdc,hwnd);
                   break;
               case 53:
                   InvalidateRect(hwnd, NULL, TRUE);
                   break;
               case 54:
                   PostQuitMessage (0);
                   break;

            }
            break;
        case WM_CREATE:
            ADDMenue(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
void swap(int& x,int& y)
{
    int tmp=x;
    x=y;
    y=tmp;
}
void ADDMenue(HWND hwnd)
{
    hMenu=CreateMenu();
    HMENU subMenu=CreateMenu();
    AppendMenu(subMenu,MF_STRING,1,"red");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(subMenu,MF_STRING,2,"green");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(subMenu,MF_STRING,3,"blue");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(subMenu,MF_STRING,4,"black");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(subMenu,MF_STRING,5,"violet");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(subMenu,MF_STRING,6,"orange");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(subMenu,MF_STRING,7,"grey");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(subMenu,MF_STRING,8,"olive");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(subMenu,MF_STRING,9,"yellow");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(subMenu,MF_STRING,10,"brown");
    AppendMenu(subMenu,MF_STRING,NULL,NULL);
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu,"color");
    HMENU subMenu1=CreateMenu();
    AppendMenu(subMenu1,MF_STRING,11,"DDA");
    AppendMenu(subMenu1,MF_STRING,NULL,NULL);
    AppendMenu(subMenu1,MF_STRING,12,"Midpoint");
    AppendMenu(subMenu1,MF_STRING,NULL,NULL);
    AppendMenu(subMenu1,MF_STRING,13,"parametric");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu1,"line algorithms");
    HMENU subMenu2=CreateMenu();
    AppendMenu(subMenu2,MF_STRING,14,"Direct");
    AppendMenu(subMenu2,MF_STRING,NULL,NULL);
    AppendMenu(subMenu2,MF_STRING,15,"Polar");
    AppendMenu(subMenu2,MF_STRING,NULL,NULL);
    AppendMenu(subMenu2,MF_STRING,16,"iterative Polar");
    AppendMenu(subMenu2,MF_STRING,NULL,NULL);
    AppendMenu(subMenu2,MF_STRING,17,"midpoint");
    AppendMenu(subMenu2,MF_STRING,NULL,NULL);
    AppendMenu(subMenu2,MF_STRING,18,"modified Midpoint");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu2,"Circle algorithms");
    HMENU subMenu3=CreateMenu();
    AppendMenu(subMenu3,MF_STRING,19,"Direct");
    AppendMenu(subMenu3,MF_STRING,NULL,NULL);
    AppendMenu(subMenu3,MF_STRING,20,"Polar");
    AppendMenu(subMenu3,MF_STRING,NULL,NULL);
    AppendMenu(subMenu3,MF_STRING,21,"midpoint");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu3,"Ellipse Algorithms");
    AppendMenu(hMenu,MF_STRING,22,"Draw Rectangle");
    AppendMenu(hMenu,MF_STRING,23,"Draw Square");
    AppendMenu(hMenu,MF_STRING,24,"Draw Line");
    AppendMenu(hMenu,MF_STRING,25,"Draw point");
    HMENU subMenu4=CreateMenu();
    AppendMenu(subMenu4,MF_STRING,26,"polygon for clipping");
    AppendMenu(subMenu4,MF_STRING,NULL,NULL);
    AppendMenu(subMenu4,MF_STRING,27,"polygon for Convex");
    AppendMenu(subMenu4,MF_STRING,NULL,NULL);
    AppendMenu(subMenu4,MF_STRING,28,"polygon for nonConvex");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu4,"Draw polygon");
    AppendMenu(hMenu,MF_STRING,29,"Draw Circle");
    HMENU subMenu5=CreateMenu();
    AppendMenu(subMenu5,MF_STRING,30,"Point ");
    AppendMenu(subMenu5,MF_STRING,NULL,NULL);
    AppendMenu(subMenu5,MF_STRING,31,"Line");
    AppendMenu(subMenu5,MF_STRING,NULL,NULL);
    AppendMenu(subMenu5,MF_STRING,32,"Polygon");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu5,"Clipping algorithms using Rectangle");
    HMENU subMenu6=CreateMenu();
    AppendMenu(subMenu6,MF_STRING,33,"Point ");
    AppendMenu(subMenu6,MF_STRING,NULL,NULL);
    AppendMenu(subMenu6,MF_STRING,34,"Line");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu6,"Clipping algorithms using Square");
    HMENU subMenu7=CreateMenu();
    AppendMenu(subMenu7,MF_STRING,35,"Point ");
    AppendMenu(subMenu7,MF_STRING,NULL,NULL);
    AppendMenu(subMenu7,MF_STRING,36,"Line");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu7,"Clipping algorithms using circle");
    HMENU subMenu8=CreateMenu();
    AppendMenu(subMenu8,MF_STRING,37,"Recursive");
    AppendMenu(subMenu8,MF_STRING,NULL,NULL);
    AppendMenu(subMenu8,MF_STRING,38,"Non Recursive");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu8,"FloodFill");
    HMENU subMenu9=CreateMenu();
    AppendMenu(subMenu9,MF_STRING,39,"Convex");
    AppendMenu(subMenu9,MF_STRING,NULL,NULL);
    AppendMenu(subMenu9,MF_STRING,40,"Non Convex");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu9,"Filling Algorithm");
    HMENU subMenu10=CreateMenu();
    AppendMenu(subMenu10,MF_STRING,41,"quarter 1");
    AppendMenu(subMenu10,MF_STRING,NULL,NULL);
    AppendMenu(subMenu10,MF_STRING,42,"quarter 2");
    AppendMenu(subMenu10,MF_STRING,NULL,NULL);
    AppendMenu(subMenu10,MF_STRING,43,"quartert 3");
    AppendMenu(subMenu10,MF_STRING,NULL,NULL);
    AppendMenu(subMenu10,MF_STRING,44,"quarter 4");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu10,"Filling Circle with lines");
    HMENU subMenu11=CreateMenu();
    AppendMenu(subMenu11,MF_STRING,45,"quarter 1");
    AppendMenu(subMenu11,MF_STRING,NULL,NULL);
    AppendMenu(subMenu11,MF_STRING,46,"quarter 2");
    AppendMenu(subMenu11,MF_STRING,NULL,NULL);
    AppendMenu(subMenu11,MF_STRING,47,"quartert 3");
    AppendMenu(subMenu11,MF_STRING,NULL,NULL);
    AppendMenu(subMenu11,MF_STRING,48,"quarter 4");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)subMenu11,"Filling Circle with other circles");
    AppendMenu(hMenu,MF_STRING,49,"Filling Square with Hermit Curve");
    AppendMenu(hMenu,MF_STRING,50,"Filling Rectangle with Bezier Curve");
    AppendMenu(hMenu,MF_STRING,51,"Cardinal Spline Curve");
    AppendMenu(hMenu,MF_STRING,52,"load");
    AppendMenu(hMenu,MF_STRING,53,"clear screen");
    AppendMenu(hMenu,MF_STRING,54,"Destroy Screen");
    SetMenu(hwnd,hMenu);
}
void DrawDDALine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dy = y2 - y1;
    int dx = x2 - x1;
    if (abs(dx) >= abs(dy))
    {
        if (x2 < x1)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        SetPixel(hdc, x1, y1, c);
        double x = x1;
        double y = y1;
        while (x < x2)
        {
            x++;
            y = y + (double)dy / dx;
            SetPixel(hdc, x, y, c);
        }
    }
    else
    {
        if (y2 < y1)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        SetPixel(hdc, x1, y1, c);
        double x = x1;
        double y = y1;
        while (y < y2)
        {
            y++;
            x = x + (double)dx / dy;
            SetPixel(hdc, x, y, c);
        }
    }
}
void DrawMidpointLine(HDC hdc,int x1,int y1,int x2,int y2,COLORREF c)
{
    int dx=x2-x1;
    int dy=y2-y1;
    if(dy<=dx)
    {
        if(x1>x2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        SetPixel(hdc,x1,y1,c);
        int x=x1;
        int y=y1;
        while(x<x2)
        {
            double d=((y+1/2)-y1)*dx - (x+1-x1)*dy;
            if(d<0)
            {
                x++;
                y++;
            }
            else
            {
                x++;
            }
            SetPixel(hdc,x,y,c);
        }
    }
    else
    {
        if(y1>y2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        SetPixel(hdc,x1,y1,c);
        int x=x1;
        int y=y1;
        while(y<y2)
        {
            double d=(y+1-y1)*dx - ((x+1/2)-x1)*dy;
            if(d>0)
            {
                y++;
                x++;
            }
            else
            {
                y++;
            }
            SetPixel(hdc,x,y,c);
        }
    }
}
void parametricLine(HDC hdc,int x1,int y1,int x2,int y2,COLORREF c)
{
    double dx=x2-x1;
    double dy=y2-y1;
    for(double t=0;t<1;t+=0.001)
    {
        int x=x1+(t*dx);
        int y=y1+(t*dy);
        SetPixel(hdc,x,y,c);
    }
}
void Draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{
    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc-a, yc-b, color);
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc+b, yc+a, color);
    SetPixel(hdc, xc-b, yc+a, color);
    SetPixel(hdc, xc-b, yc-a, color);
    SetPixel(hdc, xc+b, yc-a, color);
}
void circleDirect(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int R2=R*R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        x++;
        y=round(sqrt((double)(R2-x*x)));
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
void circlePolar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    double x, y ;
    double theta = 0, dtheta=1.0/R;
    for (theta ; theta <= 2* M_PI ; theta+=dtheta)
    {
        x = xc + R * cos(theta);
        y = yc + R * sin(theta);
        SetPixel(hdc, x, y, color);
    }
}
void circleIterativePolar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    double x=R,y=0;
    double dtheta=1.0/R;
    double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
    Draw8Points(hdc,xc,yc,R,0,color);
    while(x>y)
    {
        double x1=x*cdtheta-y*sdtheta;
        y=x*sdtheta+y*cdtheta;
        x=x1;
        Draw8Points(hdc,xc,yc,round(x),round(y),color);
    }
}
void circleBresenham(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
            d+=2*x+2;
        else
        {
            d+=2*(x-y)+5;
            y--;
        }
        x++;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
void circleFasterBresenham(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    int c1=3, c2=5-2*R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
        {
            d+=c1;
            c2+=2;
        }
        else
        {
            d+=c2;
            c2+=4;
            y--;
        }
        c1+=2;
        x++;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}
void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
    SetPixel(hdc, xc+x, yc+y, color);
    SetPixel(hdc, xc-x, yc+y, color);
    SetPixel(hdc, xc-x, yc-y, color);
    SetPixel(hdc, xc+x, yc-y, color);
}
void ellipseDirect(HDC hdc,int xc,int yc, int A, int B, COLORREF color)
{
    int x = 0;
    double y = B;
    Draw4Points(hdc,xc,yc,x,y,color);
    while(x*B*B < y*A*A)
    {
        x++;
        y = B*sqrt(1.0 - (double)x*x/(A*A));
        Draw4Points(hdc,xc, yc, x, round(y), color);
    }
    int y1 = 0;
    double x1 = A;
    Draw4Points(hdc,xc,yc,x1,y1,color);
    while(x1*B*B > y1*A*A)
    {
        y1++;
        x1 = A*sqrt(1.0 - (double)y1*y1/(B*B));
        Draw4Points(hdc,xc, yc, round(x1), y1, color);
    }
}
void ellipsePolar(HDC hdc,int xc,int yc, int A, int B, COLORREF color)
{
    double x, y ;
    double theta = 0, dtheta=1.0/(A+B);
    for (theta ; theta <= 2* M_PI ; theta+=dtheta)
    {
        x = xc + A*cos(theta);
        y = yc + B*sin(theta);
        SetPixel(hdc, round(x), round(y), color);
    }
}
void ellipseBresenham(HDC hdc,int xs1,int ys1,int rx, int ry,COLORREF c)
{
    int x,y;
    float d1,d2,dx,dy;
    x = 0;                  // take start position as (0,ry)
    y = ry;                 // finding decision parameter d1
    d1 = pow(ry,2) - (pow(rx,2) * ry) + (0.25 * pow(rx,2));
    dx = 2 * pow(ry,2) * x;
    dy = 2 * pow(rx,2) * y;
    do                         // region one
    {
        Draw4Points(hdc,xs1,ys1,x,y,c);
        if(d1<0)
        {
            x++;
            dx = dx + (2 * (pow(ry,2)));
            d1 = d1 + dx +(pow(ry,2));
        }
        else
        {
            x++;
            y--;
            dx = dx + (2 * (pow(ry,2)));
            dy = dy - (2 * (pow(rx,2)));
            d1 = d1 + dx - dy + (pow(ry,2));
        }
    }while(dx<dy);      // change over condition for region-2
    do                       // region two
    {
        Draw4Points(hdc,xs1,ys1,x,y,c);
        if(d2>0)
        {
            x = x;
            y--;
            dy = dy - (2 * (pow(rx,2)));
            d2 = d2 - dy + pow(rx,2);
        }
        else
        {
            x++;
            y--;
            dy = dy - (2 * (pow(rx,2)));
            dx = dx + (2 * (pow(ry,2)));
            d2 = d2 +dx - dy + pow(rx,2);

        }
    }while(y>0);
}
void DrawRectangleandSquare(HDC hdc,Vertex p1 ,Vertex p2,Vertex p3,Vertex p4,COLORREF color)
{
    DrawDDALine(hdc,p1.x,p1.y,p2.x,p2.y,color);
    DrawDDALine(hdc,p2.x,p2.y,p3.x,p3.y,color);
    DrawDDALine(hdc,p3.x,p3.y,p4.x,p4.y,color);
    DrawDDALine(hdc,p4.x,p4.y,p1.x,p1.y,color);
}
void DrawPolygon(HDC hdc,vector<Vertex> v ,int n,COLORREF color)
{
    for(int i=0;i<y;i++)
    {
        if(i<y-1)
        {
            parametricLine(hdc,v[i].x,v[i].y,v[i+1].x,v[i+1].y,color);
        }
        else if(i==y-1)
        {
            parametricLine(hdc,v[i].x,v[i].y,v[0].x,v[0].y,color);
        }
    }
}
OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out; out.All=0;
    if(x<xleft)out.left=1;
    else if(x>xright)out.right=1;
    if(y<ytop)out.top=1;
    else if(y>ybottom)out.bottom=1;
    return out;
}
void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
    *xi=x;
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
    *yi=y;
}
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi); x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {
            MoveToEx(hdc,x1,y1,NULL);
            LineTo(hdc,x2,y2);
    }
}
VertexList ClipWithEdge(VertexList p,int edge,IsInFunc In,IntersectFunc Intersect)
{
    VertexList OutList;
    Vertex v1=p[p.size()-1];
    bool v1_in=In(v1,edge);
    for(int i=0;i<(int)p.size();i++)
    {
        Vertex v2=p[i];
        bool v2_in=In(v2,edge);
        if(!v1_in && v2_in)
        {
            OutList.push_back(Intersect(v1,v2,edge)); OutList.push_back(v2);
        }
        else if(v1_in && v2_in)
            OutList.push_back(v2);
        else if(v1_in)
            OutList.push_back(Intersect(v1,v2,edge));
        v1=v2;
        v1_in=v2_in;
    }
    return OutList;
}
bool InLeft(Vertex& v,int edge)
{
    return v.x>=edge;
}
bool InRight(Vertex& v,int edge)
{
    return v.x<=edge;
}
bool InTop(Vertex& v,int edge)
{
    return v.y>=edge;
}
bool InBottom(Vertex& v,int edge)
{
    return v.y<=edge;
}
Vertex VIntersect(Vertex& v1,Vertex& v2,int xedge)
{
    Vertex res;
    res.x=xedge;
    res.y=v1.y+(xedge-v1.x)*(v2.y-v1.y)/(v2.x-v1.x);
    return res;
}
Vertex HIntersect(Vertex& v1,Vertex& v2,int yedge)
{
    Vertex res;
    res.y=yedge;
    res.x=v1.x+(yedge-v1.y)*(v2.x-v1.x)/(v2.y-v1.y);
    return res;
}
void PolygonClip(HDC hdc,vector<Vertex>v,int n,int xleft,int ytop,int xright,int ybottom)
{
    VertexList vlist;
    for(int i=0;i<n;i++)
        vlist.push_back(Vertex(v[i].x,v[i].y));
    vlist=ClipWithEdge(vlist,xleft,InLeft,VIntersect);
    vlist=ClipWithEdge(vlist,ytop,InTop,HIntersect);
    vlist=ClipWithEdge(vlist,xright,InRight,VIntersect);
    vlist=ClipWithEdge(vlist,ybottom,InBottom,HIntersect);
    Vertex v1=vlist[vlist.size()-1];
    for(int i=0;i<(int)vlist.size();i++)
    {
        Vertex v2=vlist[i];
        MoveToEx(hdc,v1.x,v1.y,NULL);
        LineTo(hdc,v2.x,v2.y);
        v1=v2;
    }
}
void PointClippinguseRectagleWindow(HDC hdc,int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)
    {
        SetPixel(hdc,x,y,color);
    }
}
void clippinpointguseCircle(HDC hdc,int x,int y,int xc,int yc,int R,COLORREF c)
{

    double d=(x-xc)*(x-xc)+(y-yc)*(y-yc)-R*R;
    if(d<0)
    {
        SetPixel(hdc,x,y,c);
    }
}
void clippingLineuseCircle(HDC hdc,int x1,int y1,int x2,int y2,int xc,int yc,int R)
{
    double dx=x2-x1;
    double dy=y2-y1;
    for(double t=0;t<1;t+=0.001)
    {
        int x=x1+(t*dx);
        int y=y1+(t*dy);
        double d=(x-xc)*(x-xc)+(y-yc)*(y-yc)-R*R;
        if(d<0)
        {
            SetPixel(hdc,x,y,RGB(0,0,0));
        }
    }
}
void NRFloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf)
{
    stack<Vertex> S;
    S.push(Vertex(x,y));
    while(!S.empty())
        {
            Vertex v=S.top();
            S.pop();
            COLORREF c=GetPixel(hdc,v.x,v.y);
            if(c==Cb || c==Cf)continue;
            SetPixel(hdc,v.x,v.y,Cf);
            S.push(Vertex(v.x+1,v.y));
            S.push(Vertex(v.x-1,v.y));
            S.push(Vertex(v.x,v.y+1));
            S.push(Vertex(v.x,v.y-1));
        }
}
void FloodFill(HDC hdc,int x,int y,COLORREF Cb,COLORREF Cf)
{
    COLORREF C=GetPixel(hdc,x,y);
    if(C==Cb || C==Cf)return;
    SetPixel(hdc,x,y,Cf);
    FloodFill(hdc,x+1,y,Cb,Cf);
    FloodFill(hdc,x-1,y,Cb,Cf);
    FloodFill(hdc,x,y+1,Cb,Cf);
    FloodFill(hdc,x,y-1,Cb,Cf);
}
void InitEntries(Entry table[])
{
    for(int i=0;i<MAXENTRIES;i++)
    {
        table[i].xmin=std::numeric_limits<int>::max();
        table[i].xmax=-std::numeric_limits<int>::max();
    }
}
void ScanEdge(Vertex v1,Vertex v2,Entry table[])
{
    if(v1.y==v2.y)return;
    if(v1.y>v2.y)swap(v1,v2);
    double minv=(double)(v2.x-v1.x)/(v2.y-v1.y);
    double x=v1.x;
    int y=v1.y;
    while(y<v2.y)
    {
        if(x<table[y].xmin)table[y].xmin=(int)ceil(x);
        if(x>table[y].xmax)table[y].xmax=(int)floor(x); y++;
        x+=minv;
    }
}
void DrawSanLines(HDC hdc,Entry table[],COLORREF color)
{
    for(int y=0;y<MAXENTRIES;y++)
        if(table[y].xmin<table[y].xmax)
            for(int x=table[y].xmin;x<=table[y].xmax;x++)
                SetPixel(hdc,x,y,color);
}
void ConvexFill(HDC hdc,vector<Vertex> v,int n,COLORREF color)
{
    Entry *table=new Entry[MAXENTRIES];
    InitEntries(table);
    Vertex v1=v[v.size()-1];
    for(int i=0;i<v.size();i++)
    {
        Vertex v2=v[i];
        ScanEdge(v1,v2,table);
        v1=v[i];
    }
    DrawSanLines(hdc,table,color);
    delete table;
}
EdgeRec InitEdgeRec(Vertex& v1,Vertex& v2)
{
    if(v1.y>v2.y)swap(v1,v2);
    EdgeRec rec;
    rec.x=v1.x;
    rec.ymax=v2.y;
    rec.minv=(double)(v2.x-v1.x)/(v2.y-v1.y);
    return rec;
}
void InitEdgeTable(vector<Vertex>v,int n,EdgeList table[])
{
    Vertex v1=v[v.size()-1];
    for(int i=0;i<v.size();i++)
    {
        Vertex v2=v[i];
        if(v1.y==v2.y){ v1=v2; continue; }
        EdgeRec rec=InitEdgeRec(v1, v2);
        table[int(v1.y)].push_back(rec);
        v1=v[i];
    }
}
void GeneralPolygonFill(HDC hdc,vector<Vertex> v,int n,COLORREF c)
{
    EdgeList *table=new EdgeList [MAXENTRIES];
    InitEdgeTable(v,n,table);
    int y=0;
    while(y<MAXENTRIES && table[y].size()==0)
        y++;
    if(y==MAXENTRIES)return;
    EdgeList ActiveList=table[y];
    while (ActiveList.size()>0)
    {
        ActiveList.sort();
        for(EdgeList::iterator it=ActiveList.begin();it!=ActiveList.end();it++)
        {
            int x1=(int)ceil(it->x);
            it++;
            int x2=(int)floor(it->x);
            for(int x=x1;x<=x2;x++)SetPixel(hdc,x,y,c);
        }
        y++;
        EdgeList::iterator it=ActiveList.begin();
        while(it!=ActiveList.end())
        if(y==it->ymax)
            it=ActiveList.erase(it);
        else
            it++;
        for(EdgeList::iterator it=ActiveList.begin();it!=ActiveList.end();it++)
            it->x+=it->minv;
        ActiveList.insert(ActiveList.end(),table[y].begin(),table[y].end());
    }
    delete[] table;
}
void Draw2point1(HDC hdc,int xc,int yc,int x,int y,COLORREF c)
{
    SetPixel(hdc,xc+x,yc+y,c);
    SetPixel(hdc,xc+y,yc+x,c);
}
void Draw2point2(HDC hdc,int xc,int yc,int x,int y,COLORREF c)
{
    SetPixel(hdc,xc-x,yc+y,c);
    SetPixel(hdc,xc-y,yc+x,c);
}
void Draw2point3(HDC hdc,int xc,int yc,int x,int y,COLORREF c)
{
    SetPixel(hdc,xc-x,yc-y,c);
    SetPixel(hdc,xc-y,yc-x,c);
}
void Draw2point4(HDC hdc,int xc,int yc,int x,int y,COLORREF c)
{
    SetPixel(hdc,xc+x,yc-y,c);
    SetPixel(hdc,xc+y,yc-x,c);
}
void fillQuarterofcircleuseathorline(HDC hdc,double xc,double yc,int R,int num,COLORREF c)
{
    int y=0;
    int x=R;
    for(double theta=0;theta<2*3.14;theta+=0.03){
        x =xc+R*cos(theta);
        y =yc+R*sin(theta);
        if(num==1&& theta>(3.14+.5*3.14)&&theta<2*3.14)
        {//fourth quartar
            parametricLine( hdc, xc,yc, x, y,c);

        }
        if(num==2&&theta>3.14&&theta<(3.14+.5*3.14))
        {//second quartar
            parametricLine( hdc, xc, yc, x, y,c);

        }
        if(num==3&&theta>=.5*3.14&&theta<3.14){//Third quartar
            parametricLine( hdc,xc,yc, x, y,c);

       }
        if(num==4&&theta>0&&theta<=.5*3.14){//fourth quartar
         parametricLine( hdc, xc, yc, x, y,c);
       }
}
}
void fillQuarterofcircleuseathorcircle(HDC hdc,int xc,int yc, int R,int count,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    if(count==1)
    {
        while(x<y)
        {
            if(d<0)
                d+=2*x+2;
            else
            {
                d+=2*(x-y)+5;
                y--;
            }
            x++;
            Draw2point4(hdc,xc,yc,x,y,color);
        }
    }
    else if(count==2)
    {
        Draw2point3(hdc,xc,yc,x,y,color);
        while(x<y)
        {
            if(d<0)
                d+=2*x+2;
            else
            {
                d+=2*(x-y)+5;
                y--;
            }
            x++;
            Draw2point3(hdc,xc,yc,x,y,color);
        }
    }
    else if(count==3)
    {
        Draw2point2(hdc,xc,yc,x,y,color);
        while(x<y)
        {
            if(d<0)
                d+=2*x+2;
            else
            {
                d+=2*(x-y)+5;
                y--;
            }
            x++;
            Draw2point2(hdc,xc,yc,x,y,color);
        }
    }
    else if(count==4)
    {
        Draw2point1(hdc,xc,yc,x,y,color);
        while(x<y)
        {
            if(d<0)
                d+=2*x+2;
            else
            {
                d+=2*(x-y)+5;
                y--;
            }
            x++;
            Draw2point1(hdc,xc,yc,x,y,color);
        }
    }
}
void DrawHermiteCurve(HDC hdc,Vertex p1,Vertex T1,Vertex p2,Vertex T2,COLORREF c)
{
    double a0=p1.x;
    double a1=T1.x;
    double a2=-3*p1.x-2*T1.x+3*p2.x-T2.x;
    double a3=2*p1.x+T1.x -2*p2.x+T2.x;
    double b0=p1.y;
    double b1=T1.y;
    double b2=-3*p1.y-2*T1.y+3*p2.y-T2.y;
    double b3=2*p1.y+T1.y -2*p2.y+T2.y;
    for(double i=0;i<1;i=i+0.001)
    {
        double x=a0+a1*i+a2*i*i+a3*i*i*i;
        double y=b0+b1*i+b2*i*i+b3*i*i*i;
        SetPixel(hdc,x,y,c);
    }
}
void DrawCardinalSpline(HDC hdc,Vertex P[],int n,double c,COLORREF color)
{
    double c1=1-c;
    Vertex T0(c1*(P[2].x-P[0].x),c1*(P[1].y-P[0].y));
    for(int i=2;i<n-1;i++)
    {
        Vertex T1(c1*(P[i+1].x-P[i-1].x),c1*(P[i+1].y-P[i-1].y));
        DrawHermiteCurve(hdc,P[i-1],T0,P[i],T1,color);
        T0=T1;
    }
}
void DrawBezierCurve(HDC hdc,Vertex P0,Vertex P3,COLORREF c)
{
    Vertex T0(0,0);
    Vertex T1(0,0);
    DrawHermiteCurve(hdc,P0,T0,P3,T1,c);
}
void loaddata(HDC hdc,HWND hwnd)
{
    static double R;
    if(!DDA1.empty())
    {
        for(int i=0;i<DDA1.size()-1;i=i+2)
        {
             hdc=GetDC(hwnd);
             DrawDDALine(hdc,DDA1[i].x,DDA1[i].y,DDA1[i+1].x,DDA1[i+1].y,colorDDA1[i]);
             ReleaseDC(hwnd,hdc);
        }
    }
    if(!MidPoint1.empty())
    {
        for(int i=0;i<MidPoint1.size()-1;i=i+2)
        {
             hdc=GetDC(hwnd);
             DrawMidpointLine(hdc,MidPoint1[i].x,MidPoint1[i].y,MidPoint1[i+1].x,MidPoint1[i+1].y,colorMidPoint1[i]);
             ReleaseDC(hwnd,hdc);
        }
    }
    if(!Prametric1.empty())
    {
        for(int i=0;i<Prametric1.size()-1;i=i+2)
        {
             hdc=GetDC(hwnd);
             parametricLine(hdc,Prametric1[i].x,Prametric1[i].y,Prametric1[i+1].x,Prametric1[i+1].y,colorPrametric1[i]);
             ReleaseDC(hwnd,hdc);
        }
    }
    if(!CircleDirect1.empty())
    {
        for(int i=0;i<CircleDirect1.size()-1;i=i+2)
        {
                R=sqrt(pow((CircleDirect1[i].x-CircleDirect1[i+1].x),2)+pow((CircleDirect1[i].y-CircleDirect1[i+1].y),2));
                hdc=GetDC(hwnd);
                circleDirect(hdc,CircleDirect1[i].x,CircleDirect1[i].y,R,colorCircleDirect1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!CirclePolar1.empty())
    {
        for(int i=0;i<CirclePolar1.size()-1;i=i+2)
        {
                R=sqrt(pow((CirclePolar1[i].x-CirclePolar1[i+1].x),2)+pow((CirclePolar1[i].y-CirclePolar1[i+1].y),2));
                hdc=GetDC(hwnd);
                circlePolar(hdc,CirclePolar1[i].x,CirclePolar1[i].y,R,colorCirclePolar1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!CircleIterativePolar1.empty())
    {
        for(int i=0;i<CircleIterativePolar1.size()-1;i=i+2)
        {
                R=sqrt(pow((CircleIterativePolar1[i].x-CircleIterativePolar1[i+1].x),2)+pow((CircleIterativePolar1[i].y-CircleIterativePolar1[i+1].y),2));
                hdc=GetDC(hwnd);
                circleIterativePolar(hdc,CircleIterativePolar1[i].x,CircleIterativePolar1[i].y,R,colorCircleIterativePolar1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!CircleBresenham1.empty())
    {
        for(int i=0;i<CircleBresenham1.size()-1;i=i+2)
        {
                R=sqrt(pow((CircleBresenham1[i].x-CircleBresenham1[i+1].x),2)+pow((CircleBresenham1[i].y-CircleBresenham1[i+1].y),2));
                hdc=GetDC(hwnd);
                circleBresenham(hdc,CircleBresenham1[i].x,CircleBresenham1[i].y,R,colorCircleBresenham1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!CircleFasterBresenham1.empty())
    {
        for(int i=0;i<CircleFasterBresenham1.size()-1;i=i+2)
        {
                R=sqrt(pow((CircleFasterBresenham1[i].x-CircleFasterBresenham1[i+1].x),2)+pow((CircleFasterBresenham1[i].y-CircleFasterBresenham1[i+1].y),2));
                hdc=GetDC(hwnd);
                circleFasterBresenham(hdc,CircleFasterBresenham1[i].x,CircleFasterBresenham1[i].y,R,colorCircleFasterBresenham1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!EllipseDirect1.empty())
    {
        for(int i=0;i<EllipseDirect1.size();i++)
        {
                hdc=GetDC(hwnd);
                ellipseDirect(hdc,EllipseDirect1[i].x,EllipseDirect1[i].y,A,B,colorEllipseDirect1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!EllipsePolar1.empty())
    {
        for(int i=0;i<EllipsePolar1.size();i++)
        {
                hdc=GetDC(hwnd);
                ellipsePolar(hdc,EllipsePolar1[i].x,EllipsePolar1[i].y,A,B,colorEllipsePolar1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!EllipseBresenham1.empty())
    {
        for(int i=0;i<EllipseBresenham1.size();i++)
        {
                hdc=GetDC(hwnd);
                ellipseBresenham(hdc,EllipseBresenham1[i].x,EllipseBresenham1[i].y,A,B,colorEllipseBresenham1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!DrawRectangle1.empty())
    {
        for(int i=0;i<DrawRectangle1.size();i+=2)
        {
                Vertex p2=Vertex(DrawRectangle1[i+1].x,DrawRectangle1[i].y);
                Vertex p4=Vertex(DrawRectangle1[i].x,DrawRectangle1[i+1].y);
                hdc=GetDC(hwnd);
                DrawRectangleandSquare(hdc,DrawRectangle1[i],p2,DrawRectangle1[i+1],p4,colorDrawRectangle1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!DrawSquare1.empty())
    {
        for(int i=0;i<DrawSquare1.size();i+=4)
        {
                hdc=GetDC(hwnd);
                DrawRectangleandSquare(hdc,DrawSquare1[i],DrawSquare1[i+1],DrawSquare1[i+2],DrawSquare1[i+3],colorDrawSquare1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!polygonforconvex1.empty())
    {
                hdc=GetDC(hwnd);
                DrawPolygon(hdc,polygonforconvex1,y,colorpolygonforconvex1[colorpolygonforconvex1.size()-1]);
                ReleaseDC(hwnd,hdc);
    }
    if(!polygonfornonconvex1.empty())
    {
                hdc=GetDC(hwnd);
                DrawPolygon(hdc,polygonfornonconvex1,y,colorpolygonfornonconvex1[colorpolygonfornonconvex1.size()-1]);
                ReleaseDC(hwnd,hdc);
    }
    if(!DrawCircle1.empty())
    {
        for(int i=0;i<DrawCircle1.size();i+=2)
        {
                R=sqrt(pow((DrawCircle1[i].x-DrawCircle1[i+1].x),2)+pow((DrawCircle1[i].y-DrawCircle1[i+1].y),2));
                hdc=GetDC(hwnd);
                circleDirect(hdc,DrawCircle1[i].x,DrawCircle1[i].y,R,colorDrawCircle1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!fillRcursev1.empty())
    {
        for(int i=0;i<fillRcursev1.size();i+=1)
        {
                hdc=GetDC(hwnd);
                FloodFill(hdc,fillRcursev1[i].x,fillRcursev1[i].y,colorfillRcursev1[i],colorfillRcursev1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    if(!fillnonRcursev1.empty())
    {
        for(int i=0;i<fillnonRcursev1.size();i+=1)
        {
                hdc=GetDC(hwnd);
                FloodFill(hdc,fillnonRcursev1[i].x,fillnonRcursev1[i].y,colorfillnonRcursev1[i],colorfillnonRcursev1[i]);
                ReleaseDC(hwnd,hdc);
        }
    }
    for(int i=0;i<fillQuruseline1.size();i+=2)
    {

            R=sqrt(pow((fillQuruseline1[i].x-fillQuruseline1[i+1].x),2)+pow((fillQuruseline1[i].y-fillQuruseline1[i+1].y),2));
            hdc=GetDC(hwnd);
            fillQuarterofcircleuseathorline(hdc,fillQuruseline1[i].x,fillQuruseline1[i].y,R,1,colorfillQuruseline1[i]);
            ReleaseDC(hwnd,hdc);
    }
    for(int i=0;i<fillQuruseline2.size();i+=2)
    {

            R=sqrt(pow((fillQuruseline2[i].x-fillQuruseline2[i+1].x),2)+pow((fillQuruseline2[i].y-fillQuruseline2[i+1].y),2));
            hdc=GetDC(hwnd);
            fillQuarterofcircleuseathorline(hdc,fillQuruseline2[i].x,fillQuruseline2[i].y,R,2,colorfillQuruseline2[i]);
            ReleaseDC(hwnd,hdc);
    }
    for(int i=0;i<fillQuruseline3.size();i+=2)
    {

            R=sqrt(pow((fillQuruseline3[i].x-fillQuruseline3[i+1].x),2)+pow((fillQuruseline3[i].y-fillQuruseline3[i+1].y),2));
            hdc=GetDC(hwnd);
            fillQuarterofcircleuseathorline(hdc,fillQuruseline3[i].x,fillQuruseline3[i].y,R,3,colorfillQuruseline3[i]);
            ReleaseDC(hwnd,hdc);
    }
    for(int i=0;i<fillQuruseline4.size();i+=2)
    {
            R=sqrt(pow((fillQuruseline4[i].x-fillQuruseline4[i+1].x),2)+pow((fillQuruseline4[i].y-fillQuruseline4[i+1].y),2));
            hdc=GetDC(hwnd);
            fillQuarterofcircleuseathorline(hdc,fillQuruseline4[i].x,fillQuruseline4[i].y,R,4,colorfillQuruseline4[i]);
            ReleaseDC(hwnd,hdc);
    }

    for(int i=0;i<fillQuruseCircle1.size();i+=2)
    {

            R=sqrt(pow((fillQuruseCircle1[i].x-fillQuruseCircle1[i+1].x),2)+pow((fillQuruseCircle1[i].y-fillQuruseCircle1[i+1].y),2));
            for(double j=0;j<R;j+=1)
            {
                hdc=GetDC(hwnd);
                fillQuarterofcircleuseathorcircle(hdc,fillQuruseCircle1[i].x,fillQuruseCircle1[i].y,j,1,colorfillQuruseCircle1[i]);
                ReleaseDC(hwnd,hdc);
            }
    }
    for(int i=0;i<fillQuruseCircle2.size();i+=2)
    {

            R=sqrt(pow((fillQuruseCircle2[i].x-fillQuruseCircle2[i+1].x),2)+pow((fillQuruseCircle2[i].y-fillQuruseCircle2[i+1].y),2));
            for(double j=0;j<R;j+=1)
            {
                hdc=GetDC(hwnd);
                fillQuarterofcircleuseathorcircle(hdc,fillQuruseCircle2[i].x,fillQuruseCircle2[i].y,j,2,colorfillQuruseCircle2[i]);
                ReleaseDC(hwnd,hdc);
            }
    }
    for(int i=0;i<fillQuruseCircle3.size();i+=2)
    {
            R=sqrt(pow((fillQuruseCircle3[i].x-fillQuruseCircle3[i+1].x),2)+pow((fillQuruseCircle3[i].y-fillQuruseCircle3[i+1].y),2));
            for(double j=0;j<R;j+=1)
            {
                hdc=GetDC(hwnd);
                fillQuarterofcircleuseathorcircle(hdc,fillQuruseCircle3[i].x,fillQuruseCircle3[i].y,j,3,colorfillQuruseCircle3[i]);
                ReleaseDC(hwnd,hdc);
            }
    }
    for(int i=0;i<fillQuruseCircle4.size();i+=2)
    {

            R=sqrt(pow((fillQuruseCircle4[i].x-fillQuruseCircle4[i+1].x),2)+pow((fillQuruseCircle4[i].y-fillQuruseCircle4[i+1].y),2));
            for(double j=0;j<R;j+=1)
            {
                hdc=GetDC(hwnd);
                fillQuarterofcircleuseathorcircle(hdc,fillQuruseCircle4[i].x,fillQuruseCircle4[i].y,j,4,colorfillQuruseCircle4[i]);
                ReleaseDC(hwnd,hdc);
            }
    }
    if(index10==1)
    {
        hdc=GetDC(hwnd);
        DrawCardinalSpline(hdc,CardinalSplineCurve1,n,0.5,colorCardinalSplineCurve1[colorCardinalSplineCurve1.size()-1]);
        ReleaseDC(hwnd,hdc);
    }
    if(index8==1)
    {
        hdc=GetDC(hwnd);
        ConvexFill(hdc,polygonforconvex1,y,colorpolygonforconvex1[colorpolygonforconvex1.size()-1]);
        ReleaseDC(hwnd,hdc);
    }
    if(index9==1)
    {
        hdc=GetDC(hwnd);
        GeneralPolygonFill(hdc,polygonfornonconvex1,y,colorpolygonfornonconvex1[colorpolygonfornonconvex1.size()-1]);
        ReleaseDC(hwnd,hdc);
    }
    if(index1==1)
    {
        for(int i=0;i<DrawPoint1.size();i++)
        {
            hdc=GetDC(hwnd);
            PointClippinguseRectagleWindow(hdc,DrawPoint1[i].x,DrawPoint1[i].y,clipRectanglepoint1[0].x,clipRectanglepoint1[0].y,clipRectanglepoint1[1].x,clipRectanglepoint1[1].y,colorDrawPoint1[i]);
            ReleaseDC(hwnd,hdc);
        }
    }
    if(index2==1)
    {
        for(int i=0;i<DrawLine1.size();i+=2)
        {
            hdc=GetDC(hwnd);
            CohenSuth(hdc,DrawLine1[i].x,DrawLine1[i].y,DrawLine1[i+1].x,DrawLine1[i+1].y,clipRectangleLine1[0].x,clipRectangleLine1[0].y,clipRectangleLine1[1].x,clipRectangleLine1[1].y);
            ReleaseDC(hwnd,hdc);
        }
    }
    if(index3==1)
    {
        hdc=GetDC(hwnd);
        PolygonClip(hdc,polygonforclipping1,y,clipRectanglepolygon1[0].x,clipRectanglepolygon1[0].y,clipRectanglepolygon1[1].x,clipRectanglepolygon1[1].y);
        ReleaseDC(hwnd,hdc);
    }
    if(index4==1)
    {
        for(int i=0;i<DrawPoint1.size();i++)
        {
            hdc=GetDC(hwnd);
            PointClippinguseRectagleWindow(hdc,DrawPoint1[i].x,DrawPoint1[i].y,clipSquarepoint1[0].x,clipSquarepoint1[0].y,clipSquarepoint1[1].x,clipSquarepoint1[1].y,colorDrawPoint1[i]);
            ReleaseDC(hwnd,hdc);
        }
    }
    if(index5==1)
    {
        for(int i=0;i<DrawLine1.size();i+=2)
        {
            hdc=GetDC(hwnd);
            CohenSuth(hdc,DrawLine1[i].x,DrawLine1[i].y,DrawLine1[i+1].x,DrawLine1[i+1].y,clipSquareLine1[0].x,clipSquareLine1[0].y,clipSquareLine1[1].x,clipSquareLine1[1].y);
            ReleaseDC(hwnd,hdc);
        }
    }
    if(index6==1)
    {
        for(int i=0;i<DrawPoint1.size();i++)
        {
            R=sqrt(pow((clipCirclepoint1[0].x-clipCirclepoint1[1].x),2)+pow((clipCirclepoint1[0].y-clipCirclepoint1[1].y),2));
            hdc=GetDC(hwnd);
            clippinpointguseCircle(hdc,DrawPoint1[i].x,DrawPoint1[i].y,clipCirclepoint1[0].x,clipCirclepoint1[0].y,R,colorDrawPoint1[i]);
            ReleaseDC(hwnd,hdc);
        }
    }
    if(index7==1)
    {
        for(int i=0;i<DrawLine1.size();i+=2)
        {
            R=sqrt(pow((clipCircleLine1[0].x-clipCircleLine1[1].x),2)+pow((clipCircleLine1[0].y-clipCircleLine1[1].y),2));
            hdc=GetDC(hwnd);
            clippingLineuseCircle(hdc,DrawLine1[i].x,DrawLine1[i].y,DrawLine1[i+1].x,DrawLine1[i+1].y,clipCircleLine1[0].x,clipCircleLine1[0].y,R);
            ReleaseDC(hwnd,hdc);
        }
    }
}
