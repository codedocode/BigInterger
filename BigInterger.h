#pragma once
#include <vector>
#include <exception>
#include <string>
using namespace std;
class BigInterger
{
	//typedef unsigned int u32;
	//typedef int i32;
	//typedef long long i64;
	//typedef unsigned long long u64;
public:
	BigInterger(string val, int radix);
	string toString(int radix);
private:
	vector<unsigned int> mag;	//用于承载大数的数组
	int signum;	//存储符号位
private:
	static const int MIN_RADIX = 2;	//最小进制
	static const int MAX_RADIX = 36;//最大进制
	/* 每个int能放多少位数字（索引为进制数） */
	static const int digitsPerInt[] = {0, 0, 30, 19, 15, 13, 11,
		11, 10, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5
	};
	/* 每个进制的数要用几位二进制来表示（索引为进制数，结果乘以1024以防止纯小数的产生） */
	static const int bitsPerDigit[] = { 0, 0,
		1024, 1624, 2048, 2378, 2648, 2875, 3072, 3247, 3402, 3543, 3672,
		3790, 3899, 4001, 4096, 4186, 4271, 4350, 4426, 4498, 4567, 4633,
		4696, 4756, 4814, 4870, 4923, 4975, 5025, 5074, 5120, 5166, 5210,
		5253, 5295
	};
	/* 保存了对应各种radix的最大可表示的数字，由进制和位数表求出 */
	static const int intRadix[] = {0, 0,
		0x40000000, 0x4546b3db, 0x40000000, 0x48c27395, 0x159fd800,
		0x75db9c97, 0x40000000, 0x17179149, 0x3b9aca00, 0xcc6db61,
		0x19a10000, 0x309f1021, 0x57f6c100, 0xa2f1b6f,  0x10000000,
		0x18754571, 0x247dbc80, 0x3547667b, 0x4c4b4000, 0x6b5a6e1d,
		0x6c20a40,  0x8d2d931,  0xb640000,  0xe8d4a51,  0x1269ae40,
		0x17179149, 0x1cb91000, 0x23744899, 0x2b73a840, 0x34e63b41,
		0x40000000, 0x4cfa3cc1, 0x5c13d840, 0x6d91b519, 0x39aa400
	};
	int parseInt(string val, int radix);
	void delZeroInts(){
		while(!mag.empty() && mag[0] == 0){
			if(mag[0] == 0) mag.erase(mag.begin());
		}
	} 
};

int BigInterger::parseInt(string val, int radix)
{
	int ret = 0;
	for(int i = 0; i < val.size(); ++i)
		ret = ret * radix + val[i] - '0';
	return ret;
}
BigInterger::BigInterger(string val, int radix)
{
	int cursor = 0, numDigits;	//0的个数和有效数字的个数
	int len = val.size();	//字符串长度
	if(radix > MIN_RADIX || radix > MAX_RADIX)
		throw exception("Radix out of range");
	if(val.size() == 0)
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
	/* 换算成二进制需要多少位，加1是防止小数丢失 */
	int numBits = ((numDigits * bitsPerDigit[radix]) >> 10) + 1;
	int numWords = (numBits + 31) >> 32;	//mag数组的大小
	mag.resize(numWords);
	int firstLen = numDigits % digitsPerInt[radix];	//最高位多出来的位数先处理
	if(firstLen == 0)	//刚好整除
		firstLen = digitsPerInt[radix];
	string group = val.substr(cursor, cursor += firstLen);
	mag[mag.size() - 1] = parseInt(group, radix);
	if(mag.back() < 0)
		throw exception("Illegal digit");
	int superRadix = intRadix[radix];	//当前进制下，一个unsigned int可表示的最大的数
	int groupVal = 0;
	while(cursor < val.size()){
		group = val.substr(cursor, cursor += digitsPerInt[radix]);
		groupVal = parseInt(group, radix);
		if(groupVal < 0)
			throw exception("Illegal digit");
		destructiveMulAdd(mag, superRadix, groupVal);
	}
	/* 去除前导0 */
	delZeroInts();
}
void BigInterger::destructiveMulAdd(vector<unsigned int>& v, int sr, int gv)
{
	long lsr = static_cast<long long>(sr);
	long lgv = static_cast<long long>(gv);
	int len = v.size();
	long long product = 0, carry = 0;	//存储乘积的低32位和高32位
	/* 先进行数组的移位，从低位到高位分别和y相乘，每次都加上之前的进位 */
	for(int i = len - 1; i >= 0; --i){
		product = lsr * static_cast<long long>(v[i]) + carry;
		x[i] = static_cast<int>(product);	//低32位
		carry = product >> 32;	//进位（高32位）
	}
	long long sum = static_cast<long long>(v[len - 1]) + lgv;	//最低位置
	v[len - 1] = static_cast<int>(sum);
	carry = sum >> 32;
	/* 再将需要添加的gv添加进来 */
	for(int i = len - 2; i >= 0; --i){
		sum = static_cast<long long>(v[i]) + carry;
		v[i] = static_cast<int>(sum);
		carry = sum >> 32;
	}
}
string BigIntergers::toString(int radix)
{
	if(signum == 0)
		return string("0");
	if(radix > MIN_RADIX || radix > MAX_RADIX)
		radix = 10;
	//int maxNumDigitsGroups = (4 * mag.size() + 6) / 7;	//求出字节数
	//string str;
	//str.resize(maxNumDigitsGroups);	//申请合适的空间
	string ret;
	int carry = 0;
	for(int i = mag.size() - 1; i >= 0; --i){
		
	}
}
