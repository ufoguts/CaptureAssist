#pragma once


//c库
#include <ctime>
#include <clocale>


//io相关
#include <iostream>//c++io
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::ios_base;
using std::istream;
using std::ostream;
#include <sstream>//字符串io
using std::istringstream;
using std::ostringstream;
using std::stringstream;
#include <fstream>//文件io
using std::ifstream;
using std::ofstream;
using std::fstream;
#include <iomanip>//高级io

//容器相关
#include <string>//字符串
using std::string;
using std::wstring;
#include <vector>//向量
using std::vector;
#include <array>//数组
using std::array;

//泛型相关
#include <algorithm>
#include <functional>//函数
using std::function;
using std::hash;
using std::reference_wrapper;
namespace placeholders = std::placeholders;

//工具类相关
#include <memory>//内存
using std::shared_ptr;
using std::unique_ptr;
#include <thread>//线程
using std::thread;
namespace this_thread = std::this_thread;
#include <mutex>//互斥
using std::mutex;
using std::timed_mutex;
using std::lock_guard;
using std::unique_lock;
using std::defer_lock;
using std::try_to_lock;
using std::adopt_lock;
#include <condition_variable>//线程同步
using std::condition_variable;
using std::condition_variable_any;
using std::cv_status;


/*当前主机和版本参考路径：
Boost 1.66.0
附加包含目录：
C:\Program Files\boost_1_66_0
附加库目录：
C:\Program Files\boost_1_66_0\stage\lib
附加依赖项：
环境：
*/

//asio，网络io库
#include <boost/asio.hpp>
namespace boost { namespace asio { using ip::tcp; }; };
namespace boost { namespace asio { using ip::udp; }; };
namespace boost { namespace asio { using boost::system::error_code; }; };
namespace asio = boost::asio;


//公共包含
#include "TypeExtend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
