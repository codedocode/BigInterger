#pragma once
#include <vector>
#include <exception>
#include <string>
using namespace std;
class BigInterger
{
public:
	BigInterger(string val = "0");
	BigInterger(const BigInterger& bi);
	BigInterger& operator=(const BigInterger& bi);
	BigInterger& operator==(const BigInterger& bi);
	//四则运算
	BigInterger& operator+(const BigInterger& bi);
	BigInterger& operator-(const BigInterger& bi);
	BigInterger& operator*(const BigInterger& bi);
	BigInterger& operator/(const BigInterger& bi);
	BigInterger& operator/(const BigInterger& bi);
	BigInterger& operator+=(const BigInterger& bi);
	BigInterger& operator-=(const BigInterger& bi);
	BigInterger& operator*=(const BigInterger& bi);
	BigInterger& operator/=(const BigInterger& bi);
	//逻辑运算
	BigInterger& operator&(const BigInterger& bi);
	BigInterger& operator|(const BigInterger& bi);
	BigInterger& operator&=(const BigInterger& bi);
	BigInterger& operator|=(const BigInterger& bi);

	string toString();
private:
	string changeRadix(unsigned int val, int radix);
	void destructiveMulAdd(vector<unsigned int>& v, int sr, int gv);
	int parseInt(string val){
		int ret = 0;
		for(int i = 0; i < val.size(); ++i)
			ret = ret * 10 + val[i] - '0';
		return ret;
	}
	void changeMag(const BigInterger& bi){
		if(mag == bi.mag && signum == bi.signum) 
			return ;
		mag.clear();
		signum = bi.signum;
		mag = bi.mag;
	}
	void swap(char& a, char& b){
		char c = a;
		a = b;
		b = c;
	}
	void delZeroInts(){
		while(!mag.empty() && mag[0] == 0)
			mag.erase(mag.begin());
	} 
private:
	vector<unsigned int> mag;	//用于承载大数的数组
	int signum;	//存储符号位
	/* 每一个unsigned int类型的数最多存放9位十进制的数据 */
	static const int digitsPerInt = 9;
	/* 十进制的数的每一位要用几位二进制来表示 */
	static const int bitsPerDigit = 4;
	/* 9位十进制能表示的最大的数字 */
	static const int intRadix = 1e9;
};
string BigInterger::changeRadix(unsigned int val, int radix)
{
	string str;
	while(val != 0){
		int num = val % radix;
		val /= radix;
		char tmp;
		if(num > 10) tmp = 'a' + num - 10;
		else tmp = '0' + tmp;
		str.push_back(tmp);
	}
	int i = 0, j = str.size() - 1;
	while(i < j) swap(str[i++], str[j--]);
	return str;
}
BigInterger::BigInterger(string val)
{
	int cursor = 0, numDigits;	//0的个数和有效数字的个数
	int len = val.size();	//字符串长度
	if(len == 0) 
		throw exception("Zero length BigInteger");
	//判断正负性
	signum = 1;	//正数
	int sigloc = val.find_last_of('-');
	if(sigloc != -1){
		if(sigloc == 0){
			if(val.size() == 1)
				throw exception("Zero length BigInteger");
			signum = -1;	//负数
			cursor = 1;
		}
		else{
			throw exception("Illegal embedded minus sign");
		}
	}
	//跳过前导0
	while(cursor < len && val[cursor] == '0') cursor++;
	if(cursor == len){
		signum = 0;
		mag.push_back(0);
		return ;
	}
	else{
		numDigits = len - cursor;	//有效数字的长度
	}
	/* 换算成二进制需要多少位，每一个十进制都需要最多四位 */
	int numBits = numDigits * bitsPerDigit;
	mag.resize((numBits + 31) >> 5);	//mag数组的大小
	int firstLen = numDigits % digitsPerInt;	//最高位多出来的数先处理（例如长度13，则先处理最高的4字数字）
	if(firstLen == 0)	//刚好整除
		firstLen = digitsPerInt;
	string group = val.substr(cursor, cursor += firstLen);
	mag[mag.size() - 1] = parseInt(group);
	/*
	if(mag.back() < 0)
		throw exception("Illegal digit");
	*/
	int groupVal = 0;
	while(cursor < val.size()){
		group = val.substr(cursor, cursor += digitsPerInt);
		groupVal = parseInt(group);
		if(groupVal < 0)
			throw exception("Illegal digit");
		destructiveMulAdd(mag, intRadix, groupVal);
	}
	/* 去除前导0 */
	delZeroInts();
}
BigInterger::BigInterger(const BigInterger& bi)
{
	changeMag(bi);
}
BigInterger& BigInterger::operator=(const BigInterger& bi)
{
	changeMag(bi);
	return *this;
}
BigInterger& BigInterger::operator==(const BigInterger& bi)
{
	
}
//四则运算
BigInterger& BigInterger::operator+(const BigInterger& bi);
{

}
BigInterger& BigInterger::operator-(const BigInterger& bi);
{

}
BigInterger& BigInterger::operator*(const BigInterger& bi);
{}
BigInterger& BigInterger::operator/(const BigInterger& bi);
{}
BigInterger& BigInterger::operator/(const BigInterger& bi);
{}
BigInterger& BigInterger::operator+=(const BigInterger& bi);
{}
BigInterger& BigInterger::operator-=(const BigInterger& bi);
{}
BigInterger& BigInterger::operator*=(const BigInterger& bi);
{}
BigInterger& BigInterger::operator/=(const BigInterger& bi);
{}
//逻辑运算
BigInterger& BigInterger::operator&(const BigInterger& bi);
{}
BigInterger& BigInterger::operator|(const BigInterger& bi);
{}
BigInterger& BigInterger::operator&=(const BigInterger& bi);
{}
BigInterger& BigInterger::operator|=(const BigInterger& bi);
{}
//私有方法
void BigInterger::destructiveMulAdd(vector<unsigned int>& v, int sr, int gv)
{
	long long lsr = static_cast<long long>(sr);	//可以表示的最大数字（用来当进制）
	long long lgv = static_cast<long long>(gv);	//当前数字
	int len = v.size();
	long long product = 0, carry = 0;	//存储乘积的低32位和高32位
	/* 先进行数组的移位，从低位到高位分别和进制相乘 */
	for(int i = len - 1; i >= 0; --i){
		product = lsr * static_cast<long long>(v[i]) + carry;
		v[i] = static_cast<int>(product & 0x00000000ffffffff);	//低32位
		carry = product >> 32;	//进位（高32位）
	}
	long long sum = static_cast<long long>(v[len - 1]) + lgv;	//最低位置
	v[len - 1] = static_cast<int>(sum & 0x00000000ffffffff);
	carry = sum >> 32;
	/* 再将需要添加的gv添加进来 */
	for(int i = len - 2; i >= 0; --i){
		sum = static_cast<long long>(v[i]) + carry;
		v[i] = static_cast<int>(sum);
		carry = sum >> 32;
		if(carry == 0) break;
	}
}
string BigInterger::toString()
{
	if(signum == 0)
		return string("0");
	string ret;
	if(signum == -1) ret.push_back('-');
	ret += to_string(mag.front());
	/* 逐个把数放进去，不够补0 */
	for(int i = 1; i < mag.size(); ++i){
		string tmp = to_string(mag[i]);
		int zero = 9 - tmp.size();
		while(zero--) ret.push_back('0');
		ret += tmp;
	}
	return ret;
}
