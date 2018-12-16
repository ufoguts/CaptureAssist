#pragma once

#include "��������С����Dlg.h"


//���紦��������
class NetData
{
public:
	//�ش���������Ϣ��Ϣ
	enum class Action
	{
		tcpAccept,
		tcpAcceptError,
		tcpReadEof,
		tcpReadError,
		udpRecvError,
	};
	//�ӳ�����������
	using ReadBuf = array<char, 256>;
	using ReadBufPtr = shared_ptr<ReadBuf>;
	using WriteBuf = string;
	using WriteBufPtr = shared_ptr<WriteBuf>;
	using UdpEndpoint = asio::udp::endpoint;
	using UdpEndpointPtr = shared_ptr<UdpEndpoint>;

private:
	HWND mainWnd;//�����ھ��
	bool bExit;//�˳�����
	mutex mtx;//������
	condition_variable_any cv;//ͬ��
	bool bWork;//��������
	asio::io_context ioContext;//ioʵ��
	asio::tcp::acceptor acceptor;//tcp������
	asio::tcp::socket tcpSocket;//tcp
	asio::udp::socket udpSocket;//udp

public:
	//����
	NetData():
		bExit(false), bWork(false),
		acceptor(ioContext), tcpSocket(ioContext), udpSocket(ioContext)
	{
	}
	//����
	~NetData()
		= default;
	//������ʼ��
	void Init(HWND o_mainWnd)
	{
		mainWnd = o_mainWnd;
	}

public:
	//�̺߳���
	void ThdFunc()
	{
		//����ѭ��
		while(true) {
			//���������ж��˳�����
			mtx.lock();
			//����ȴ�״̬
			cv.wait(mtx, [this]() {return bWork || bExit; });
			if(bExit) {
				mtx.unlock();
				break;
			}
			//���빤��״̬
			bWork = false;
			mtx.unlock();
			ioContext.run();
		}
	}

public:
	//����������̵߳���
	void AddTesk()
	{
		mtx.lock();
		bWork = true;
		mtx.unlock();
		cv.notify_one();
	}
	//ֹͣ�������̵߳���
	void StopTesk()
	{
		ioContext.stop();
		mtx.lock();
		bWork = false;
		mtx.unlock();
		ioContext.run();
	}
	//�����������̵߳���
	void ContinueTesk()
	{
		ioContext.restart();
		AddTesk();
	}
	//��������������̵߳���
	void RestartTesk()
	{
		ioContext.restart();
		ioContext.run();
		ioContext.restart();
	}
	//�˳������̵߳���
	void Exit()
	{
		mtx.lock();
		ioContext.stop();
		bExit = true;
		mtx.unlock();
		cv.notify_one();
	}

	//tcp���������̵߳���
	bool TcpListen(int port)
	{
		//�򿪰󶨼���
		asio::tcp::endpoint endpoint(asio::tcp::v4(), port);
		asio::error_code err;
		assert(!acceptor.is_open());
		acceptor.open(asio::tcp::v4(), err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("��������ʧ��: "+err.message()+"\r\n"));
			acceptor.close();
			return false;
		}
		acceptor.bind(endpoint, err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("��������ʧ��: "+err.message()+"\r\n"));
			acceptor.close();
			return false;
		}
		acceptor.listen(asio::socket_base::max_listen_connections, err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("����������ʧ��: "+err.message()+"\r\n"));
			acceptor.close();
			return false;
		}
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("��ʼtcp����:"+std::to_string(port)+"\r\n"));
		//�����첽����
		assert(!tcpSocket.is_open());
		acceptor.async_accept(tcpSocket,
			std::bind(&NetData::TcpAcceptFunc, this, placeholders::_1));
		//�������
		AddTesk();
		return true;
	}

	//tcp���ӣ����̵߳���
	bool TcpConnect(const asio::ip::address_v4 &ipv4, int port)
	{
		assert(!tcpSocket.is_open());
		asio::error_code err;
		//��������
		tcpSocket.connect(asio::tcp::endpoint(ipv4, port), err);
		//�����ж�
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp����ʧ��: "+err.message()+"\r\n"));
			tcpSocket.close();
			return false;
		}
		string strMsg = '(' +std::to_string(tcpSocket.local_endpoint().port())
			+")-" +ipv4.to_string() +':' +std::to_string(port);
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("tcp���ӳɹ�: "+strMsg+"\r\n"));
		//��ʼ�첽��ȡ
		auto spBuf = std::make_shared<ReadBuf>();
		tcpSocket.async_read_some(asio::buffer(*spBuf),
			std::bind(&NetData::TcpReadFunc, this, placeholders::_1,
			placeholders::_2, spBuf));
		//�������
		AddTesk();
		return true;
	}

	//tcpд�룬���̵߳���
	bool TcpWrite(const string &message)
	{
		//ֹͣ����
		StopTesk();
		//�жϿ���
		if(!tcpSocket.is_open()) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp����ʧ��: socket�ѹر�\r\n"));
			return false;
		}
		//�첽����
		auto spBuf = std::make_shared<WriteBuf>(message);
		asio::async_write(tcpSocket, asio::buffer(*spBuf),
			std::bind(&NetData::TcpWriteFunc, this, placeholders::_1,
			placeholders::_2, spBuf));
		//��������
		ContinueTesk();
		return true;
	}

	//tcp�Ͽ����ӣ��Ƿ�����������Ƿ���Ҫֹͣ�����̵߳���
	void TcpDisConnect(bool bListen, bool bStop= true)
	{
		//ֹͣ����
		if(bStop)
			StopTesk();
		assert(tcpSocket.is_open());
		//�ر�����
		asio::error_code err;
		//tcpSocket.cancel(err);
		//if(err) {
		//	::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
		//		NewCstrLong("tcp socketȡ���쳣: "+err.message()+"\r\n"));
		//}
		tcpSocket.close(err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp socket�ر��쳣: "+err.message()+"\r\n"));
		}
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("�����Ͽ�tcp����\r\n"));
		//�����첽����
		if(bListen) {
			acceptor.async_accept(tcpSocket,
				std::bind(&NetData::TcpAcceptFunc, this, placeholders::_1));
			//�������������
			if(bStop)
				ContinueTesk();
		}
		else {
			//�Ǽ�������������
			if(bStop)
				RestartTesk();
		}
	}

	//tcpֹͣ���������̵߳���
	void TcpDisListen()
	{
		//ֹͣ����
		StopTesk();
		//�ر�����
		if(tcpSocket.is_open()) {
			TcpDisConnect(false, false);
		}
		//ֹͣ����
		asio::error_code err;
		assert(acceptor.is_open());
		acceptor.cancel(err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("������ȡ���쳣: "+err.message()+"\r\n"));
		}
		acceptor.close(err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("�������ر��쳣: "+err.message()+"\r\n"));
		}
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("ֹͣtcp����\r\n"));
		//��������
		RestartTesk();
	}

	//udp���������̵߳���
	bool UdpListen(int port)
	{
		asio::error_code err;
		//�����˿�
		assert(!udpSocket.is_open());
		udpSocket.open(asio::udp::v4(), err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket��ʧ��: "+err.message()+"\r\n"));
			udpSocket.close();
			return false;
		}
		udpSocket.bind(asio::udp::endpoint(asio::udp::v4(), port), err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket��ʧ��: "+err.message()+"\r\n"));
			udpSocket.close();
			return false;
		}
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("��ʼudp����: "+std::to_string(port)+"\r\n"));
		//�����첽����
		auto spEndpoint = std::make_shared<UdpEndpoint>();
		auto spBuf = std::make_shared<ReadBuf>();
		udpSocket.async_receive_from(asio::buffer(*spBuf), *spEndpoint,
			std::bind(&NetData::UdpRecvFunc, this, placeholders::_1,
			placeholders::_2, spEndpoint, spBuf));
		//�������
		AddTesk();
		return true;
	}

	//udp���������̵߳���
	bool UdpOpen(const asio::ip::address_v4 &ipv4, int port,
		const string &message)
	{
		asio::error_code err;
		//�����˿�
		assert(!udpSocket.is_open());
		udpSocket.open(asio::udp::v4(), err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket��ʧ��: "+err.message()+"\r\n"));
			udpSocket.close();
			return false;
		}
		//����������Ϣ
		size_t size = udpSocket.send_to(asio::buffer(message),
			asio::udp::endpoint(ipv4, port), 0, err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket����ʧ��: "+err.message()+"\r\n"));
			udpSocket.close();
			return false;
		}
		//������Ϣ�ɹ�
		int portNew = udpSocket.local_endpoint().port();
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("��udp�ɹ�: "+std::to_string(portNew)+"\r\n"));
		string str(message.begin(), message.begin()+size);
		string strMsg = ipv4.to_string() +':' +std::to_string(port);
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("udp����("+strMsg+"): "+StrBinToText(str)+"\r\n"));
		//�����첽����
		auto spEndpoint = std::make_shared<UdpEndpoint>();
		auto spBuf = std::make_shared<ReadBuf>();
		udpSocket.async_receive_from(asio::buffer(*spBuf), *spEndpoint,
			std::bind(&NetData::UdpRecvFunc, this, placeholders::_1,
			placeholders::_2, spEndpoint, spBuf));
		//�������
		AddTesk();
		return true;
	}

	//udp���ͣ����̵߳���
	bool UdpSend(const asio::ip::address_v4 &ipv4, int port,
		const string &message)
	{
		//ֹͣ����
		StopTesk();
		//�жϿ���
		if(!udpSocket.is_open()) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp����ʧ��: socket�ѹر�\r\n"));
			return false;
		}
		//�첽����
		auto spEndpoint = std::make_shared<UdpEndpoint>(ipv4, port);
		auto spBuf = std::make_shared<WriteBuf>(message);
		udpSocket.async_send_to(asio::buffer(*spBuf), *spEndpoint,
			std::bind(&NetData::UdpSendFunc, this, placeholders::_1,
			placeholders::_2, spEndpoint, spBuf));
		//��������
		ContinueTesk();
		return true;
	}

	//udp�ر�
	void UdpClose(bool bListen)
	{
		//ֹͣ����
		StopTesk();
		//�ر�socket
		assert(udpSocket.is_open());
		asio::error_code err;
		udpSocket.close(err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket�ر��쳣: "+err.message()+"\r\n"));
		}
		if(bListen) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
				NewCstrLong("ֹͣudp����\r\n"));
		}
		else {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
				NewCstrLong("�ر�udp�ɹ�\r\n"));
		}
		//��������
		RestartTesk();
	}

private:
	//tcp��������
	void TcpAcceptFunc(asio::error_code err)
	{
		//ֹͣ��������
		if(err && err==asio::error::operation_aborted)
			return;
		//�����ж�
		if(err) {
			//֪ͨ������
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp��������ʧ��: "+err.message()+"\r\n"));
			::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
				(LPARAM)Action::tcpAcceptError);
			return;
		}
		//���ӳɹ�
		string message = tcpSocket.remote_endpoint().address().to_string()
			+':' +std::to_string(tcpSocket.remote_endpoint().port());
		::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("tcp�������ӳɹ�: "+message+"\r\n"));
		::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
			(LPARAM)Action::tcpAccept);
		//��ʼ��ȡ
		auto spBuf = std::make_shared<ReadBuf>();
		tcpSocket.async_read_some(asio::buffer(*spBuf),
			std::bind(&NetData::TcpReadFunc, this, placeholders::_1,
			placeholders::_2, spBuf));
	}

	//tcp��ȡ
	void TcpReadFunc(asio::error_code err, size_t size, ReadBufPtr spBuf)
	{
		//ֹͣ��������
		if(err && err==asio::error::operation_aborted)
			return;
		//�����ж�
		if(err && err!=asio::error::eof) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp������Ϣʧ��: "+err.message()+"\r\n"));
			//֪ͨ������
			::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
				(LPARAM)Action::tcpReadError);
			return;
		}
		//���յ���Ϣ
		if(size>0) {
			string str(spBuf->begin(), spBuf->begin()+size);
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
				NewCstrLong("tcp������Ϣ: "+StrBinToText(str)+"\r\n"));
		}
		//�Է��Ͽ�����
		if(err==asio::error::eof) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
				NewCstrLong("�Է��Ͽ�tcp����\r\n"));
			//֪ͨ������
			::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
				(LPARAM)Action::tcpReadEof);
		}
		//��������
		else {
			auto spBufNew = std::make_shared<ReadBuf>();
			tcpSocket.async_read_some(asio::buffer(*spBufNew),
				std::bind(&NetData::TcpReadFunc, this, placeholders::_1,
				placeholders::_2, spBufNew));
		}
	}

	//tcpд��
	void TcpWriteFunc(asio::error_code err, size_t size, WriteBufPtr spBuf)
	{
		//�����ж�
		if(err) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp������Ϣʧ��: "+err.message()+"\r\n"));
			return;
		}
		//������Ϣ�ɹ�
		string str(spBuf->begin(), spBuf->begin()+size);
		::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("tcp������Ϣ: "+StrBinToText(str)+"\r\n"));
	}

	//udp����
	void UdpRecvFunc(asio::error_code err, size_t size, UdpEndpointPtr spEndpoint,
		ReadBufPtr spBuf)
	{
		//ֹͣ��������
		if(err && err==asio::error::operation_aborted)
			return;
		//�����ж�
		if(err && err!=asio::error::message_size) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp������Ϣʧ��: "+err.message()+"\r\n"));
			//֪ͨ������
			::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
				(LPARAM)Action::udpRecvError);
			return;
		}
		//���յ���Ϣ
		string str(spBuf->begin(), spBuf->begin()+size);
		string strMsg = spEndpoint->address().to_string()
			+':' +std::to_string(spEndpoint->port());
		::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("udp������Ϣ("+strMsg+"): "+StrBinToText(str)+"\r\n"));
		//��������
		auto spEndpointNew = std::make_shared<UdpEndpoint>();
		auto spBufNew = std::make_shared<ReadBuf>();
		udpSocket.async_receive_from(asio::buffer(*spBufNew), *spEndpointNew,
			std::bind(&NetData::UdpRecvFunc, this, placeholders::_1,
			placeholders::_2, spEndpointNew, spBufNew));
	}

	//udp����
	void UdpSendFunc(asio::error_code err, size_t size, UdpEndpointPtr spEndpoint,
		WriteBufPtr spBuf)
	{
		//�����ж�
		if(err) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp������Ϣʧ��: "+err.message()+"\r\n"));
			return;
		}
		//������Ϣ�ɹ�
		string str(spBuf->begin(), spBuf->begin()+size);
		string strMsg = spEndpoint->address().to_string()
			+':' +std::to_string(spEndpoint->port());
		::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("udp������Ϣ("+strMsg+"): "+StrBinToText(str)+"\r\n"));
	}
};

