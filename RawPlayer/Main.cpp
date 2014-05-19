


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#include "Main.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 库文件
#pragma comment(lib, "ShLWApi.lib")
#pragma comment(lib, "WinInet.lib")
#pragma comment(lib, "ComCtl32.lib")

#pragma comment(lib, "WinMM.lib")
#pragma comment(lib, "DDraw.lib")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 全局变量
HINSTANCE g_hInst = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 程序入口
INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PTSTR ptzCmdLine, INT iCmdShow)
{
	HWND hWnd;
	COPYDATASTRUCT cdData;

	// 不允许运行多个实例
	CreateMutex(NULL, FALSE, STR_AppName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		hWnd = FindWindow(STR_AppName, NULL);
		if (hWnd)
		{
			// 唤出窗口
			SendMessage(hWnd, WM_COMMAND, IDC_TrayIcon, WM_LBUTTONUP);

			if (ptzCmdLine && ptzCmdLine[0])
			{
				// 打开文件
				cdData.dwData = IDM_File_Open;
				cdData.cbData = _StrSize(ptzCmdLine);
				cdData.lpData = ptzCmdLine;
				SendMessage(hWnd, WM_COPYDATA, NULL, (LPARAM) &cdData);
			}

			return -1;
		}
	}

	// 获取当前实例句柄
	g_hInst = hInstance;

	// 初始化配置文件和语言文件
	CIni::SetIniName();
	CLanguage::Initialize();

	// 初始化转换库
	CRawImage::Initialize();

	// 显示闪屏窗口
	CAboutDlg::Splash(CIni::GetInt(INI_ShowSplash, FALSE));

	// 初始化 COM 库和公用控件库
	CoInitialize(NULL);
	InitCommonControls();

	// 初始化主窗口
	if (CMainWnd::Create())
	{
		// 关闭关于对话框，运行主窗口
		CMainWnd::Show(ptzCmdLine, iCmdShow);
	}

	// 释放 COM 库
	CoUninitialize();

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
