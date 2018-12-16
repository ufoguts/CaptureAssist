
// 捕包辅助小工具Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Common.h"
#include "捕包辅助小工具.h"
#include "捕包辅助小工具Dlg.h"
#include "afxdialogex.h"



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C捕包辅助小工具Dlg 对话框



C捕包辅助小工具Dlg::C捕包辅助小工具Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY_DIALOG, pParent),
	thdNet(&NetData::ThdFunc, &netData)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

C捕包辅助小工具Dlg::~C捕包辅助小工具Dlg()
{
	//退出网络线程
	netData.Exit();
	thdNet.join();

	CDialog::~CDialog();
}

void C捕包辅助小工具Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C捕包辅助小工具Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_UPDATE(IDC_EDIT_HEX, &C捕包辅助小工具Dlg::OnUpdateEditHex)
	ON_EN_UPDATE(IDC_EDIT_IP, &C捕包辅助小工具Dlg::OnUpdateEditIp)
	ON_EN_CHANGE(IDC_EDIT_BEFORE_REMOVE, &C捕包辅助小工具Dlg::OnChangeEditBeforeRemove)
	ON_BN_CLICKED(IDOK, &C捕包辅助小工具Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_REMOVE, &C捕包辅助小工具Dlg::OnBnClickedButtonClearRemove)
	ON_BN_CLICKED(IDC_BUTTON_COPY_REMOVE, &C捕包辅助小工具Dlg::OnBnClickedButtonCopyRemove)
	ON_EN_CHANGE(IDC_EDIT_LINUX_TIME, &C捕包辅助小工具Dlg::OnEnChangeEditLinuxTime)
	ON_EN_CHANGE(IDC_EDIT_LOCAL_TIME, &C捕包辅助小工具Dlg::OnEnChangeEditLocalTime)
	ON_EN_CHANGE(IDC_EDIT_UTC, &C捕包辅助小工具Dlg::OnEnChangeEditUtc)
	ON_EN_UPDATE(IDC_EDIT_LINUX_TIME, &C捕包辅助小工具Dlg::OnEnUpdateEditLinuxTime)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN, &C捕包辅助小工具Dlg::OnBnClickedButtonListen)
	ON_MESSAGE(WM_SHOW_MESSAGE_BOX, &C捕包辅助小工具Dlg::OnShowMessageBox)
	ON_MESSAGE(WM_NET_EDIT_LOGOUT, &C捕包辅助小工具Dlg::OnNetEditLogout)
	ON_MESSAGE(WM_NET_THREAD_ACTION, &C捕包辅助小工具Dlg::OnNetThreadAction)
	ON_BN_CLICKED(IDC_BUTTON_TCP_CONNECT, &C捕包辅助小工具Dlg::OnBnClickedButtonTcpConnect)
	ON_BN_CLICKED(IDC_BUTTON_UDP_RECV, &C捕包辅助小工具Dlg::OnBnClickedButtonUdpRecv)
	ON_BN_CLICKED(IDC_BUTTON_ALL_SEND, &C捕包辅助小工具Dlg::OnBnClickedButtonAllSend)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN_ACCEPT, &C捕包辅助小工具Dlg::OnBnClickedButtonListenAccept)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SEND, &C捕包辅助小工具Dlg::OnBnClickedButtonClearSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RECV, &C捕包辅助小工具Dlg::OnBnClickedButtonClearRecv)
	ON_EN_UPDATE(IDC_EDIT_IPNUM, &C捕包辅助小工具Dlg::OnEnUpdateEditIpnum)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &C捕包辅助小工具Dlg::OnBnClickedButtonReverse)
END_MESSAGE_MAP()


// C捕包辅助小工具Dlg 消息处理程序

BOOL C捕包辅助小工具Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//绑定控件
	//ip转换
	attache.Attache(editHex, this, IDC_EDIT_HEX);
	attache.Attache(editIp, this, IDC_EDIT_IP);
	attache.Attache(editIpnum, this, IDC_EDIT_IPNUM);
	//去空格
	attache.Attache(editBeforeRemove, this, IDC_EDIT_BEFORE_REMOVE);
	attache.Attache(editAfterRemove, this, IDC_EDIT_AFTER_REMOVE);
	//时间转换
	attache.Attache(editLinuxTime, this, IDC_EDIT_LINUX_TIME);
	attache.Attache(editLocalTime, this, IDC_EDIT_LOCAL_TIME);
	attache.Attache(editUTC, this, IDC_EDIT_UTC);
	//网络发送接收
	attache.Attache(editRecvIp, this, IDC_EDIT_RECV_IP);
	attache.Attache(editRecvPort, this, IDC_EDIT_RECV_PORT);
	attache.Attache(editNetStatus, this, IDC_EDIT_NET_STATUS);
	attache.Attache(editListenPort, this, IDC_EDIT_LISTEN_PORT);
	attache.Attache(editListenStatus, this, IDC_EDIT_LISTEN_STATUS);
	attache.Attache(editSendText, this, IDC_EDIT_SEND_TEXT);
	attache.Attache(editRecvText, this, IDC_EDIT_RECV_TEXT);
	attache.Attache(buttonTcpConnect, this, IDC_BUTTON_TCP_CONNECT);
	attache.Attache(buttonUdpRecv, this, IDC_BUTTON_UDP_RECV);
	attache.Attache(buttonAllSend, this, IDC_BUTTON_ALL_SEND);
	attache.Attache(buttonListen, this, IDC_BUTTON_LISTEN);
	attache.Attache(buttonListenAccept, this, IDC_BUTTON_LISTEN_ACCEPT);

	//ip相关初始化
	bInnerIpChange = false;
	editIp.SetWindowTextW(L"192.168.0.1");
	//时间转换初始化
	bInnerTimeChange = false;
	editLinuxTime.SetWindowTextW(L"0");
	//网络io初始化
	NetStatusChange(NetStatus::none);
	netData.Init(m_hWnd);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C捕包辅助小工具Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C捕包辅助小工具Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C捕包辅助小工具Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//显示messagebox函数
afx_msg LRESULT C捕包辅助小工具Dlg::OnShowMessageBox(WPARAM wParam, LPARAM lParam)
{
	//要求二参数为堆空间CString
	auto pCstr = reinterpret_cast<CString *>(lParam);
	MessageBox(*pCstr);
	//一参数为1则不删除内存
	if(wParam!=1)
		delete pCstr;
	return 0;
}


//改变对话框关闭策略
void C捕包辅助小工具Dlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}
void C捕包辅助小工具Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


//改变ip编辑框辅助函数
void C捕包辅助小工具Dlg::UpdataIpEdit(unsigned long long num, bool bHex, bool bNum, bool bIp)
{
	CString cstrTmp;
	//更新hex框
	if(bHex) {
		ostringstream oss;
		oss <<std::hex <<std::uppercase <<std::setw(8) <<std::setfill('0') <<num;
		StrToCstr(cstrTmp, oss.str());
		editHex.SetWindowTextW(cstrTmp);
	}
	//更新num框
	if(bNum) {
		StrToCstr(cstrTmp, std::to_string(num));
		editIpnum.SetWindowTextW(cstrTmp);
	}
	//更新ip框
	if(bIp) {
		char sz[16];
		unsigned long ipAddr = htonl((unsigned long)num);
		inet_ntop(AF_INET, &ipAddr, sz, 16);
		StrToCstr(cstrTmp, sz);
		editIp.SetWindowTextW(cstrTmp);
	}
}


//hex栏改变前消息函数
void C捕包辅助小工具Dlg::OnUpdateEditHex()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(bInnerIpChange)
		return;
	bInnerIpChange = true;

	//获取字符串
	CString cstrOri, cstrTmp;
	editHex.GetWindowTextW(cstrOri);
	string strOri;
	CstrToStr(strOri, cstrOri);
	bool bForm = true;
	//转化数字
	if(strOri.size()>8) {
		strOri.resize(8);
		bForm = false;
	}
	unsigned long long num;
	try {
		num = std::stoull(strOri, nullptr, 16);
	}
	catch(...) {
		num = 0;
		bForm = false;
	}
	if(num>=(1ULL<<32)) {
		num = (1ULL<<32)-1;
		bForm = false;
	}
	//更新编辑框
	UpdataIpEdit(num, !bForm, true, true);

	bInnerIpChange = false;
}


//ipnum改变前消息函数
void C捕包辅助小工具Dlg::OnEnUpdateEditIpnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(bInnerIpChange)
		return;
	bInnerIpChange = true;

	//获取字符串
	CString cstrOri, cstrTmp;
	editIpnum.GetWindowTextW(cstrOri);
	string strOri;
	CstrToStr(strOri, cstrOri);
	//转化数字
	bool bForm = true;
	if(strOri.size()>10) {
		strOri.resize(10);
		bForm = false;
	}
	unsigned long long num;
	if(!FromString(strOri, num))
		bForm = false;
	if(num>=(1ULL<<32)) {
		num = (1ULL<<32)-1;
		bForm = false;
	}
	//更新编辑框
	UpdataIpEdit(num, true, !bForm, true);

	bInnerIpChange = false;
}


//ip栏改变前消息函数
void C捕包辅助小工具Dlg::OnUpdateEditIp()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(bInnerIpChange)
		return;
	bInnerIpChange = true;

	//获取字符串
	CString cstrOri, cstrTmp;
	editIp.GetWindowTextW(cstrOri);
	string strOri;
	CstrToStr(strOri, cstrOri);
	//转化数字
	bool bForm = true;
	unsigned long long num;
	if(strOri.size()>15) {
		strOri.resize(15);
		bForm = false;
	}
	unsigned long ipAddr = 0;
	inet_pton(AF_INET, strOri.c_str(), &ipAddr);
	num = ntohl(ipAddr);
	//更新ip框
	if(!bForm) {
		StrToCstr(cstrTmp, strOri);
		editIp.SetWindowTextW(cstrTmp);
	}
	//更新其他框
	UpdataIpEdit(num, true, true, false);

	bInnerIpChange = false;
}


//反序按钮按下
void C捕包辅助小工具Dlg::OnBnClickedButtonReverse()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bInnerIpChange)
		return;
	bInnerIpChange = true;

	//获取字符串
	CString cstrOri, cstrTmp;
	editIpnum.GetWindowTextW(cstrOri);
	string strOri;
	CstrToStr(strOri, cstrOri);
	//转化数字
	unsigned long long num;
	FromString(strOri, num);
	if(num>=(1ULL<<32)) {
		num = (1ULL<<32)-1;
	}
	//反序
	num = ntohl(static_cast<uint32_t>(num));
	//更新编辑框
	UpdataIpEdit(num, true, true, true);

	bInnerIpChange = false;
}


//beforeremove改变消息函数
void C捕包辅助小工具Dlg::OnChangeEditBeforeRemove()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strBefore, strAfter;
	editBeforeRemove.GetWindowTextW(strBefore);
	for(int i=0; i<strBefore.GetLength(); ++i) {
		//若非回车
		if(!(i+1<strBefore.GetLength() && strBefore[i]=='\r' && strBefore[i+1]=='\n'))
			strAfter += strBefore[i];
		//若为回车
		else {
			//若前面没东西或者为回车，不做复制
			if(i==0)
				;
			//若前面为回车，原样复制
			else if(i>=2 && strBefore[i-2]=='\r' && strBefore[i-1]=='\n')
				strAfter += "\r\n";
			//若前面为连接线，去掉此连接线
			else if(strBefore[i-1]=='-')
				strAfter.Delete(strAfter.GetLength()-1);
			//否则，加入空格
			else
				strAfter += ' ';
			++ i;
		}
	}
	editAfterRemove.SetWindowTextW(strAfter);
}


//清除去除回车编辑框
void C捕包辅助小工具Dlg::OnBnClickedButtonClearRemove()
{
	// TODO: 在此添加控件通知处理程序代码
	editBeforeRemove.SetWindowTextW(L"");
	OnChangeEditBeforeRemove();
}


//复制去除所有回车编辑框
void C捕包辅助小工具Dlg::OnBnClickedButtonCopyRemove()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text;
	editAfterRemove.GetWindowTextW(text);
	//打开剪贴板
	if(::OpenClipboard(m_hWnd)) {
		::EmptyClipboard();
		//获取全局内存块
		HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(wchar_t)*text.GetLength()+2);
		LPSTR pData = (LPSTR)::GlobalLock(hData);
		//传数据
		memcpy(pData, text.GetString(), sizeof(wchar_t)*text.GetLength()+2);
		::GlobalUnlock(hData);
		//全局内存块交给剪贴板
		::SetClipboardData(CF_UNICODETEXT, hData);
		::CloseClipboard();
	}
	else
		MessageBox(L"Clipboard Open Fail");
}


//LinuxTime更改前
void C捕包辅助小工具Dlg::OnEnUpdateEditLinuxTime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(bInnerTimeChange)
		return;
	bInnerTimeChange = true;
	
	//更改格式化时间
	CString cstrText;
	editLinuxTime.GetWindowTextW(cstrText);
	string strText(cstrText.GetBuffer(), cstrText.GetBuffer()+cstrText.GetLength());
	long long time;
	bool res = FromString(strText, time);
	if(time>=1LL<<32) {
		time = (1LL<<32)-1;
		res = false;
	}
	else if(time<0) {
		time = 0;
		res = false;
	}
	if(!res) {
		cstrText.Format(L"%lld", time);
		editLinuxTime.SetWindowTextW(cstrText);
	}
	
	bInnerTimeChange = false;
}


//LinuxTime更改
void C捕包辅助小工具Dlg::OnEnChangeEditLinuxTime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(bInnerTimeChange)
		return;
	bInnerTimeChange = true;

	//获取时间
	CString cstrText;
	editLinuxTime.GetWindowTextW(cstrText);
	string strText(cstrText.GetBuffer(), cstrText.GetBuffer()+cstrText.GetLength());
	time_t time;
	FromString(strText, time);
	//转换本地时间
	strText = TimeToStr(time);
	std::tm *pTmstu;
	char buf[256];
	pTmstu = std::localtime(&time);
	if(strftime(buf, sizeof(buf), "%F %T", pTmstu)) {
		CString cstrTime(buf);
		editLocalTime.SetWindowTextW(cstrTime);
	}
	//转换UTC
	pTmstu = std::gmtime(&time);
	if(strftime(buf, sizeof(buf), "%F %T", pTmstu)) {
		CString cstrTime(buf);
		editUTC.SetWindowTextW(cstrTime);
	}
	
	bInnerTimeChange = false;
}


//转换辅助函数
time_t C捕包辅助小工具Dlg::StrToTmstu(const string &str, std::tm &tmstu)
{
	string::const_iterator itSt, itEd;
	auto funcIsNum = [](char ch) {
		return ch>='0' && ch<='9'; };
	itEd = str.begin();
	bool bAble = true;
	//通用处理函数
	auto funcFindNext = [&str, &funcIsNum, &itSt, &itEd, &bAble](int &num, int offset) {
		if(!bAble)
			return;
		itSt = std::find_if(itEd, str.end(), funcIsNum);
		itEd = std::find_if_not(itSt, str.end(), funcIsNum);
		if(!FromString(itSt, itEd, num))
			bAble = false;
		num += offset;
	};
	//构造tmstu
	funcFindNext(tmstu.tm_year, -1900);
	funcFindNext(tmstu.tm_mon, -1);
	funcFindNext(tmstu.tm_mday, 0);
	funcFindNext(tmstu.tm_hour, 0);
	funcFindNext(tmstu.tm_min, 0);
	funcFindNext(tmstu.tm_sec, 0);
	tmstu.tm_wday = tmstu.tm_yday = 0;
	tmstu.tm_isdst = 0;
	time_t time = -1;
	if(bAble)
		time = std::mktime(&tmstu);
	if(time<0)
		time = 0;
	else if(time>1LL<<32)
		time = (1LL<<32)-1;
	return time;
}


//LocalTime更改
void C捕包辅助小工具Dlg::OnEnChangeEditLocalTime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(bInnerTimeChange)
		return;
	bInnerTimeChange = true;

	//获取时间
	CString cstrText;
	editLocalTime.GetWindowTextW(cstrText);
	string strText(cstrText.GetBuffer(), cstrText.GetBuffer()+cstrText.GetLength());
	std::tm tmstu;
	//转换到linux
	long long time = StrToTmstu(strText, tmstu);
	cstrText.Format(L"%lld", time);
	editLinuxTime.SetWindowTextW(cstrText);
	//转换到UTC
	std::tm *pTmstu = std::gmtime(&time);
	char buf[256];
	if(strftime(buf, sizeof(buf), "%F %T", pTmstu)) {
		CString cstrTime(buf);
		editUTC.SetWindowTextW(cstrTime);
	}
	
	bInnerTimeChange = false;
}


//UTC更改
void C捕包辅助小工具Dlg::OnEnChangeEditUtc()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if(bInnerTimeChange)
		return;
	bInnerTimeChange = true;
	
	bInnerTimeChange = false;
}


//显示日志信息
afx_msg LRESULT C捕包辅助小工具Dlg::OnNetEditLogout(WPARAM wParam, LPARAM lParam)
{
	//要求二参数为堆空间CString
	auto pCstr = reinterpret_cast<CString *>(lParam);
	CString cstr;
	//一参数为1则同时发送messagebox
	if(wParam==1) {
		OnShowMessageBox(1, lParam);
	}
	//加入日志信息
	editRecvText.GetWindowTextW(cstr);
	cstr += *pCstr;
	editRecvText.SetWindowTextW(cstr);
	editRecvText.LineScroll(editRecvText.GetLineCount());
	//删除内存
	delete pCstr;
	return 0;
}


//线程回传状态信息
afx_msg LRESULT C捕包辅助小工具Dlg::OnNetThreadAction(WPARAM wParam, LPARAM lParam)
{
	auto action = static_cast<NetData::Action>(lParam);
	//tcp监听情况下接收连接
	if(action==NetData::Action::tcpAccept) {
		if(netStatus==NetStatus::tcpListen) {
			editListenStatus.SetWindowTextW(L"双向连接");
			buttonListenAccept.EnableWindow(true);
			buttonAllSend.EnableWindow(true);
		}
	}
	//tcp监听状态下接受连接失败
	else if(action==NetData::Action::tcpAcceptError) {
		if(netStatus==NetStatus::tcpListen) {
			OnBnClickedButtonListen();
		}
	}
	//tcp监听状态下或tcp连接状态下对方结束
	else if(action==NetData::Action::tcpReadEof) {
		if(netStatus==NetStatus::tcpListen || netStatus==NetStatus::tcpConnect) {
			editListenStatus.SetWindowTextW(L"单向连接");
		}
	}
	else if(action==NetData::Action::tcpReadError) {
		//tcp监听状态下读取失败
		if(netStatus==NetStatus::tcpListen) {
			OnBnClickedButtonListenAccept();
		}
		//tcp连接状态下读取失败
		else if(netStatus==NetStatus::tcpConnect) {
			OnBnClickedButtonTcpConnect();
		}
	}
	else if(action==NetData::Action::udpRecvError) {
		//udp监听状态下读取失败
		if(netStatus==NetStatus::udpListen) {
			OnBnClickedButtonListenAccept();
		}
		//udp触发状态下读取失败
		else if(netStatus==NetStatus::udpRecv) {
			OnBnClickedButtonUdpRecv();
		}
	}
	else {
		assert(false);
	}
	return 0;
}


//改变状态
void C捕包辅助小工具Dlg::NetStatusChange(NetStatus status)
{
	netStatus = status;
	//无状态
	if(status==NetStatus::none) {
		editNetStatus.SetWindowTextW(L"无动作");
		buttonTcpConnect.SetWindowTextW(L"tcp连接");
		buttonTcpConnect.EnableWindow(true);
		buttonUdpRecv.SetWindowTextW(L"udp触发");
		buttonUdpRecv.EnableWindow(true);
		buttonAllSend.SetWindowTextW(L"发送");
		buttonAllSend.EnableWindow(false);
		buttonListen.SetWindowTextW(L"tcp监听");
		buttonListen.EnableWindow(true);
		buttonListenAccept.SetWindowTextW(L"udp监听");
		buttonListenAccept.EnableWindow(true);
		editListenStatus.SetWindowTextW(L"无连接");
	}
	//tcp监听状态
	else if(status==NetStatus::tcpListen) {
		editNetStatus.SetWindowTextW(L"tcp监听");
		buttonTcpConnect.SetWindowTextW(L"tcp连接");
		buttonTcpConnect.EnableWindow(false);
		buttonUdpRecv.SetWindowTextW(L"udp触发");
		buttonUdpRecv.EnableWindow(false);
		buttonAllSend.SetWindowTextW(L"tcp发送");
		buttonAllSend.EnableWindow(false);
		buttonListen.SetWindowTextW(L"停止监听");
		buttonListen.EnableWindow(true);
		buttonListenAccept.SetWindowTextW(L"断开连接");
		buttonListenAccept.EnableWindow(false);
		editListenStatus.SetWindowTextW(L"监听状态");
	}
	//tcp连接状态
	else if(status==NetStatus::tcpConnect) {
		editNetStatus.SetWindowTextW(L"tcp连接");
		buttonTcpConnect.SetWindowTextW(L"断开连接");
		buttonTcpConnect.EnableWindow(true);
		buttonUdpRecv.SetWindowTextW(L"udp触发");
		buttonUdpRecv.EnableWindow(false);
		buttonAllSend.SetWindowTextW(L"tcp发送");
		buttonAllSend.EnableWindow(true);
		buttonListen.SetWindowTextW(L"tcp监听");
		buttonListen.EnableWindow(false);
		buttonListenAccept.SetWindowTextW(L"udp监听");
		buttonListenAccept.EnableWindow(false);
		editListenStatus.SetWindowTextW(L"双向状态");
	}
	//udp监听状态
	else if(status==NetStatus::udpListen) {
		editNetStatus.SetWindowTextW(L"udp监听");
		buttonTcpConnect.SetWindowTextW(L"tcp连接");
		buttonTcpConnect.EnableWindow(false);
		buttonUdpRecv.SetWindowTextW(L"udp触发");
		buttonUdpRecv.EnableWindow(false);
		buttonAllSend.SetWindowTextW(L"udp发送");
		buttonAllSend.EnableWindow(true);
		buttonListen.SetWindowTextW(L"tcp监听");
		buttonListen.EnableWindow(false);
		buttonListenAccept.SetWindowTextW(L"停止监听");
		buttonListenAccept.EnableWindow(true);
		editListenStatus.SetWindowTextW(L"非状态连接");
	}
	//udp触发状态
	else if(status==NetStatus::udpRecv) {
		editNetStatus.SetWindowTextW(L"udp触发");
		buttonTcpConnect.SetWindowTextW(L"tcp连接");
		buttonTcpConnect.EnableWindow(false);
		buttonUdpRecv.SetWindowTextW(L"关闭端口");
		buttonUdpRecv.EnableWindow(true);
		buttonAllSend.SetWindowTextW(L"udp发送");
		buttonAllSend.EnableWindow(true);
		buttonListen.SetWindowTextW(L"tcp监听");
		buttonListen.EnableWindow(false);
		buttonListenAccept.SetWindowTextW(L"udp监听");
		buttonListenAccept.EnableWindow(false);
		editListenStatus.SetWindowTextW(L"非状态连接");
	}
	else {
		assert(false);
	}
}


//tcp连接按钮
void C捕包辅助小工具Dlg::OnBnClickedButtonTcpConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	//无状态转换tcp连接状态
	if(netStatus==NetStatus::none) {
		//获取连接地址
		CString cstrIp, cstrPort;
		editRecvIp.GetWindowTextW(cstrIp);
		string strIp;
		CstrToStr(strIp, cstrIp);
		asio::error_code err;
		asio::ip::address_v4 ipv4 = asio::ip::make_address_v4(strIp, err);
		if(err) {
			CString errMsg;
			StrToCstr(errMsg, err.message());
			MessageBox(L"ip错误: "+errMsg+"\r\n");
			return;
		}
		//获取连接端口
		editRecvPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"端口转换数字错误\r\n");
			return;
		}
		//开启监听
		if(!netData.TcpConnect(ipv4, port))
			return;
		//转换状态
		NetStatusChange(NetStatus::tcpConnect);
	}
	//tcp连接状态转换无状态
	else if(netStatus==NetStatus::tcpConnect) {
		//关闭监听
		netData.TcpDisConnect(false);
		//转换状态
		NetStatusChange(NetStatus::none);
	}
	else {
		assert(false);
	}
}


//udp触发按钮
void C捕包辅助小工具Dlg::OnBnClickedButtonUdpRecv()
{
	// TODO: 在此添加控件通知处理程序代码
	//无状态转换udp触发状态
	if(netStatus==NetStatus::none) {
		//获取连接地址
		CString cstrIp, cstrPort;
		editRecvIp.GetWindowTextW(cstrIp);
		string strIp;
		CstrToStr(strIp, cstrIp);
		asio::error_code err;
		asio::ip::address_v4 ipv4 = asio::ip::make_address_v4(strIp, err);
		if(err) {
			CString errMsg;
			StrToCstr(errMsg, err.message());
			MessageBox(L"ip错误: "+errMsg+"\r\n");
			return;
		}
		//获取连接端口
		editRecvPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"端口转换数字错误\r\n");
			return;
		}
		//获取发送字符串
		CString cstr;
		editSendText.GetWindowTextW(cstr);
		string str, strSend;
		CstrToStr(str, cstr);
		strSend = StrTextToBin(str);
		//触发端口
		if(!netData.UdpOpen(ipv4, port, strSend))
			return;
		//转换状态
		NetStatusChange(NetStatus::udpRecv);
	}
	//udp触发状态转换无状态
	else if(netStatus==NetStatus::udpRecv) {
		//关闭udp
		netData.UdpClose(false);
		//转换状态
		NetStatusChange(NetStatus::none);
	}
	else {
		assert(false);
	}
}


//发送按钮
void C捕包辅助小工具Dlg::OnBnClickedButtonAllSend()
{
	// TODO: 在此添加控件通知处理程序代码
	//tcp监听状态或tcp连接状态
	if(netStatus==NetStatus::tcpListen || netStatus==NetStatus::tcpConnect) {
		//获取发送字符串
		CString cstr;
		editSendText.GetWindowTextW(cstr);
		string str, strSend;
		CstrToStr(str, cstr);
		strSend = StrTextToBin(str);
		//发送
		netData.TcpWrite(strSend);
	}
	//udp监听状态或udp触发状态
	else if(netStatus==NetStatus::udpListen || netStatus==NetStatus::udpRecv) {
		//获取连接地址
		CString cstrIp, cstrPort;
		editRecvIp.GetWindowTextW(cstrIp);
		string strIp;
		CstrToStr(strIp, cstrIp);
		asio::error_code err;
		asio::ip::address_v4 ipv4 = asio::ip::make_address_v4(strIp, err);
		if(err) {
			CString errMsg;
			StrToCstr(errMsg, err.message());
			MessageBox(L"ip错误: "+errMsg+"\r\n");
			return;
		}
		//获取连接端口
		editRecvPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"端口转换数字错误\r\n");
			return;
		}
		//获取发送字符串
		CString cstr;
		editSendText.GetWindowTextW(cstr);
		string str, strSend;
		CstrToStr(str, cstr);
		strSend = StrTextToBin(str);
		//发送
		netData.UdpSend(ipv4, port, strSend);
	}
	else {
		assert(false);
	}

}


//监听按钮
void C捕包辅助小工具Dlg::OnBnClickedButtonListen()
{
	// TODO: 在此添加控件通知处理程序代码
	//无状态转换监听状态
	if(netStatus==NetStatus::none) {
		//获取监听端口字符串
		CString cstrPort;
		editListenPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"端口转换数字错误\r\n");
			return;
		}
		//开启监听
		if(!netData.TcpListen(port))
			return;
		//转换状态
		NetStatusChange(NetStatus::tcpListen);
	}
	//监听状态转换无状态
	else if(netStatus==NetStatus::tcpListen) {
		//关闭监听
		netData.TcpDisListen();
		//转换状态
		NetStatusChange(NetStatus::none);
	}
	else {
		assert(false);
	}
}


//监听和接受连接按钮
void C捕包辅助小工具Dlg::OnBnClickedButtonListenAccept()
{
	// TODO: 在此添加控件通知处理程序代码
	//监听状态下断开连接
	if(netStatus==NetStatus::tcpListen) {
		//断开连接
		netData.TcpDisConnect(true);
		editNetStatus.SetWindowTextW(L"监听状态");
		buttonListenAccept.EnableWindow(false);
		buttonAllSend.EnableWindow(false);
	}
	//无状态下udp监听
	else if(netStatus==NetStatus::none) {
		//获取监听端口字符串
		CString cstrPort;
		editListenPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"端口转换数字错误\r\n");
			return;
		}
		//开启监听
		if(!netData.UdpListen(port))
			return;
		//转换状态
		NetStatusChange(NetStatus::udpListen);
	}
	//监听状态转换无状态
	else if(netStatus==NetStatus::udpListen) {
		//关闭监听
		netData.UdpClose(true);
		//转换状态
		NetStatusChange(NetStatus::none);
	}
	else {
		assert(false);
	}
}


//清除发送编辑框
void C捕包辅助小工具Dlg::OnBnClickedButtonClearSend()
{
	// TODO: 在此添加控件通知处理程序代码
	editSendText.SetWindowTextW(L"");
}


//清除日志编辑框
void C捕包辅助小工具Dlg::OnBnClickedButtonClearRecv()
{
	// TODO: 在此添加控件通知处理程序代码
	editRecvText.SetWindowTextW(L"");
}
