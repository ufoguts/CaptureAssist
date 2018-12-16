
// ��������С����Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Common.h"
#include "��������С����.h"
#include "��������С����Dlg.h"
#include "afxdialogex.h"



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// C��������С����Dlg �Ի���



C��������С����Dlg::C��������С����Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY_DIALOG, pParent),
	thdNet(&NetData::ThdFunc, &netData)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

C��������С����Dlg::~C��������С����Dlg()
{
	//�˳������߳�
	netData.Exit();
	thdNet.join();

	CDialog::~CDialog();
}

void C��������С����Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C��������С����Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_UPDATE(IDC_EDIT_HEX, &C��������С����Dlg::OnUpdateEditHex)
	ON_EN_UPDATE(IDC_EDIT_IP, &C��������С����Dlg::OnUpdateEditIp)
	ON_EN_CHANGE(IDC_EDIT_BEFORE_REMOVE, &C��������С����Dlg::OnChangeEditBeforeRemove)
	ON_BN_CLICKED(IDOK, &C��������С����Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_REMOVE, &C��������С����Dlg::OnBnClickedButtonClearRemove)
	ON_BN_CLICKED(IDC_BUTTON_COPY_REMOVE, &C��������С����Dlg::OnBnClickedButtonCopyRemove)
	ON_EN_CHANGE(IDC_EDIT_LINUX_TIME, &C��������С����Dlg::OnEnChangeEditLinuxTime)
	ON_EN_CHANGE(IDC_EDIT_LOCAL_TIME, &C��������С����Dlg::OnEnChangeEditLocalTime)
	ON_EN_CHANGE(IDC_EDIT_UTC, &C��������С����Dlg::OnEnChangeEditUtc)
	ON_EN_UPDATE(IDC_EDIT_LINUX_TIME, &C��������С����Dlg::OnEnUpdateEditLinuxTime)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN, &C��������С����Dlg::OnBnClickedButtonListen)
	ON_MESSAGE(WM_SHOW_MESSAGE_BOX, &C��������С����Dlg::OnShowMessageBox)
	ON_MESSAGE(WM_NET_EDIT_LOGOUT, &C��������С����Dlg::OnNetEditLogout)
	ON_MESSAGE(WM_NET_THREAD_ACTION, &C��������С����Dlg::OnNetThreadAction)
	ON_BN_CLICKED(IDC_BUTTON_TCP_CONNECT, &C��������С����Dlg::OnBnClickedButtonTcpConnect)
	ON_BN_CLICKED(IDC_BUTTON_UDP_RECV, &C��������С����Dlg::OnBnClickedButtonUdpRecv)
	ON_BN_CLICKED(IDC_BUTTON_ALL_SEND, &C��������С����Dlg::OnBnClickedButtonAllSend)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN_ACCEPT, &C��������С����Dlg::OnBnClickedButtonListenAccept)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SEND, &C��������С����Dlg::OnBnClickedButtonClearSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RECV, &C��������С����Dlg::OnBnClickedButtonClearRecv)
	ON_EN_UPDATE(IDC_EDIT_IPNUM, &C��������С����Dlg::OnEnUpdateEditIpnum)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &C��������С����Dlg::OnBnClickedButtonReverse)
END_MESSAGE_MAP()


// C��������С����Dlg ��Ϣ�������

BOOL C��������С����Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//�󶨿ؼ�
	//ipת��
	attache.Attache(editHex, this, IDC_EDIT_HEX);
	attache.Attache(editIp, this, IDC_EDIT_IP);
	attache.Attache(editIpnum, this, IDC_EDIT_IPNUM);
	//ȥ�ո�
	attache.Attache(editBeforeRemove, this, IDC_EDIT_BEFORE_REMOVE);
	attache.Attache(editAfterRemove, this, IDC_EDIT_AFTER_REMOVE);
	//ʱ��ת��
	attache.Attache(editLinuxTime, this, IDC_EDIT_LINUX_TIME);
	attache.Attache(editLocalTime, this, IDC_EDIT_LOCAL_TIME);
	attache.Attache(editUTC, this, IDC_EDIT_UTC);
	//���緢�ͽ���
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

	//ip��س�ʼ��
	bInnerIpChange = false;
	editIp.SetWindowTextW(L"192.168.0.1");
	//ʱ��ת����ʼ��
	bInnerTimeChange = false;
	editLinuxTime.SetWindowTextW(L"0");
	//����io��ʼ��
	NetStatusChange(NetStatus::none);
	netData.Init(m_hWnd);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C��������С����Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C��������С����Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C��������С����Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ʾmessagebox����
afx_msg LRESULT C��������С����Dlg::OnShowMessageBox(WPARAM wParam, LPARAM lParam)
{
	//Ҫ�������Ϊ�ѿռ�CString
	auto pCstr = reinterpret_cast<CString *>(lParam);
	MessageBox(*pCstr);
	//һ����Ϊ1��ɾ���ڴ�
	if(wParam!=1)
		delete pCstr;
	return 0;
}


//�ı�Ի���رղ���
void C��������С����Dlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}
void C��������С����Dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}


//�ı�ip�༭��������
void C��������С����Dlg::UpdataIpEdit(unsigned long long num, bool bHex, bool bNum, bool bIp)
{
	CString cstrTmp;
	//����hex��
	if(bHex) {
		ostringstream oss;
		oss <<std::hex <<std::uppercase <<std::setw(8) <<std::setfill('0') <<num;
		StrToCstr(cstrTmp, oss.str());
		editHex.SetWindowTextW(cstrTmp);
	}
	//����num��
	if(bNum) {
		StrToCstr(cstrTmp, std::to_string(num));
		editIpnum.SetWindowTextW(cstrTmp);
	}
	//����ip��
	if(bIp) {
		char sz[16];
		unsigned long ipAddr = htonl((unsigned long)num);
		inet_ntop(AF_INET, &ipAddr, sz, 16);
		StrToCstr(cstrTmp, sz);
		editIp.SetWindowTextW(cstrTmp);
	}
}


//hex���ı�ǰ��Ϣ����
void C��������С����Dlg::OnUpdateEditHex()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// �������Խ� EM_SETEVENTMASK ��Ϣ���͵��ÿؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(bInnerIpChange)
		return;
	bInnerIpChange = true;

	//��ȡ�ַ���
	CString cstrOri, cstrTmp;
	editHex.GetWindowTextW(cstrOri);
	string strOri;
	CstrToStr(strOri, cstrOri);
	bool bForm = true;
	//ת������
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
	//���±༭��
	UpdataIpEdit(num, !bForm, true, true);

	bInnerIpChange = false;
}


//ipnum�ı�ǰ��Ϣ����
void C��������С����Dlg::OnEnUpdateEditIpnum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// �������Խ� EM_SETEVENTMASK ��Ϣ���͵��ÿؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(bInnerIpChange)
		return;
	bInnerIpChange = true;

	//��ȡ�ַ���
	CString cstrOri, cstrTmp;
	editIpnum.GetWindowTextW(cstrOri);
	string strOri;
	CstrToStr(strOri, cstrOri);
	//ת������
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
	//���±༭��
	UpdataIpEdit(num, true, !bForm, true);

	bInnerIpChange = false;
}


//ip���ı�ǰ��Ϣ����
void C��������С����Dlg::OnUpdateEditIp()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// �������Խ� EM_SETEVENTMASK ��Ϣ���͵��ÿؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(bInnerIpChange)
		return;
	bInnerIpChange = true;

	//��ȡ�ַ���
	CString cstrOri, cstrTmp;
	editIp.GetWindowTextW(cstrOri);
	string strOri;
	CstrToStr(strOri, cstrOri);
	//ת������
	bool bForm = true;
	unsigned long long num;
	if(strOri.size()>15) {
		strOri.resize(15);
		bForm = false;
	}
	unsigned long ipAddr = 0;
	inet_pton(AF_INET, strOri.c_str(), &ipAddr);
	num = ntohl(ipAddr);
	//����ip��
	if(!bForm) {
		StrToCstr(cstrTmp, strOri);
		editIp.SetWindowTextW(cstrTmp);
	}
	//����������
	UpdataIpEdit(num, true, true, false);

	bInnerIpChange = false;
}


//����ť����
void C��������С����Dlg::OnBnClickedButtonReverse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(bInnerIpChange)
		return;
	bInnerIpChange = true;

	//��ȡ�ַ���
	CString cstrOri, cstrTmp;
	editIpnum.GetWindowTextW(cstrOri);
	string strOri;
	CstrToStr(strOri, cstrOri);
	//ת������
	unsigned long long num;
	FromString(strOri, num);
	if(num>=(1ULL<<32)) {
		num = (1ULL<<32)-1;
	}
	//����
	num = ntohl(static_cast<uint32_t>(num));
	//���±༭��
	UpdataIpEdit(num, true, true, true);

	bInnerIpChange = false;
}


//beforeremove�ı���Ϣ����
void C��������С����Dlg::OnChangeEditBeforeRemove()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strBefore, strAfter;
	editBeforeRemove.GetWindowTextW(strBefore);
	for(int i=0; i<strBefore.GetLength(); ++i) {
		//���ǻس�
		if(!(i+1<strBefore.GetLength() && strBefore[i]=='\r' && strBefore[i+1]=='\n'))
			strAfter += strBefore[i];
		//��Ϊ�س�
		else {
			//��ǰ��û��������Ϊ�س�����������
			if(i==0)
				;
			//��ǰ��Ϊ�س���ԭ������
			else if(i>=2 && strBefore[i-2]=='\r' && strBefore[i-1]=='\n')
				strAfter += "\r\n";
			//��ǰ��Ϊ�����ߣ�ȥ����������
			else if(strBefore[i-1]=='-')
				strAfter.Delete(strAfter.GetLength()-1);
			//���򣬼���ո�
			else
				strAfter += ' ';
			++ i;
		}
	}
	editAfterRemove.SetWindowTextW(strAfter);
}


//���ȥ���س��༭��
void C��������С����Dlg::OnBnClickedButtonClearRemove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	editBeforeRemove.SetWindowTextW(L"");
	OnChangeEditBeforeRemove();
}


//����ȥ�����лس��༭��
void C��������С����Dlg::OnBnClickedButtonCopyRemove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString text;
	editAfterRemove.GetWindowTextW(text);
	//�򿪼�����
	if(::OpenClipboard(m_hWnd)) {
		::EmptyClipboard();
		//��ȡȫ���ڴ��
		HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(wchar_t)*text.GetLength()+2);
		LPSTR pData = (LPSTR)::GlobalLock(hData);
		//������
		memcpy(pData, text.GetString(), sizeof(wchar_t)*text.GetLength()+2);
		::GlobalUnlock(hData);
		//ȫ���ڴ�齻��������
		::SetClipboardData(CF_UNICODETEXT, hData);
		::CloseClipboard();
	}
	else
		MessageBox(L"Clipboard Open Fail");
}


//LinuxTime����ǰ
void C��������С����Dlg::OnEnUpdateEditLinuxTime()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// �������Խ� EM_SETEVENTMASK ��Ϣ���͵��ÿؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(bInnerTimeChange)
		return;
	bInnerTimeChange = true;
	
	//���ĸ�ʽ��ʱ��
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


//LinuxTime����
void C��������С����Dlg::OnEnChangeEditLinuxTime()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(bInnerTimeChange)
		return;
	bInnerTimeChange = true;

	//��ȡʱ��
	CString cstrText;
	editLinuxTime.GetWindowTextW(cstrText);
	string strText(cstrText.GetBuffer(), cstrText.GetBuffer()+cstrText.GetLength());
	time_t time;
	FromString(strText, time);
	//ת������ʱ��
	strText = TimeToStr(time);
	std::tm *pTmstu;
	char buf[256];
	pTmstu = std::localtime(&time);
	if(strftime(buf, sizeof(buf), "%F %T", pTmstu)) {
		CString cstrTime(buf);
		editLocalTime.SetWindowTextW(cstrTime);
	}
	//ת��UTC
	pTmstu = std::gmtime(&time);
	if(strftime(buf, sizeof(buf), "%F %T", pTmstu)) {
		CString cstrTime(buf);
		editUTC.SetWindowTextW(cstrTime);
	}
	
	bInnerTimeChange = false;
}


//ת����������
time_t C��������С����Dlg::StrToTmstu(const string &str, std::tm &tmstu)
{
	string::const_iterator itSt, itEd;
	auto funcIsNum = [](char ch) {
		return ch>='0' && ch<='9'; };
	itEd = str.begin();
	bool bAble = true;
	//ͨ�ô�����
	auto funcFindNext = [&str, &funcIsNum, &itSt, &itEd, &bAble](int &num, int offset) {
		if(!bAble)
			return;
		itSt = std::find_if(itEd, str.end(), funcIsNum);
		itEd = std::find_if_not(itSt, str.end(), funcIsNum);
		if(!FromString(itSt, itEd, num))
			bAble = false;
		num += offset;
	};
	//����tmstu
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


//LocalTime����
void C��������С����Dlg::OnEnChangeEditLocalTime()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(bInnerTimeChange)
		return;
	bInnerTimeChange = true;

	//��ȡʱ��
	CString cstrText;
	editLocalTime.GetWindowTextW(cstrText);
	string strText(cstrText.GetBuffer(), cstrText.GetBuffer()+cstrText.GetLength());
	std::tm tmstu;
	//ת����linux
	long long time = StrToTmstu(strText, tmstu);
	cstrText.Format(L"%lld", time);
	editLinuxTime.SetWindowTextW(cstrText);
	//ת����UTC
	std::tm *pTmstu = std::gmtime(&time);
	char buf[256];
	if(strftime(buf, sizeof(buf), "%F %T", pTmstu)) {
		CString cstrTime(buf);
		editUTC.SetWindowTextW(cstrTime);
	}
	
	bInnerTimeChange = false;
}


//UTC����
void C��������С����Dlg::OnEnChangeEditUtc()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(bInnerTimeChange)
		return;
	bInnerTimeChange = true;
	
	bInnerTimeChange = false;
}


//��ʾ��־��Ϣ
afx_msg LRESULT C��������С����Dlg::OnNetEditLogout(WPARAM wParam, LPARAM lParam)
{
	//Ҫ�������Ϊ�ѿռ�CString
	auto pCstr = reinterpret_cast<CString *>(lParam);
	CString cstr;
	//һ����Ϊ1��ͬʱ����messagebox
	if(wParam==1) {
		OnShowMessageBox(1, lParam);
	}
	//������־��Ϣ
	editRecvText.GetWindowTextW(cstr);
	cstr += *pCstr;
	editRecvText.SetWindowTextW(cstr);
	editRecvText.LineScroll(editRecvText.GetLineCount());
	//ɾ���ڴ�
	delete pCstr;
	return 0;
}


//�̻߳ش�״̬��Ϣ
afx_msg LRESULT C��������С����Dlg::OnNetThreadAction(WPARAM wParam, LPARAM lParam)
{
	auto action = static_cast<NetData::Action>(lParam);
	//tcp��������½�������
	if(action==NetData::Action::tcpAccept) {
		if(netStatus==NetStatus::tcpListen) {
			editListenStatus.SetWindowTextW(L"˫������");
			buttonListenAccept.EnableWindow(true);
			buttonAllSend.EnableWindow(true);
		}
	}
	//tcp����״̬�½�������ʧ��
	else if(action==NetData::Action::tcpAcceptError) {
		if(netStatus==NetStatus::tcpListen) {
			OnBnClickedButtonListen();
		}
	}
	//tcp����״̬�»�tcp����״̬�¶Է�����
	else if(action==NetData::Action::tcpReadEof) {
		if(netStatus==NetStatus::tcpListen || netStatus==NetStatus::tcpConnect) {
			editListenStatus.SetWindowTextW(L"��������");
		}
	}
	else if(action==NetData::Action::tcpReadError) {
		//tcp����״̬�¶�ȡʧ��
		if(netStatus==NetStatus::tcpListen) {
			OnBnClickedButtonListenAccept();
		}
		//tcp����״̬�¶�ȡʧ��
		else if(netStatus==NetStatus::tcpConnect) {
			OnBnClickedButtonTcpConnect();
		}
	}
	else if(action==NetData::Action::udpRecvError) {
		//udp����״̬�¶�ȡʧ��
		if(netStatus==NetStatus::udpListen) {
			OnBnClickedButtonListenAccept();
		}
		//udp����״̬�¶�ȡʧ��
		else if(netStatus==NetStatus::udpRecv) {
			OnBnClickedButtonUdpRecv();
		}
	}
	else {
		assert(false);
	}
	return 0;
}


//�ı�״̬
void C��������С����Dlg::NetStatusChange(NetStatus status)
{
	netStatus = status;
	//��״̬
	if(status==NetStatus::none) {
		editNetStatus.SetWindowTextW(L"�޶���");
		buttonTcpConnect.SetWindowTextW(L"tcp����");
		buttonTcpConnect.EnableWindow(true);
		buttonUdpRecv.SetWindowTextW(L"udp����");
		buttonUdpRecv.EnableWindow(true);
		buttonAllSend.SetWindowTextW(L"����");
		buttonAllSend.EnableWindow(false);
		buttonListen.SetWindowTextW(L"tcp����");
		buttonListen.EnableWindow(true);
		buttonListenAccept.SetWindowTextW(L"udp����");
		buttonListenAccept.EnableWindow(true);
		editListenStatus.SetWindowTextW(L"������");
	}
	//tcp����״̬
	else if(status==NetStatus::tcpListen) {
		editNetStatus.SetWindowTextW(L"tcp����");
		buttonTcpConnect.SetWindowTextW(L"tcp����");
		buttonTcpConnect.EnableWindow(false);
		buttonUdpRecv.SetWindowTextW(L"udp����");
		buttonUdpRecv.EnableWindow(false);
		buttonAllSend.SetWindowTextW(L"tcp����");
		buttonAllSend.EnableWindow(false);
		buttonListen.SetWindowTextW(L"ֹͣ����");
		buttonListen.EnableWindow(true);
		buttonListenAccept.SetWindowTextW(L"�Ͽ�����");
		buttonListenAccept.EnableWindow(false);
		editListenStatus.SetWindowTextW(L"����״̬");
	}
	//tcp����״̬
	else if(status==NetStatus::tcpConnect) {
		editNetStatus.SetWindowTextW(L"tcp����");
		buttonTcpConnect.SetWindowTextW(L"�Ͽ�����");
		buttonTcpConnect.EnableWindow(true);
		buttonUdpRecv.SetWindowTextW(L"udp����");
		buttonUdpRecv.EnableWindow(false);
		buttonAllSend.SetWindowTextW(L"tcp����");
		buttonAllSend.EnableWindow(true);
		buttonListen.SetWindowTextW(L"tcp����");
		buttonListen.EnableWindow(false);
		buttonListenAccept.SetWindowTextW(L"udp����");
		buttonListenAccept.EnableWindow(false);
		editListenStatus.SetWindowTextW(L"˫��״̬");
	}
	//udp����״̬
	else if(status==NetStatus::udpListen) {
		editNetStatus.SetWindowTextW(L"udp����");
		buttonTcpConnect.SetWindowTextW(L"tcp����");
		buttonTcpConnect.EnableWindow(false);
		buttonUdpRecv.SetWindowTextW(L"udp����");
		buttonUdpRecv.EnableWindow(false);
		buttonAllSend.SetWindowTextW(L"udp����");
		buttonAllSend.EnableWindow(true);
		buttonListen.SetWindowTextW(L"tcp����");
		buttonListen.EnableWindow(false);
		buttonListenAccept.SetWindowTextW(L"ֹͣ����");
		buttonListenAccept.EnableWindow(true);
		editListenStatus.SetWindowTextW(L"��״̬����");
	}
	//udp����״̬
	else if(status==NetStatus::udpRecv) {
		editNetStatus.SetWindowTextW(L"udp����");
		buttonTcpConnect.SetWindowTextW(L"tcp����");
		buttonTcpConnect.EnableWindow(false);
		buttonUdpRecv.SetWindowTextW(L"�رն˿�");
		buttonUdpRecv.EnableWindow(true);
		buttonAllSend.SetWindowTextW(L"udp����");
		buttonAllSend.EnableWindow(true);
		buttonListen.SetWindowTextW(L"tcp����");
		buttonListen.EnableWindow(false);
		buttonListenAccept.SetWindowTextW(L"udp����");
		buttonListenAccept.EnableWindow(false);
		editListenStatus.SetWindowTextW(L"��״̬����");
	}
	else {
		assert(false);
	}
}


//tcp���Ӱ�ť
void C��������С����Dlg::OnBnClickedButtonTcpConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��״̬ת��tcp����״̬
	if(netStatus==NetStatus::none) {
		//��ȡ���ӵ�ַ
		CString cstrIp, cstrPort;
		editRecvIp.GetWindowTextW(cstrIp);
		string strIp;
		CstrToStr(strIp, cstrIp);
		asio::error_code err;
		asio::ip::address_v4 ipv4 = asio::ip::make_address_v4(strIp, err);
		if(err) {
			CString errMsg;
			StrToCstr(errMsg, err.message());
			MessageBox(L"ip����: "+errMsg+"\r\n");
			return;
		}
		//��ȡ���Ӷ˿�
		editRecvPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"�˿�ת�����ִ���\r\n");
			return;
		}
		//��������
		if(!netData.TcpConnect(ipv4, port))
			return;
		//ת��״̬
		NetStatusChange(NetStatus::tcpConnect);
	}
	//tcp����״̬ת����״̬
	else if(netStatus==NetStatus::tcpConnect) {
		//�رռ���
		netData.TcpDisConnect(false);
		//ת��״̬
		NetStatusChange(NetStatus::none);
	}
	else {
		assert(false);
	}
}


//udp������ť
void C��������С����Dlg::OnBnClickedButtonUdpRecv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��״̬ת��udp����״̬
	if(netStatus==NetStatus::none) {
		//��ȡ���ӵ�ַ
		CString cstrIp, cstrPort;
		editRecvIp.GetWindowTextW(cstrIp);
		string strIp;
		CstrToStr(strIp, cstrIp);
		asio::error_code err;
		asio::ip::address_v4 ipv4 = asio::ip::make_address_v4(strIp, err);
		if(err) {
			CString errMsg;
			StrToCstr(errMsg, err.message());
			MessageBox(L"ip����: "+errMsg+"\r\n");
			return;
		}
		//��ȡ���Ӷ˿�
		editRecvPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"�˿�ת�����ִ���\r\n");
			return;
		}
		//��ȡ�����ַ���
		CString cstr;
		editSendText.GetWindowTextW(cstr);
		string str, strSend;
		CstrToStr(str, cstr);
		strSend = StrTextToBin(str);
		//�����˿�
		if(!netData.UdpOpen(ipv4, port, strSend))
			return;
		//ת��״̬
		NetStatusChange(NetStatus::udpRecv);
	}
	//udp����״̬ת����״̬
	else if(netStatus==NetStatus::udpRecv) {
		//�ر�udp
		netData.UdpClose(false);
		//ת��״̬
		NetStatusChange(NetStatus::none);
	}
	else {
		assert(false);
	}
}


//���Ͱ�ť
void C��������С����Dlg::OnBnClickedButtonAllSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//tcp����״̬��tcp����״̬
	if(netStatus==NetStatus::tcpListen || netStatus==NetStatus::tcpConnect) {
		//��ȡ�����ַ���
		CString cstr;
		editSendText.GetWindowTextW(cstr);
		string str, strSend;
		CstrToStr(str, cstr);
		strSend = StrTextToBin(str);
		//����
		netData.TcpWrite(strSend);
	}
	//udp����״̬��udp����״̬
	else if(netStatus==NetStatus::udpListen || netStatus==NetStatus::udpRecv) {
		//��ȡ���ӵ�ַ
		CString cstrIp, cstrPort;
		editRecvIp.GetWindowTextW(cstrIp);
		string strIp;
		CstrToStr(strIp, cstrIp);
		asio::error_code err;
		asio::ip::address_v4 ipv4 = asio::ip::make_address_v4(strIp, err);
		if(err) {
			CString errMsg;
			StrToCstr(errMsg, err.message());
			MessageBox(L"ip����: "+errMsg+"\r\n");
			return;
		}
		//��ȡ���Ӷ˿�
		editRecvPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"�˿�ת�����ִ���\r\n");
			return;
		}
		//��ȡ�����ַ���
		CString cstr;
		editSendText.GetWindowTextW(cstr);
		string str, strSend;
		CstrToStr(str, cstr);
		strSend = StrTextToBin(str);
		//����
		netData.UdpSend(ipv4, port, strSend);
	}
	else {
		assert(false);
	}

}


//������ť
void C��������С����Dlg::OnBnClickedButtonListen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��״̬ת������״̬
	if(netStatus==NetStatus::none) {
		//��ȡ�����˿��ַ���
		CString cstrPort;
		editListenPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"�˿�ת�����ִ���\r\n");
			return;
		}
		//��������
		if(!netData.TcpListen(port))
			return;
		//ת��״̬
		NetStatusChange(NetStatus::tcpListen);
	}
	//����״̬ת����״̬
	else if(netStatus==NetStatus::tcpListen) {
		//�رռ���
		netData.TcpDisListen();
		//ת��״̬
		NetStatusChange(NetStatus::none);
	}
	else {
		assert(false);
	}
}


//�����ͽ������Ӱ�ť
void C��������С����Dlg::OnBnClickedButtonListenAccept()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����״̬�¶Ͽ�����
	if(netStatus==NetStatus::tcpListen) {
		//�Ͽ�����
		netData.TcpDisConnect(true);
		editNetStatus.SetWindowTextW(L"����״̬");
		buttonListenAccept.EnableWindow(false);
		buttonAllSend.EnableWindow(false);
	}
	//��״̬��udp����
	else if(netStatus==NetStatus::none) {
		//��ȡ�����˿��ַ���
		CString cstrPort;
		editListenPort.GetWindowTextW(cstrPort);
		string strPort;
		CstrToStr(strPort, cstrPort);
		unsigned port;
		if(!FromString(strPort, port)) {
			MessageBox(L"�˿�ת�����ִ���\r\n");
			return;
		}
		//��������
		if(!netData.UdpListen(port))
			return;
		//ת��״̬
		NetStatusChange(NetStatus::udpListen);
	}
	//����״̬ת����״̬
	else if(netStatus==NetStatus::udpListen) {
		//�رռ���
		netData.UdpClose(true);
		//ת��״̬
		NetStatusChange(NetStatus::none);
	}
	else {
		assert(false);
	}
}


//������ͱ༭��
void C��������С����Dlg::OnBnClickedButtonClearSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	editSendText.SetWindowTextW(L"");
}


//�����־�༭��
void C��������С����Dlg::OnBnClickedButtonClearRecv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	editRecvText.SetWindowTextW(L"");
}
