
// ��������С����.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


#define WM_SHOW_MESSAGE_BOX WM_USER+1//��ʾmessagebox��Ϣ
#define WM_NET_EDIT_LOGOUT WM_USER+2//��ʾ��־��Ϣ
#define WM_NET_THREAD_ACTION WM_USER+3//�̻߳ش�״̬��Ϣ


// C��������С����App: 
// �йش����ʵ�֣������ ��������С����.cpp
//

class C��������С����App : public CWinApp
{
public:
	C��������С����App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C��������С����App theApp;


//���ڰ󶨸�����
class CWndAttache
{
private:
	vector<reference_wrapper<CWnd>> vecCWnd;//���ڶ���
public:
	CWndAttache()
		= default;
	CWndAttache(const CWndAttache &)
		= delete;
	CWndAttache &operator =(const CWndAttache &)
		= delete;
	//�������
	~CWndAttache()
	{
		for(auto &r : vecCWnd) {
			r.get().Detach();
		}
	}
	//���а�
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


//�ַ�����CStringת��
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


//�ַ���ת����CString
inline LONG_PTR NewCstrLong(const string &str)
{
	CString cstr;
	StrToCstr(cstr, str);
	auto p = new CString(cstr);
	return reinterpret_cast<LONG_PTR>(p);
}
