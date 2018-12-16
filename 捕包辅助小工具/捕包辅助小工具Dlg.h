
// 捕包辅助小工具Dlg.h : 头文件
//

#pragma once


#include "NetData.h"

// C捕包辅助小工具Dlg 对话框
class C捕包辅助小工具Dlg : public CDialogEx
{
public:
	//网络io状态
	enum class NetStatus
	{
		none,//无
		tcpConnect,//tcp连接
		tcpListen,//tcp监听
		udpRecv,//udp接收
		udpListen,//udp监听
	};

private:
	//控件引用
	CEdit editHex;//ip转换
	CEdit editIp;//ip转换
	CEdit editIpnum;//ip转换
	CEdit editBeforeRemove;//去空格
	CEdit editAfterRemove;//去空格
	CEdit editLinuxTime;//时间转换
	CEdit editLocalTime;//时间转换
	CEdit editUTC;//时间转换
	CEdit editRecvIp;//网络发送接收
	CEdit editRecvPort;//网络发送接收
	CEdit editNetStatus;//网络发送接收
	CEdit editListenPort;//网络发送接收
	CEdit editListenStatus;//网络发送接收
	CEdit editSendText;//网络发送接收
	CEdit editRecvText;//网络发送接收
	CButton buttonTcpConnect;//网络发送接收
	CButton buttonUdpRecv;//网络发送接收
	CButton buttonAllSend;//网络发送接收
	CButton buttonListen;//网络发送接收
	CButton buttonListenAccept;//网络发送接收

	//变量
	CWndAttache attache;//绑定辅助
	bool bInnerIpChange;//控制ip变动
	bool bInnerTimeChange;//控制时间变动
	NetStatus netStatus;//网络io状态
	NetData netData;//网络处理数据
	thread thdNet;//网络处理副线程



// 构造
public:
	C捕包辅助小工具Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~C捕包辅助小工具Dlg() override;	// 析构函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	//成员函数
private:
	void UpdataIpEdit(unsigned long long num, bool bHex, bool bNum, bool bIp);
	time_t StrToTmstu(const string &str, std::tm &tmstu);//转换辅助函数
	void NetStatusChange(NetStatus status);//改变状态

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateEditHex();
	afx_msg void OnUpdateEditIp();
	afx_msg void OnChangeEditBeforeRemove();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonClearRemove();
	afx_msg void OnBnClickedButtonCopyRemove();
	virtual void OnOK();
	afx_msg void OnEnChangeEditLinuxTime();
	afx_msg void OnEnChangeEditLocalTime();
	afx_msg void OnEnChangeEditUtc();
	afx_msg void OnEnUpdateEditLinuxTime();
	afx_msg void OnBnClickedButtonListen();
protected:
	afx_msg LRESULT OnShowMessageBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNetEditLogout(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNetThreadAction(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonTcpConnect();
	afx_msg void OnBnClickedButtonUdpRecv();
	afx_msg void OnBnClickedButtonAllSend();
	afx_msg void OnBnClickedButtonListenAccept();
	afx_msg void OnBnClickedButtonClearSend();
	afx_msg void OnBnClickedButtonClearRecv();
	afx_msg void OnEnUpdateEditIpnum();
	afx_msg void OnBnClickedButtonReverse();
};
