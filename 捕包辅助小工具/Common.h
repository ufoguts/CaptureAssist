#pragma once


//c��
#include <ctime>
#include <clocale>


//io���
#include <iostream>//c++io
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::ios_base;
using std::istream;
using std::ostream;
#include <sstream>//�ַ���io
using std::istringstream;
using std::ostringstream;
using std::stringstream;
#include <fstream>//�ļ�io
using std::ifstream;
using std::ofstream;
using std::fstream;
#include <iomanip>//�߼�io

//�������
#include <string>//�ַ���
using std::string;
using std::wstring;
#include <vector>//����
using std::vector;
#include <array>//����
using std::array;

//�������
#include <algorithm>
#include <functional>//����
using std::function;
using std::hash;
using std::reference_wrapper;
namespace placeholders = std::placeholders;

//���������
#include <memory>//�ڴ�
using std::shared_ptr;
using std::unique_ptr;
#include <thread>//�߳�
using std::thread;
namespace this_thread = std::this_thread;
#include <mutex>//����
using std::mutex;
using std::timed_mutex;
using std::lock_guard;
using std::unique_lock;
using std::defer_lock;
using std::try_to_lock;
using std::adopt_lock;
#include <condition_variable>//�߳�ͬ��
using std::condition_variable;
using std::condition_variable_any;
using std::cv_status;


/*��ǰ�����Ͱ汾�ο�·����
Boost 1.66.0
���Ӱ���Ŀ¼��
C:\Program Files\boost_1_66_0
���ӿ�Ŀ¼��
C:\Program Files\boost_1_66_0\stage\lib
���������
������
*/

//asio������io��
#include <boost/asio.hpp>
namespace boost { namespace asio { using ip::tcp; }; };
namespace boost { namespace asio { using ip::udp; }; };
namespace boost { namespace asio { using boost::system::error_code; }; };
namespace asio = boost::asio;


//��������
#include "TypeExtend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
