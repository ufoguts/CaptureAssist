#pragma once

//��׼��������չ����

#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>
#include <stdexcept>
#include <cassert>
#include <type_traits>
#include <mutex>
#include <clocale>
#include <algorithm>



//Ԥ������������չ

//duration��չ
namespace std
{
namespace chrono
{
typedef duration<long, ratio<3600*24>> days;
typedef duration<long, ratio<3600*24*7>> weeks;
}
}



//����������չ

//����ռλ����
template<int N>
class BlankType
{
};



//����ת����������

//c����ַ���ת���ַ���
inline std::string SzToStr(const char *sz)
{
	return std::string(sz);
}
inline std::string SzToStr(char *sz)
{
	return std::string(sz);
}
template<typename Ty>
inline Ty &&SzToStr(Ty &&arg)
{
	return std::forward<Ty>(arg);
}

//�޷�������ת���з��ţ������ܱ���ض�
inline constexpr int ToSigned(unsigned char num)
{
	return static_cast<int>(num);
}
inline constexpr int ToSigned(char num)
{
	return static_cast<int>(num);
}
inline constexpr long ToSigned(unsigned short num)
{
	return static_cast<long>(num);
}
inline constexpr long long ToSigned(unsigned num)
{
	return static_cast<long long>(num);
}
inline constexpr long long ToSigned(unsigned long num)
{
	return static_cast<long long>(num);
}
inline constexpr long long ToSigned(unsigned long long num)
{
	return static_cast<long long>(num);
}



//���ͳ�Ա��չ

//�����з����޷����ж�
#define _InterSignUnsign(Ty1, Ty2) \
	((std::is_integral<Ty1>::value && std::is_signed<Ty1>::value)\
	&& std::is_unsigned<Ty2>::value)
//�������Ͱ�ȫ�ж�С��
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty1, Ty2)>::type
>
inline constexpr bool SafeLT(Ty1 num1, Ty2 num2,
	BlankType<0> = BlankType<0>())
{
	return (num1<0) ? (true)
		: (static_cast<typename std::make_unsigned<Ty1>::type>(num1)<num2);
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeLT(Ty1 num1, Ty2 num2,
	BlankType<1> = BlankType<1>())
{
	return (num2<0) ? (false)
		: (num1<static_cast<typename std::make_unsigned<Ty2>::type>(num2));
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	std::is_fundamental<Ty1>::value && std::is_fundamental<Ty2>::value
	&& !_InterSignUnsign(Ty1, Ty2) && !_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeLT(Ty1 num1, Ty2 num2,
	BlankType<2> = BlankType<2>())
{
	return num1<num2;
}
//�������Ͱ�ȫ�ж�С�ڵ���
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty1, Ty2)>::type
>
inline constexpr bool SafeLTE(Ty1 num1, Ty2 num2,
	BlankType<0> = BlankType<0>())
{
	return (num1<0) ? (true)
		: (static_cast<typename std::make_unsigned<Ty1>::type>(num1)<=num2);
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeLTE(Ty1 num1, Ty2 num2,
	BlankType<1> = BlankType<1>())
{
	return (num2<0) ? (false)
		: (num1<=static_cast<typename std::make_unsigned<Ty2>::type>(num2));
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	std::is_fundamental<Ty1>::value && std::is_fundamental<Ty2>::value
	&& !_InterSignUnsign(Ty1, Ty2) && !_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeLTE(Ty1 num1, Ty2 num2,
	BlankType<2> = BlankType<2>())
{
	return num1<=num2;
}
//�������Ͱ�ȫ�жϴ���
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty1, Ty2)>::type
>
inline constexpr bool SafeGT(Ty1 num1, Ty2 num2,
	BlankType<0> = BlankType<0>())
{
	return (num1<0) ? (false)
		: (static_cast<typename std::make_unsigned<Ty1>::type>(num1)>num2);
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeGT(Ty1 num1, Ty2 num2,
	BlankType<1> = BlankType<1>())
{
	return (num2<0) ? (true)
		: (num1>static_cast<typename std::make_unsigned<Ty2>::type>(num2));
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	std::is_fundamental<Ty1>::value && std::is_fundamental<Ty2>::value
	&& !_InterSignUnsign(Ty1, Ty2) && !_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeGT(Ty1 num1, Ty2 num2,
	BlankType<2> = BlankType<2>())
{
	return num1>num2;
}
//�������Ͱ�ȫ�жϴ��ڵ���
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty1, Ty2)>::type
>
inline constexpr bool SafeGTE(Ty1 num1, Ty2 num2,
	BlankType<0> = BlankType<0>())
{
	return (num1<0) ? (false)
		: (static_cast<typename std::make_unsigned<Ty1>::type>(num1)>=num2);
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeGTE(Ty1 num1, Ty2 num2,
	BlankType<1> = BlankType<1>())
{
	return (num2<0) ? (true)
		: (num1>=static_cast<typename std::make_unsigned<Ty2>::type>(num2));
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	std::is_fundamental<Ty1>::value && std::is_fundamental<Ty2>::value
	&& !_InterSignUnsign(Ty1, Ty2) && !_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeGTE(Ty1 num1, Ty2 num2,
	BlankType<2> = BlankType<2>())
{
	return num1>=num2;
}
//�������Ͱ�ȫ�жϵ���
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty1, Ty2)>::type
>
inline constexpr bool SafeEQ(Ty1 num1, Ty2 num2,
	BlankType<0> = BlankType<0>())
{
	return (num1<0) ? (false)
		: (static_cast<typename std::make_unsigned<Ty1>::type>(num1)==num2);
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeEQ(Ty1 num1, Ty2 num2,
	BlankType<1> = BlankType<1>())
{
	return (num2<0) ? (false)
		: (num1==static_cast<typename std::make_unsigned<Ty2>::type>(num2));
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	std::is_fundamental<Ty1>::value && std::is_fundamental<Ty2>::value
	&& !_InterSignUnsign(Ty1, Ty2) && !_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeEQ(Ty1 num1, Ty2 num2,
	BlankType<2> = BlankType<2>())
{
	return num1==num2;
}
//�������Ͱ�ȫ�жϲ�����
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty1, Ty2)>::type
>
inline constexpr bool SafeNEQ(Ty1 num1, Ty2 num2,
	BlankType<0> = BlankType<0>())
{
	return (num1<0) ? (true)
		: (static_cast<typename std::make_unsigned<Ty1>::type>(num1)!=num2);
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeNEQ(Ty1 num1, Ty2 num2,
	BlankType<1> = BlankType<1>())
{
	return (num2<0) ? (true)
		: (num1!=static_cast<typename std::make_unsigned<Ty2>::type>(num2));
}
template<typename Ty1, typename Ty2, typename TyEnable= typename std::enable_if<
	std::is_fundamental<Ty1>::value && std::is_fundamental<Ty2>::value
	&& !_InterSignUnsign(Ty1, Ty2) && !_InterSignUnsign(Ty2, Ty1)>::type
>
inline constexpr bool SafeNEQ(Ty1 num1, Ty2 num2,
	BlankType<2> = BlankType<2>())
{
	return num1!=num2;
}
#undef _InterSignUnsign

//string���ۼӲ�����ʹ�����������
template<typename Ty>
inline std::string &operator <<(std::string &str, Ty &&arg)
{
	return str += std::forward<Ty>(arg);
}
template<typename Ty>
inline std::string &operator <<(std::wstring &str, Ty &&arg)
{
	return str += std::forward<Ty>(arg);
}

//hash��������
namespace std
{
//��pair������
template<typename Ty1, typename Ty2>
struct hash<std::pair<Ty1, Ty2>>
{
	typedef size_t result_type;
	typedef std::pair<Ty1, Ty2> argument_type;
	size_t operator()(const std::pair<Ty1, Ty2> &pr) const
	{
		return std::hash<Ty1>()(pr.first)
			^ std::hash<Ty2>()(pr.second);
	}
};
//��tuple������
template<typename ...Tys>
struct hash<std::tuple<Tys...>>
{
	typedef size_t result_type;
	typedef std::tuple<Tys...> argument_type;
	size_t operator()(const std::tuple<Tys...> &tup) const
	{
		return Hash<sizeof...(Tys)-1>(tup);
	}
	template<int index,
		typename TyEnble = typename std::enable_if<(index>=0)>::type
	>
		static size_t Hash(const std::tuple<Tys...> &tup)
	{
		return std::hash<typename std::tuple_element<index, std::tuple<Tys...>>::type>
			()(std::get<index>(tup))
			^ Hash<index-1>(tup);
	}
	template<int index, typename Ty>
	static size_t Hash(const Ty &tup)
	{
		return 0;
	}
};
}

//�ַ������ж�
inline bool CharIsNum(char ch)
{
	return ch>='0' && ch<='9';
}
inline bool CharIsLetter(char ch)
{
	return (ch>='a' && ch<='z') || (ch>='A' && ch<='Z');
}
inline bool CharIsNumLetter(char ch)
{
	return (ch>='0' && ch<='9')
		|| (ch>='a' && ch<='z') || (ch>='A' && ch<='Z');
}
inline bool CharIsIdentifier(char ch)
{
	return (ch>='0' && ch<='9')
		|| (ch>='a' && ch<='z') || (ch>='A' && ch<='Z')
		|| (ch=='_');
}
inline bool CharIsHex(char ch)
{
	return (ch>='0' && ch<='9')
		|| (ch>='a' && ch<='f') || (ch>='A' && ch<='F')
		|| (ch=='_');
}
//�ַ�ת��
inline int CharToNum(char ch)
{
	if(ch>='0' && ch<='9')
		return ch-'0';
	else
		return -1;
}
inline int CharToLetter(char ch)
{
	if(ch>='a' && ch<='z')
		return ch-'a';
	else if(ch>='A' && ch<='Z')
		return ch-'A';
	else
		return -1;
}
inline int CharToHex(char ch)
{
	if(ch>='0' && ch<='9')
		return ch-'0';
	else if(ch>='a' && ch<='z')
		return ch-'a'+10;
	else if(ch>='A' && ch<='Z')
		return ch-'A'+10;
	else
		return -1;
}
//�ַ���ת��
inline char NumToChar(int i)
{
	if(i>=0 && i<=9)
		return i+'0';
	else
		return 0;
}
inline char LetterToCapitalChar(int i)
{
	if(i>=0 && i<=25)
		return i+'A';
	else
		return 0;
}
inline char LetterToLittleChar(int i)
{
	if(i>=0 && i<=25)
		return i+'a';
	else
		return 0;
}
inline char HexToCapitalChar(int i)
{
	if(i>=0 && i<=9)
		return i+'0';
	else if(i>=10 && i<=15)
		return i+'A';
	else
		return 0;
}
inline char HexToLittleChar(int i)
{
	if(i>=0 && i<=9)
		return i+'0';
	else if(i>=10 && i<=15)
		return i+'a';
	else
		return 0;
}

//��ֵ�Ƚ����
template<typename Ty1, typename Ty2>
inline constexpr bool MultiEQ(Ty1 &&arg1, Ty2 &&arg2)
{
	return std::forward<Ty1>(arg1)==std::forward<Ty2>(arg2);
}
template<typename Ty1, typename Ty2, typename ...Tys>
inline constexpr bool MultiEQ(Ty1 &&arg1, Ty2 &&arg2, Tys &&...args)
{
	return std::forward<Ty1>(arg1)==std::forward<Ty2>(arg2)
		&& MultiEQ(std::forward<Tys>(args)...);
}
//��ֵ�Ƚ�С��
template<typename Ty1, typename Ty2>
inline constexpr bool MultiLT(Ty1 &&arg1, Ty2 &&arg2)
{
	return std::forward<Ty1>(arg1)<std::forward<Ty2>(arg2);
}
template<typename Ty1, typename Ty2, typename ...Tys>
inline constexpr bool MultiLT(Ty1 &&arg1, Ty2 &&arg2, Tys &&...args)
{
	return std::forward<Ty1>(arg1)<std::forward<Ty2>(arg2)
		|| (std::forward<Ty1>(arg1)==std::forward<Ty2>(arg2)
		&& MultiLT(std::forward<Tys>(args)...));
}



//���߹���ת������

//��tmת��Ϊ��ʽ���ַ���
template<typename Ty= BlankType<0>>
inline std::string TimeToStr(const std::tm &time,
	const std::string &fmt="%F %T")
{
	char buf[256];
	std::string str;
	if(strftime(buf, sizeof(buf), fmt.c_str(), &time)) {
		str = buf;
	}
	return str;
}
//��time_tת��Ϊ��ʽ���ַ���
template<typename Ty= BlankType<0>>
inline std::string TimeToStr(std::time_t time,
	const std::string &fmt="%F %T", bool bThdSafe= false)
{
	static std::mutex s_mtx;
	if(time>=(1LL<<31))
		time = (1LL<<31)-1;
	else if(time<0)
		time = 0;
	if(bThdSafe)
		s_mtx.lock();
	std::tm *pTimeStu = std::localtime(&time);
	string str = TimeToStr(*pTimeStu, fmt);
	if(bThdSafe)
		s_mtx.unlock();
	return str;
}
//��time_pointת��Ϊ��ʽ���ַ���
template<typename Ty= BlankType<0>>
inline std::string TimeToStr(
	const std::chrono::time_point<std::chrono::system_clock> &time,
	const std::string &fmt="%F %T", bool bThdSafe= false)
{
	return TimeToStr(std::chrono::system_clock::to_time_t(time), fmt, bThdSafe);
}
//����ǰʱ��ת��Ϊ�ַ���
template<typename Ty= BlankType<0>>
inline std::string NowTimeToStr(const std::string &fmt="%F %T",
	bool bThdSafe= false)
{
	return TimeToStr(std::chrono::system_clock::now(), fmt, bThdSafe);
}

//�ַ���ת�����ָ���
inline void _FromStringAssist(const std::string str, int &num)
{
	num = std::stoi(str);
}
inline void _FromStringAssist(const std::string str, long &num)
{
	num = std::stol(str);
}
inline void _FromStringAssist(const std::string str, long long &num)
{
	num = std::stoll(str);
}
inline void _FromStringAssist(const std::string str, unsigned &num)
{
	num = std::stoul(str);
}
inline void _FromStringAssist(const std::string str, unsigned long &num)
{
	num = std::stoul(str);
}
inline void _FromStringAssist(const std::string str, unsigned long long &num)
{
	num = std::stoull(str);
}
inline void _FromStringAssist(const std::string str, float &num)
{
	num = std::stof(str);
}
inline void _FromStringAssist(const std::string str, double &num)
{
	num = std::stod(str);
}
inline void _FromStringAssist(const std::string str, long double &num)
{
	num = std::stold(str);
}
//�ַ���ת�����֣����쳣
template<typename Ty>
inline bool FromString(const std::string str, Ty &num)
{
	bool ret = true;
	try {
		_FromStringAssist(str, num);
	}
	catch(std::invalid_argument &) {
		ret = false;
		num = 0;
	}
	catch(std::out_of_range &) {
		ret = false;
		num = 0;
	}
	return ret;
}
template<typename Ty, typename TyIter>
inline bool FromString(TyIter itSt, TyIter itEd, Ty &num)
{
	return FromString(std::string(itSt, itEd), num);
}

//��������ת���ַ���������ǧλ�ָ���
template<typename Ty, typename TyEnable= typename std::enable_if<
	std::is_integral<Ty>::value>::type
>
	inline std::string GapToString(Ty num, int gap= 3, char ch= '\'')
{
	std::string str;
	//ȡ��
	if(SafeLT(num, 0))
		num = -num;
	//�����ַ���
	for(int i=0; ; ++i) {
		if(i!=0 && i%gap==0)
			str += ch;
		str += num%10+'0';
		num /= 10;
		if(num==0)
			break;
	}
	//���븺��
	if(SafeLT(num, 0))
		str += '-';
	//��ת
	std::reverse(str.begin(), str.end());
	return str;
}

//�ַ����Ϳ��ַ���ת��
template<typename Ty= BlankType<0>>
inline bool StrToWstr(std::wstring &wstr, const std::string &str,
	const std::string local= "chs", bool bThdSafe= false)
{
	static std::mutex s_mtx;
	//���ַ����ռ�
	wstr.assign(str.size(), '\0');
	if(bThdSafe)
		s_mtx.lock();
	//���ñ��ػ���
	std::string strOri = std::setlocale(LC_CTYPE, nullptr);
	std::setlocale(LC_CTYPE, local.c_str());
	//����ת��
	size_t res = mbstowcs(&wstr[0], &str[0], wstr.size());
	std::setlocale(LC_CTYPE, strOri.c_str());
	if(bThdSafe)
		s_mtx.unlock();
	//���ɹ�
	if(res!=(size_t)(-1)) {
		wstr.resize(res);
		return true;
	}
	//��ʧ��
	else {
		wstr.empty();
		return false;
	}
}
template<typename Ty= BlankType<0>>
inline bool WstrToStr(std::string &str, const std::wstring &wstr,
	const std::string local= "chs", bool bThdSafe= false)
{
	static std::mutex s_mtx;
	//���ַ����ռ�
	str.assign(wstr.size()*4, '\0');
	if(bThdSafe)
		s_mtx.lock();
	//���ñ��ػ���
	std::string strOri = std::setlocale(LC_CTYPE, nullptr);
	std::setlocale(LC_CTYPE, local.c_str());
	//����ת��
	size_t res = wcstombs(&str[0], &wstr[0], str.size());
	std::setlocale(LC_CTYPE, strOri.c_str());
	if(bThdSafe)
		s_mtx.unlock();
	//���ɹ�
	if(res!=(size_t)(-1)) {
		str.resize(res);
		return true;
	}
	//��ʧ��
	else {
		str.empty();
		return false;
	}
}

//�������ַ����ı���ʾ
inline std::string StrBinToText(const std::string &str)
{
	std::string strRet = "\"";
	//��ÿ���ַ��ж�
	for(unsigned ch : str) {
		//ascii
		if(ch>=0x00 && ch<=0x7F) {
			//ת���ַ�
			if(ch=='\r')
				strRet += "\\r";
			else if(ch=='\n')
				strRet += "\\n";
			else if(ch=='\t')
				strRet += "\\t";
			else if(ch=='\a')
				strRet += "\\a";
			else if(ch=='\b')
				strRet += "\\b";
			else if(ch=='\f')
				strRet += "\\f";
			else if(ch=='\v')
				strRet += "\\v";
			else if(ch=='\'')
				strRet += "\\\'";
			else if(ch=='\"')
				strRet += "\\\"";
			else if(ch=='\\')
				strRet += "\\\\";
			//�ɴ�ӡ�ַ�
			else if(ch>=0x20 && ch<=0x7E)
				strRet += ch;
			//���ɴ�ӡ�ַ�
			else {
				strRet <<"\\x" <<HexToCapitalChar(ch/16) <<HexToCapitalChar(ch%16);
			}
		}
		//��ascii
		else {
			strRet += ch;
		}
	}
	strRet += '\"';
	return strRet;
}
//ת���ַ�����������ʾ
inline std::string StrTextToBin(const std::string &str)
{
	std::string strRet;
	//��ÿ���ַ��ж�
	for(unsigned i=0; i<str.size(); ++i) {
		//ascii
		if(str[i]>=0x00 && str[i]<=0x7F) {
			//��β����
			if((i==0 || i==str.size()-1) && (str[i]=='\"' || str[i]=='\''))
				continue;
			//ת���ַ�
			if(str[i]=='\\') {
				++ i;
				//������ת��
				if(i<str.size()) {
					if(str[i]=='r')
						strRet += "\r";
					else if(str[i]=='n')
						strRet += "\n";
					else if(str[i]=='t')
						strRet += "\t";
					else if(str[i]=='a')
						strRet += "\a";
					else if(str[i]=='b')
						strRet += "\b";
					else if(str[i]=='f')
						strRet += "\f";
					else if(str[i]=='v')
						strRet += "\v";
					else if(str[i]=='\'')
						strRet += "\'";
					else if(str[i]=='\"')
						strRet += "\"";
					else if(str[i]=='\\')
						strRet += "\\";
					//16��������ת��
					else if(str[i]=='x') {
						if(i+2<str.size() && CharIsHex(str[i+1]) && CharIsHex(str[i+2]))
							strRet += CharToHex(str[i+1])*16+CharToHex(str[i+2]);
						i+=2;
					}
					//�޷�ʶ��ת��
					else
						strRet += str[i];
				}
				else
					break;
			}
			//��ת��
			else
				strRet += str[i];
		}
		//��ascii
		else {
			strRet += str[i];
		}
	}
	return strRet;
}
