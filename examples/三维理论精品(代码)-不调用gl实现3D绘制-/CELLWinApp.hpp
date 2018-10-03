#pragma once

#include <Windows.h>
#include <tchar.h>

#include "CELLMath.hpp"
#include "CELL3RDCamera.hpp"

namespace   CELL
{
    class   Role
    {
    public:
        float3  _pos;
        float3  _target;
        float   _speed;
    public:
        Role()
        {
            _speed  =   5;
        }
        /**
        *   设置移动的目标点
        */
        void    setTarget(float3 target)
        {
            _target =   target;
        }
        /**
        *   更新位置
        */
        void    setPosition(float3 pos)
        {
            _pos    =   pos;
            _pos.y  =   1;
        }

        void moveCheck(const float elasped)
        {
            /**
            *   目标位置不是当前位置。
            */
            if (_target == _pos)
            {
                return;
            }
            /**
            *   获取当前玩家位置与目标位置的偏移量
            */
            float3  offset  =   _target - _pos;
            /**
            *   获取到移动的方向
            */
            float3  dir     =   normalize(offset);
            
            if (distance(_target,_pos) > 1)
            {
                float   speed   =   elasped * _speed;

                _pos    +=  float3(dir.x * speed,0,dir.z  * speed) ;
            }
            else
            {
                _target  = _pos;
            }
        }
        /**
        *   绘制角色
        */
        void    render(float fElapsed)
        {
            moveCheck(fElapsed);
        }
    };
    float3 g_cubeVertices[] =
    {
        float3(-1.0f,-1.0f, 1.0f ),
        float3( 1.0f,-1.0f, 1.0f ),
        float3( 1.0f, 1.0f, 1.0f ),
        float3(-1.0f, 1.0f, 1.0f ),

        float3(-1.0f,-1.0f,-1.0f ),
        float3(-1.0f, 1.0f,-1.0f ),
        float3( 1.0f, 1.0f,-1.0f ),
        float3( 1.0f,-1.0f,-1.0f ),

        float3(-1.0f, 1.0f,-1.0f ),
        float3(-1.0f, 1.0f, 1.0f ),
        float3( 1.0f, 1.0f, 1.0f ),
        float3( 1.0f, 1.0f,-1.0f ),

        float3(-1.0f,-1.0f,-1.0f ),
        float3( 1.0f,-1.0f,-1.0f ),
        float3( 1.0f,-1.0f, 1.0f ),
        float3(-1.0f,-1.0f, 1.0f ),

        float3( 1.0f,-1.0f,-1.0f ),
        float3( 1.0f, 1.0f,-1.0f ),
        float3( 1.0f, 1.0f, 1.0f ),
        float3( 1.0f,-1.0f, 1.0f ),

        float3(-1.0f,-1.0f,-1.0f ),
        float3(-1.0f,-1.0f, 1.0f ),
        float3(-1.0f, 1.0f, 1.0f ),
        float3(-1.0f, 1.0f,-1.0f )
    };

    class   Soft3d :public CELL3RDCamera
    {
        std::vector<float3> _arGround;
    public:
        Soft3d()
        {
            float   x   =   0;
            float   z   =   0;

            float   w   =   50;
            float   h   =   50;
            float   y   =   0;
            for ( ; x <= w ; x += 10 )
            {
                _arGround.push_back(float3(x,y,0));
                _arGround.push_back(float3(x,y,h));
            }

            for (;z <= h ; z += 10 )
            {
                _arGround.push_back(float3(0,y,z));
                _arGround.push_back(float3(w,y,z));
            }

        }
        void    renderGround(HDC hDC)
        {
            for(size_t i = 0 ;i < _arGround.size() ; i+=2)
            {
                float2  screen0  =   worldToScreen(_arGround[i]);
                float2  screen1  =   worldToScreen(_arGround[i+1]);

                ::MoveToEx(hDC,int(screen0.x),int(screen0.y),0);

                ::LineTo(hDC,int(screen1.x),int(screen1.y));
                
            }
        }

        void    drawLine(HDC hDC,const float3* arData,int length)
        {
            for (int i = 0 ;i < length ; ++ i)
            {
                float3  pos     =   arData[i];
                float2  screen  =   worldToScreen(pos);
                

                if (i %4 == 0)
                {
                    ::MoveToEx(hDC,int(screen.x),int(screen.y),0);
                }
                else
                {
                    ::LineTo(hDC,int(screen.x),int(screen.y));
                }
            }
            
        }
        /**
        *   绘制函数
        */
        void    render(float elapsed,HDC hDC)
        {
            //! 绘制地面经纬网络
            _matWorld.identify();
            renderGround(hDC);
            //! 绘制控制者
            static  float   angle   =   0;
            quatr   quat   =   CELL::angleAxis(angle,float3(0,0,1));
            angle   +=  0.1f;
            _matWorld  =   CELL::makeTransform(_target,float3(1,1,1),quat);
            

            drawLine(hDC,g_cubeVertices,sizeof(g_cubeVertices)/sizeof(g_cubeVertices[0]));
            
        }
    };
    class   CELLWinApp
    {
    public:
        //! 实例句柄
        HINSTANCE   _hInstance;
        //! 窗口句柄
        HWND        _hWnd;
        //! 窗口的高度
        int         _width;
        //! 窗口的宽度
        int         _height;
        //! 内存DC
        HDC         _hMemDC;
        //! 内存位图
        HBITMAP     _hBmp;

        Soft3d          _device;
    
        Role            _role;
        float2          _rightDown;
        bool            _bRightFlg;
    public:
        CELLWinApp(HINSTANCE hInstance)
            :_hInstance(hInstance)
        {
            WNDCLASSEX  winClass;
            winClass.lpszClassName  =   _T("CELLWinApp");
            winClass.cbSize         =   sizeof(winClass);
            winClass.style          =   CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            winClass.lpfnWndProc    =   wndProc;
            winClass.hInstance      =   hInstance;
            winClass.hIcon	        =   0;
            winClass.hIconSm	    =   0;
            winClass.hCursor        =   LoadCursor(hInstance, IDC_ARROW);
            winClass.hbrBackground  =   (HBRUSH)GetStockObject(BLACK_BRUSH);
            winClass.lpszMenuName   =   NULL;
            winClass.cbClsExtra     =   0;
            winClass.cbWndExtra     =   0;
            RegisterClassEx(&winClass);
            _hBmp                   =   0;
            _hMemDC                 =   0;

            _bRightFlg              =   false;
            _role.setPosition(float3(0,0,-10));
            _role.setTarget(float3(25,0,25));

            _device.setRadius(50);
            _device.setEye(float3(50,50,50));
            _device.setTarget(_role._pos);
            _device.calcDir();
            _device.setUp(float3(0,1,0));

        }

        virtual ~CELLWinApp()
        {
            UnregisterClass(_T("CELLWinApp"),_hInstance);
        }
    protected:
        static  LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            
            CELLWinApp*  pThis   =   (CELLWinApp*)GetWindowLong(hWnd,GWL_USERDATA);
            if (pThis)
            {
                return  pThis->onEvent(hWnd,msg,wParam,lParam);
            }
            if (WM_CREATE == msg)
            {
                CREATESTRUCT*   pCreate =   (CREATESTRUCT*)lParam;
                SetWindowLong(hWnd,GWL_USERDATA,(DWORD_PTR)pCreate->lpCreateParams);
            }
            return  DefWindowProc( hWnd, msg, wParam, lParam );
        }
    public:
        virtual LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            #ifndef GET_X_LPARAM
                #define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
            #endif

            #ifndef GET_Y_LPARAM
                #define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
            #endif

            #ifndef GET_WHEEL_DELTA_WPARAM
                #define GET_WHEEL_DELTA_WPARAM(wParam)          (int)((short)HIWORD(wParam))
            #endif

            switch (msg)
            {
            case WM_CLOSE:
            case WM_DESTROY:
                {
                    ::PostQuitMessage(0);
                }
                break;
            case WM_MOUSEMOVE:
                {
                    if (_bRightFlg)
                    {
                        int x   =   GET_X_LPARAM(lParam);
                        int y   =   GET_Y_LPARAM(lParam);

                        float2  offset  =   float2(x,y) - _rightDown;
                        _rightDown =   float2(x,y);
                        _device.rotateView(offset.x * 0.1f);
                        _device.update();   
                    }
                }
                break;
            case WM_LBUTTONDOWN:
                {
                    //! 鼠标点
                    int x   =   GET_X_LPARAM(lParam);
                    int y   =   GET_Y_LPARAM(lParam);
                    _device._matWorld.identify();
                    Ray ray =   _device.createRayFromScreen(x,y);
                    float3  dir     =   ray.getDirection();
                    float3  pos     =   ray.getOrigin();
                    float   tm      =   abs((pos.y) / dir.y);
                    float3  tp      =   ray.getPoint(tm);
                    _role.setTarget(tp);


                }
                break;
            case WM_LBUTTONUP:
                {
                    _bRightFlg  =   false;
                }
                break;
            case WM_RBUTTONDOWN:
                {
                    int x   =   GET_X_LPARAM(lParam);
                    int y   =   GET_Y_LPARAM(lParam);
                    _bRightFlg  =   true;
                    _rightDown  =   float2(x,y);
                }
                break;
            case WM_RBUTTONUP:
                {
                    _bRightFlg  =   false;
                }
                break;
            case WM_MOUSEWHEEL:
                {
                    int delta   =   GET_WHEEL_DELTA_WPARAM(wParam);
                    if (delta > 0)
                    {
                        _device.setRadius(_device.getRadius() * 1.2f);
                    }
                    else if (delta  < 0)
                    {
                        _device.setRadius(_device.getRadius() * 0.8f);
                    }
                }
                break;
            case WM_SIZE:
                {
                    if (::IsWindow(_hWnd))
                    {
                        if (_hBmp)
                        {
                            DeleteObject(_hBmp);
                        }
                        RECT    rect;
                        GetClientRect(_hWnd,&rect);
                        HDC     hDC =   GetDC(_hWnd);
                        int     w   =   rect.right - rect.left;
                        int     h   =   rect.bottom - rect.top;

                        _hBmp   =   CreateCompatibleBitmap(hDC,w,h);
                        SelectObject(_hMemDC,_hBmp);
                        ReleaseDC(_hWnd,hDC);

                        _width   =   w;
                        _height  =   h;

                        _device.perspective(45.0f, float(w) / float(h), 0.1f, 100000.0f);
                        _device.setViewSize(float(w),float(h));
                        _device.update();
                    }
                }
                break;
            default:
                return  DefWindowProc( hWnd, msg, wParam, lParam ); 
            }
            return  S_OK;
            
        }
        /**
        *   主函数
        */
        int     main(int width,int height)
        {
            _hWnd   =   CreateWindowEx( NULL,
                                        _T("CELLWinApp"),
                                        _T("CELLWinApp"),
                                        WS_OVERLAPPEDWINDOW,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        width,
                                        height, 
                                        NULL, 
                                        NULL,
                                        _hInstance, 
                                        this
                                        );

            if (_hWnd == 0)
            {
                return  -1;
            }

            ShowWindow(_hWnd,SW_SHOW);

            RECT    rect;
            GetClientRect(_hWnd,&rect);

            HDC     hDC =   GetDC(_hWnd);
            int     w   =   rect.right - rect.left;
            int     h   =   rect.bottom - rect.top;

            _hMemDC =   CreateCompatibleDC(hDC);
            _hBmp   =   CreateCompatibleBitmap(hDC,w,h);
            SelectObject(_hMemDC,_hBmp);

            HPEN    pen =   ::CreatePen(PS_SOLID,1,RGB(255,255,255));
            SelectObject(_hMemDC,pen);

            MSG     msg =   {0};
            while(msg.message != WM_QUIT)
            {
                if (msg.message == WM_DESTROY || 
                    msg.message == WM_CLOSE)
                {
                    break;
                }
                /**
                *   有消息，处理消息，无消息，则进行渲染绘制
                */
                if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
                { 
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                
                HDC     hDC =   GetDC(_hWnd);
                RECT    rect;
                GetClientRect(_hWnd,&rect);
                int     w   =   rect.right - rect.left;
                int     h   =   rect.bottom - rect.top;

                FillRect(_hMemDC,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH));
                {
                    float   w   =   float(rect.right - rect.left);
                    float   h   =   float(rect.bottom - rect.top);
                    _device.setViewSize(float(rect.right - rect.left),float(rect.bottom - rect.top));
                    _device.setViewSize(w,h);
                    _device.perspective(45.0f,w/h,0.1f,1000);

                    _role.moveCheck(0.026f);
                    _device.setTarget(_role._pos);
                    _device.update();
    
                    _device.render(0.1f,_hMemDC);
                }
                BitBlt(hDC,0,0,w,h,_hMemDC,0,0,SRCCOPY);
                ReleaseDC(_hWnd,hDC);
            }
            if (_hMemDC)
            {
                DeleteDC(_hMemDC);
            }
            if (_hBmp)
            {
                DeleteObject(_hBmp);
            }
            return  0;
        }
        
    };
}