#pragma once
#include <vector>
#include <exception>
#include <string>
using namespace std;
class BigInterger
{
	typedef unsigned int u32;
	typedef int i32;
	typedef long long i64;
	typedef unsigned long long u64;
public:
	BigInterger(string val, int radix);
private:
	vector<u32> mag;	//用于承载大数的数组
	int signum;	//存储符号位
private:
	static const int MIN_RADIX = 2;	//最小进制
	static const int MAX_RADIX = 36;//最大进制
	/* 每个int能放多少位数字（索引为进制数） */
	static const i32 digitsPerInt[] = {0, 0, 30, 19, 15, 13, 11,
		11, 10, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5
	};
	/* 每个进制的数要用几位二进制来表示（索引为进制数，结果乘以1024以防止纯小数的产生） */
	static const i64 bitsPerDigit[] = { 0, 0,
		1024, 1624, 2048, 2378, 2648, 2875, 3072, 3247, 3402, 3543, 3672,
		3790, 3899, 4001, 4096, 4186, 4271, 4350, 4426, 4498, 4567, 4633,
		4696, 4756, 4814, 4870, 4923, 4975, 5025, 5074, 5120, 5166, 5210,
		5253, 5295
	};
	/* 保存了对应各种radix的最佳进制的表 */
	static const i32 intRadix[] = {0, 0,
		0x40000000, 0x4546b3db, 0x40000000, 0x48c27395, 0x159fd800,
		0x75db9c97, 0x40000000, 0x17179149, 0x3b9aca00, 0xcc6db61,
		0x19a10000, 0x309f1021, 0x57f6c100, 0xa2f1b6f,  0x10000000,
		0x18754571, 0x247dbc80, 0x3547667b, 0x4c4b4000, 0x6b5a6e1d,
		0x6c20a40,  0x8d2d931,  0xb640000,  0xe8d4a51,  0x1269ae40,
		0x17179149, 0x1cb91000, 0x23744899, 0x2b73a840, 0x34e63b41,
		0x40000000, 0x4cfa3cc1, 0x5c13d840, 0x6d91b519, 0x39aa400
	};
};
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
		sinum = 1;
		mag.push_back(0);
		return ;
	}
	else{
		numDigits = len - cursor;	//有效数字的长度
	}
	int numBits

}
