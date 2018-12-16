
// 捕包辅助小工具.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


#define WM_SHOW_MESSAGE_BOX WM_USER+1//显示messagebox消息
#define WM_NET_EDIT_LOGOUT WM_USER+2//显示日志信息
#define WM_NET_THREAD_ACTION WM_USER+3//线程回传状态信息


// C捕包辅助小工具App: 
// 有关此类的实现，请参阅 捕包辅助小工具.cpp
//

class C捕包辅助小工具App : public CWinApp
{
public:
	C捕包辅助小工具App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern C捕包辅助小工具App theApp;


//窗口绑定辅助类
class CWndAttache
{
private:
	vector<reference_wrapper<CWnd>> vecCWnd;//窗口队列
public:
	CWndAttache()
		= default;
	CWndAttache(const CWndAttache &)
		= delete;
	CWndAttache &operator =(const CWndAttache &)
		= delete;
	//析构解绑定
	~CWndAttache()
	{
		for(auto &r : vecCWnd) {
			r.get().Detach();
		}
	}
	//进行绑定
	void Attache(CWnd &wnd, HWND hwnd)
	{
		vecCWnd.push_back(std::ref(wnd));
		wnd.Attach(hwnd);
	}
	void Attache(CWnd &wnd, CWnd *pParent, int id)
	{
		Attache(wnd, pParent->GetDlgItem(id)->m_hWnd);
	}
};


//字符串和CString转换
inline bool CstrToStr(string &str, const CString &cstr)
{
	wstring wstr;
	for(int i=0; i<cstr.GetLength(); ++i)
		wstr += cstr[i];
	bool res = WstrToStr(str, wstr);
	return res;
}
inline bool StrToCstr(CString &cstr, const string &str)
{
	wstring wstr;
	bool res = StrToWstr(wstr, str);
	if(!res)
		return false;
	cstr.Format(L"%ws", wstr.c_str());
	return true;
}


//字符串转化新CString
inline LONG_PTR NewCstrLong(const string &str)
{
	CString cstr;
	StrToCstr(cstr, str);
	auto p = new CString(cstr);
	return reinterpret_cast<LONG_PTR>(p);
}
