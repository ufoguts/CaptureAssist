#pragma once

#include "捕包辅助小工具Dlg.h"


//网络处理数据体
class NetData
{
public:
	//回传主窗口信息消息
	enum class Action
	{
		tcpAccept,
		tcpAcceptError,
		tcpReadEof,
		tcpReadError,
		udpRecvError,
	};
	//延长生存期类型
	using ReadBuf = array<char, 256>;
	using ReadBufPtr = shared_ptr<ReadBuf>;
	using WriteBuf = string;
	using WriteBufPtr = shared_ptr<WriteBuf>;
	using UdpEndpoint = asio::udp::endpoint;
	using UdpEndpointPtr = shared_ptr<UdpEndpoint>;

private:
	HWND mainWnd;//主窗口句柄
	bool bExit;//退出条件
	mutex mtx;//互斥锁
	condition_variable_any cv;//同步
	bool bWork;//工作条件
	asio::io_context ioContext;//io实例
	asio::tcp::acceptor acceptor;//tcp接收器
	asio::tcp::socket tcpSocket;//tcp
	asio::udp::socket udpSocket;//udp

public:
	//构造
	NetData():
		bExit(false), bWork(false),
		acceptor(ioContext), tcpSocket(ioContext), udpSocket(ioContext)
	{
	}
	//析构
	~NetData()
		= default;
	//后续初始化
	void Init(HWND o_mainWnd)
	{
		mainWnd = o_mainWnd;
	}

public:
	//线程函数
	void ThdFunc()
	{
		//主体循环
		while(true) {
			//互斥锁内判断退出条件
			mtx.lock();
			//进入等待状态
			cv.wait(mtx, [this]() {return bWork || bExit; });
			if(bExit) {
				mtx.unlock();
				break;
			}
			//进入工作状态
			bWork = false;
			mtx.unlock();
			ioContext.run();
		}
	}

public:
	//添加任务，主线程调用
	void AddTesk()
	{
		mtx.lock();
		bWork = true;
		mtx.unlock();
		cv.notify_one();
	}
	//停止任务，主线程调用
	void StopTesk()
	{
		ioContext.stop();
		mtx.lock();
		bWork = false;
		mtx.unlock();
		ioContext.run();
	}
	//继续任务，主线程调用
	void ContinueTesk()
	{
		ioContext.restart();
		AddTesk();
	}
	//清空重启任务，主线程调用
	void RestartTesk()
	{
		ioContext.restart();
		ioContext.run();
		ioContext.restart();
	}
	//退出，主线程调用
	void Exit()
	{
		mtx.lock();
		ioContext.stop();
		bExit = true;
		mtx.unlock();
		cv.notify_one();
	}

	//tcp监听，主线程调用
	bool TcpListen(int port)
	{
		//打开绑定监听
		asio::tcp::endpoint endpoint(asio::tcp::v4(), port);
		asio::error_code err;
		assert(!acceptor.is_open());
		acceptor.open(asio::tcp::v4(), err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("接收器打开失败: "+err.message()+"\r\n"));
			acceptor.close();
			return false;
		}
		acceptor.bind(endpoint, err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("接收器绑定失败: "+err.message()+"\r\n"));
			acceptor.close();
			return false;
		}
		acceptor.listen(asio::socket_base::max_listen_connections, err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("接收器监听失败: "+err.message()+"\r\n"));
			acceptor.close();
			return false;
		}
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("开始tcp监听:"+std::to_string(port)+"\r\n"));
		//开启异步连接
		assert(!tcpSocket.is_open());
		acceptor.async_accept(tcpSocket,
			std::bind(&NetData::TcpAcceptFunc, this, placeholders::_1));
		//添加任务
		AddTesk();
		return true;
	}

	//tcp连接，主线程调用
	bool TcpConnect(const asio::ip::address_v4 &ipv4, int port)
	{
		assert(!tcpSocket.is_open());
		asio::error_code err;
		//进行连接
		tcpSocket.connect(asio::tcp::endpoint(ipv4, port), err);
		//错误判断
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp连接失败: "+err.message()+"\r\n"));
			tcpSocket.close();
			return false;
		}
		string strMsg = '(' +std::to_string(tcpSocket.local_endpoint().port())
			+")-" +ipv4.to_string() +':' +std::to_string(port);
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("tcp连接成功: "+strMsg+"\r\n"));
		//开始异步读取
		auto spBuf = std::make_shared<ReadBuf>();
		tcpSocket.async_read_some(asio::buffer(*spBuf),
			std::bind(&NetData::TcpReadFunc, this, placeholders::_1,
			placeholders::_2, spBuf));
		//添加任务
		AddTesk();
		return true;
	}

	//tcp写入，主线程调用
	bool TcpWrite(const string &message)
	{
		//停止任务
		StopTesk();
		//判断开启
		if(!tcpSocket.is_open()) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp发送失败: socket已关闭\r\n"));
			return false;
		}
		//异步发送
		auto spBuf = std::make_shared<WriteBuf>(message);
		asio::async_write(tcpSocket, asio::buffer(*spBuf),
			std::bind(&NetData::TcpWriteFunc, this, placeholders::_1,
			placeholders::_2, spBuf));
		//继续任务
		ContinueTesk();
		return true;
	}

	//tcp断开连接，是否继续监听，是否需要停止，主线程调用
	void TcpDisConnect(bool bListen, bool bStop= true)
	{
		//停止任务
		if(bStop)
			StopTesk();
		assert(tcpSocket.is_open());
		//关闭连接
		asio::error_code err;
		//tcpSocket.cancel(err);
		//if(err) {
		//	::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
		//		NewCstrLong("tcp socket取消异常: "+err.message()+"\r\n"));
		//}
		tcpSocket.close(err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp socket关闭异常: "+err.message()+"\r\n"));
		}
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("主动断开tcp连接\r\n"));
		//继续异步连接
		if(bListen) {
			acceptor.async_accept(tcpSocket,
				std::bind(&NetData::TcpAcceptFunc, this, placeholders::_1));
			//监听则继续任务
			if(bStop)
				ContinueTesk();
		}
		else {
			//非监听则重启任务
			if(bStop)
				RestartTesk();
		}
	}

	//tcp停止监听，主线程调用
	void TcpDisListen()
	{
		//停止任务
		StopTesk();
		//关闭连接
		if(tcpSocket.is_open()) {
			TcpDisConnect(false, false);
		}
		//停止监听
		asio::error_code err;
		assert(acceptor.is_open());
		acceptor.cancel(err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("接收器取消异常: "+err.message()+"\r\n"));
		}
		acceptor.close(err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("接收器关闭异常: "+err.message()+"\r\n"));
		}
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("停止tcp监听\r\n"));
		//重启任务
		RestartTesk();
	}

	//udp监听，主线程调用
	bool UdpListen(int port)
	{
		asio::error_code err;
		//开启端口
		assert(!udpSocket.is_open());
		udpSocket.open(asio::udp::v4(), err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket打开失败: "+err.message()+"\r\n"));
			udpSocket.close();
			return false;
		}
		udpSocket.bind(asio::udp::endpoint(asio::udp::v4(), port), err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket绑定失败: "+err.message()+"\r\n"));
			udpSocket.close();
			return false;
		}
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("开始udp监听: "+std::to_string(port)+"\r\n"));
		//开启异步接收
		auto spEndpoint = std::make_shared<UdpEndpoint>();
		auto spBuf = std::make_shared<ReadBuf>();
		udpSocket.async_receive_from(asio::buffer(*spBuf), *spEndpoint,
			std::bind(&NetData::UdpRecvFunc, this, placeholders::_1,
			placeholders::_2, spEndpoint, spBuf));
		//添加任务
		AddTesk();
		return true;
	}

	//udp触发，主线程调用
	bool UdpOpen(const asio::ip::address_v4 &ipv4, int port,
		const string &message)
	{
		asio::error_code err;
		//开启端口
		assert(!udpSocket.is_open());
		udpSocket.open(asio::udp::v4(), err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket打开失败: "+err.message()+"\r\n"));
			udpSocket.close();
			return false;
		}
		//发送首条消息
		size_t size = udpSocket.send_to(asio::buffer(message),
			asio::udp::endpoint(ipv4, port), 0, err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket发送失败: "+err.message()+"\r\n"));
			udpSocket.close();
			return false;
		}
		//发送消息成功
		int portNew = udpSocket.local_endpoint().port();
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("打开udp成功: "+std::to_string(portNew)+"\r\n"));
		string str(message.begin(), message.begin()+size);
		string strMsg = ipv4.to_string() +':' +std::to_string(port);
		::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("udp触发("+strMsg+"): "+StrBinToText(str)+"\r\n"));
		//开启异步接收
		auto spEndpoint = std::make_shared<UdpEndpoint>();
		auto spBuf = std::make_shared<ReadBuf>();
		udpSocket.async_receive_from(asio::buffer(*spBuf), *spEndpoint,
			std::bind(&NetData::UdpRecvFunc, this, placeholders::_1,
			placeholders::_2, spEndpoint, spBuf));
		//添加任务
		AddTesk();
		return true;
	}

	//udp发送，主线程调用
	bool UdpSend(const asio::ip::address_v4 &ipv4, int port,
		const string &message)
	{
		//停止任务
		StopTesk();
		//判断开启
		if(!udpSocket.is_open()) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp发送失败: socket已关闭\r\n"));
			return false;
		}
		//异步发送
		auto spEndpoint = std::make_shared<UdpEndpoint>(ipv4, port);
		auto spBuf = std::make_shared<WriteBuf>(message);
		udpSocket.async_send_to(asio::buffer(*spBuf), *spEndpoint,
			std::bind(&NetData::UdpSendFunc, this, placeholders::_1,
			placeholders::_2, spEndpoint, spBuf));
		//继续任务
		ContinueTesk();
		return true;
	}

	//udp关闭
	void UdpClose(bool bListen)
	{
		//停止任务
		StopTesk();
		//关闭socket
		assert(udpSocket.is_open());
		asio::error_code err;
		udpSocket.close(err);
		if(err) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp socket关闭异常: "+err.message()+"\r\n"));
		}
		if(bListen) {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
				NewCstrLong("停止udp监听\r\n"));
		}
		else {
			::SendMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
				NewCstrLong("关闭udp成功\r\n"));
		}
		//重启任务
		RestartTesk();
	}

private:
	//tcp接受连接
	void TcpAcceptFunc(asio::error_code err)
	{
		//停止工作错误
		if(err && err==asio::error::operation_aborted)
			return;
		//错误判断
		if(err) {
			//通知主窗口
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp监听连接失败: "+err.message()+"\r\n"));
			::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
				(LPARAM)Action::tcpAcceptError);
			return;
		}
		//连接成功
		string message = tcpSocket.remote_endpoint().address().to_string()
			+':' +std::to_string(tcpSocket.remote_endpoint().port());
		::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("tcp监听连接成功: "+message+"\r\n"));
		::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
			(LPARAM)Action::tcpAccept);
		//开始读取
		auto spBuf = std::make_shared<ReadBuf>();
		tcpSocket.async_read_some(asio::buffer(*spBuf),
			std::bind(&NetData::TcpReadFunc, this, placeholders::_1,
			placeholders::_2, spBuf));
	}

	//tcp读取
	void TcpReadFunc(asio::error_code err, size_t size, ReadBufPtr spBuf)
	{
		//停止工作错误
		if(err && err==asio::error::operation_aborted)
			return;
		//错误判断
		if(err && err!=asio::error::eof) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp接收消息失败: "+err.message()+"\r\n"));
			//通知主窗口
			::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
				(LPARAM)Action::tcpReadError);
			return;
		}
		//接收到消息
		if(size>0) {
			string str(spBuf->begin(), spBuf->begin()+size);
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
				NewCstrLong("tcp接收消息: "+StrBinToText(str)+"\r\n"));
		}
		//对方断开连接
		if(err==asio::error::eof) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
				NewCstrLong("对方断开tcp连接\r\n"));
			//通知主窗口
			::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
				(LPARAM)Action::tcpReadEof);
		}
		//继续接收
		else {
			auto spBufNew = std::make_shared<ReadBuf>();
			tcpSocket.async_read_some(asio::buffer(*spBufNew),
				std::bind(&NetData::TcpReadFunc, this, placeholders::_1,
				placeholders::_2, spBufNew));
		}
	}

	//tcp写入
	void TcpWriteFunc(asio::error_code err, size_t size, WriteBufPtr spBuf)
	{
		//错误判断
		if(err) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("tcp发送消息失败: "+err.message()+"\r\n"));
			return;
		}
		//发送消息成功
		string str(spBuf->begin(), spBuf->begin()+size);
		::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("tcp发送消息: "+StrBinToText(str)+"\r\n"));
	}

	//udp接收
	void UdpRecvFunc(asio::error_code err, size_t size, UdpEndpointPtr spEndpoint,
		ReadBufPtr spBuf)
	{
		//停止工作错误
		if(err && err==asio::error::operation_aborted)
			return;
		//错误判断
		if(err && err!=asio::error::message_size) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp接收消息失败: "+err.message()+"\r\n"));
			//通知主窗口
			::PostMessage(mainWnd, WM_NET_THREAD_ACTION, 0,
				(LPARAM)Action::udpRecvError);
			return;
		}
		//接收到消息
		string str(spBuf->begin(), spBuf->begin()+size);
		string strMsg = spEndpoint->address().to_string()
			+':' +std::to_string(spEndpoint->port());
		::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("udp接收消息("+strMsg+"): "+StrBinToText(str)+"\r\n"));
		//继续接收
		auto spEndpointNew = std::make_shared<UdpEndpoint>();
		auto spBufNew = std::make_shared<ReadBuf>();
		udpSocket.async_receive_from(asio::buffer(*spBufNew), *spEndpointNew,
			std::bind(&NetData::UdpRecvFunc, this, placeholders::_1,
			placeholders::_2, spEndpointNew, spBufNew));
	}

	//udp发送
	void UdpSendFunc(asio::error_code err, size_t size, UdpEndpointPtr spEndpoint,
		WriteBufPtr spBuf)
	{
		//错误判断
		if(err) {
			::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 1,
				NewCstrLong("udp发送消息失败: "+err.message()+"\r\n"));
			return;
		}
		//发送消息成功
		string str(spBuf->begin(), spBuf->begin()+size);
		string strMsg = spEndpoint->address().to_string()
			+':' +std::to_string(spEndpoint->port());
		::PostMessage(mainWnd, WM_NET_EDIT_LOGOUT, 0,
			NewCstrLong("udp发送消息("+strMsg+"): "+StrBinToText(str)+"\r\n"));
	}
};

