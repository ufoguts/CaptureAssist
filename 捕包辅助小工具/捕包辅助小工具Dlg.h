
// ��������С����Dlg.h : ͷ�ļ�
//

#pragma once


#include "NetData.h"

// C��������С����Dlg �Ի���
class C��������С����Dlg : public CDialogEx
{
public:
	//����io״̬
	enum class NetStatus
	{
		none,//��
		tcpConnect,//tcp����
		tcpListen,//tcp����
		udpRecv,//udp����
		udpListen,//udp����
	};

private:
	//�ؼ�����
	CEdit editHex;//ipת��
	CEdit editIp;//ipת��
	CEdit editIpnum;//ipת��
	CEdit editBeforeRemove;//ȥ�ո�
	CEdit editAfterRemove;//ȥ�ո�
	CEdit editLinuxTime;//ʱ��ת��
	CEdit editLocalTime;//ʱ��ת��
	CEdit editUTC;//ʱ��ת��
	CEdit editRecvIp;//���緢�ͽ���
	CEdit editRecvPort;//���緢�ͽ���
	CEdit editNetStatus;//���緢�ͽ���
	CEdit editListenPort;//���緢�ͽ���
	CEdit editListenStatus;//���緢�ͽ���
	CEdit editSendText;//���緢�ͽ���
	CEdit editRecvText;//���緢�ͽ���
	CButton buttonTcpConnect;//���緢�ͽ���
	CButton buttonUdpRecv;//���緢�ͽ���
	CButton buttonAllSend;//���緢�ͽ���
	CButton buttonListen;//���緢�ͽ���
	CButton buttonListenAccept;//���緢�ͽ���

	//����
	CWndAttache attache;//�󶨸���
	bool bInnerIpChange;//����ip�䶯
	bool bInnerTimeChange;//����ʱ��䶯
	NetStatus netStatus;//����io״̬
	NetData netData;//���紦������
	thread thdNet;//���紦���߳�



// ����
public:
	C��������С����Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	~C��������С����Dlg() override;	// ��������

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	//��Ա����
private:
	void UpdataIpEdit(unsigned long long num, bool bHex, bool bNum, bool bIp);
	time_t StrToTmstu(const string &str, std::tm &tmstu);//ת����������
	void NetStatusChange(NetStatus status);//�ı�״̬

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
