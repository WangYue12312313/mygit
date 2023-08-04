#include <map>
#include <iostream>
#include <string>
#include <cmath>
#include<time.h>
#include <cstdint>
#include <algorithm>
#include <sstream>
using namespace std;
time_t start1, end1;
int total;

//32bit
string BinToHex(string str) {
	string hex = "";//用来存储最后生成的十六进制数
	int temp = 0;//用来存储每次四位二进制数的十进制值
	while (str.size() % 4 != 0) {//因为每四位二进制数就能够成为一个十六进制数，所以将二进制数长度转换为4的倍数
		str = "0" + str;//最高位添0直到长度为4的倍数即可
	}
	for (int i = 0; i < str.size(); i += 4) {
		temp = (str[i] - '0') * 8 + (str[i + 1] - '0') * 4 + (str[i + 2] - '0') * 2 + (str[i + 3] - '0') * 1;//判断出4位二进制数的十进制大小为多少
		if (temp < 10) {//当得到的值小于10时，可以直接用0-9来代替
			hex += to_string(temp);
		}
		else {//当得到的值大于10时，需要进行A-F的转换
			hex += 'a' + (temp - 10);
		}
	}
	return hex;
}
//十六进制转换为二进制函数实现
string HexToBin(string str) {
	static const std::string table[16] = {
	 "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
	 "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"
	};

	std::string bin;
	bin.reserve(str.size() * 4); // 预分配足够的空间

	for (char c : str) {
		unsigned int num;
		if (c >= 'a' && c <= 'f') {
			num = c - 'a' + 10;
		}
		else {
			num = c - '0';
		}
		bin += table[num];
	}

	return bin;
}
//二进制转换为十进制的函数实现
int BinToDec(string str) {
	int dec = 0;
	for (int i = 0; i < str.size(); i++) {
		dec += (str[i] - '0') * pow(2, str.size() - i - 1);
	}
	return dec;
}

//十进制转换为二进制的函数实现
string DecToBin(int str) {
	string bin = "";
	while (str >= 1) {
		bin = to_string(str % 2) + bin;
		str = str / 2;
	}
	return bin;
}

//十六进制转换为十进制的函数实现

int HexToDec(string str) {
	int dec = 0;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] >= 'a' && str[i] <= 'f') {
			dec += (str[i] - 'a' + 10) * pow(16, str.size() - i - 1);
		}
		else {
			dec += (str[i] - '0') * pow(16, str.size() - i - 1);
		}
	}
	return dec;
}

//十进制转换为十六进制的函数实现
string DecToHex(int str) {
	string hex = "";
	int temp = 0;
	while (str >= 1) {
		temp = str % 16;
		if (temp < 10 && temp >= 0) {
			hex = to_string(temp) + hex;
		}
		else {
			hex = char(('a' + (temp - 10))) + hex;
		}
		str = str / 16;
	}
	return hex;
}

string padding(string str) {//对数据进行填充 
	string res = str;

	int res_length = res.size() * 4;//记录的长度为2进制下的长度

	res += "8";//在获得的数据后面添1，在16进制下相当于是添加8


	while (res.size() % 128 != 112) {
		res += "0";//“0”数据填充
	}

	string res_len = DecToHex(res_length);//用于记录数据长度的字符串

	while (res_len.size() != 16) {
		res_len = "0" + res_len;
	}
	res += res_len;
	return res;
}

string LeftShift(string str, int len) {//实现循环左移len位功能
	string res = HexToBin(str);
	res = res.substr(len) + res.substr(0, len);
	return BinToHex(res);
}

string XOR(string str1, string str2) {//实现异或操作
	string res1 = HexToBin(str1);
	string res2 = HexToBin(str2);
	string res = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == res2[i]) {
			res += "0";
		}
		else {
			res += "1";
		}
	}
	return BinToHex(res);
}

string AND(string str1, string str2) {//实现与操作
	string res1 = HexToBin(str1);
	string res2 = HexToBin(str2);
	string res = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == '1' && res2[i] == '1') {
			res += "1";
		}
		else {
			res += "0";
		}
	}
	return BinToHex(res);
}

string OR(string str1, string str2) {//实现或操作
	string res1 = HexToBin(str1);
	string res2 = HexToBin(str2);
	string res = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == '0' && res2[i] == '0') {
			res += "0";
		}
		else {
			res += "1";
		}
	}
	return BinToHex(res);
}

string NOT(string str) {//实现非操作
	string res1 = HexToBin(str);
	string res = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == '0') {
			res += "1";
		}
		else {
			res += "0";
		}
	}
	return BinToHex(res);
}

char binXor(char str1, char str2) {//实现单比特的异或操作
	return str1 == str2 ? '0' : '1';
}

char binAnd(char str1, char str2) {//实现单比特的与操作

	return (str1 == '1' && str2 == '1') ? '1' : '0';
}



string ModAdd(string str1, string str2) {//mod 2^32运算的函数实现

	start1 = clock();



	string res = "";
	int maxlength = max(str1.size(), str2.size());
	int tmp1, tmp2, tmp;
	tmp = 0;
	res.reserve(maxlength);
	for (int i = str1.size() - 1; i >= 0; i--) {
		if (str1[i] >= '0' && str1[i] <= '9')
			tmp1 = str1[i] - '0';
		else
			tmp1 = str1[i] - 'a' + 10;
		if (str2[i] >= '0' && str2[i] <= '9')
			tmp2 = str2[i] - '0';
		else
			tmp2 = str2[i] - 'a' + 10;
		if ((tmp1 + tmp2 + tmp) > 16)
		{
			res = DecToHex(tmp1 + tmp2 + tmp - 16) + res;
			tmp = 1;

		}
		else if (((tmp1 + tmp2 + tmp) % 16) == 0)
		{
			res = '0' + res;
			tmp = (tmp1 + tmp2 + tmp) / 16;
		}

		else
		{
			res = DecToHex(tmp1 + tmp2 + tmp) + res;
			tmp = 0;

		}
	}
	end1 = clock();
	total += end1 - start1;
	return res;
}
string P1(string str) {//实现置换功能P1（X）
	return XOR(XOR(str, LeftShift(str, 15)), LeftShift(str, 23));
}

string P0(string str) {//实现置换功能P0（X）
	return XOR(XOR(str, LeftShift(str, 9)), LeftShift(str, 17));
}

string T(int j) {//返回Tj常量值的函数实现
	if (0 <= j && j <= 15) {
		return "79cc4519";
	}
	else {
		return "7a879d8a";
	}
}

string FF(string str1, string str2, string str3, int j) {//实现布尔函数FF功能

	if (0 <= j && j <= 15) {
		return XOR(XOR(str1, str2), str3);
	}
	else {
		return OR(OR(AND(str1, str2), AND(str1, str3)), AND(str2, str3));
	}
}

string GG(string str1, string str2, string str3, int j) {//实现布尔函数GG功能
	if (0 <= j && j <= 15) {
		return XOR(XOR(str1, str2), str3);
	}
	else {
		return OR(AND(str1, str2), AND(NOT(str1), str3));
	}
}
string extension(string str) {//消息扩展函数

	string res = str;//字符串类型存储前68位存储扩展字W值
	for (int i = 16; i < 68; i++) {//根据公式生成第17位到第68位的W值
		res += XOR(XOR(P1(XOR(XOR(res.substr((i - 16) * 8, 8), res.substr((i - 9) * 8, 8)), LeftShift(res.substr((i - 3) * 8, 8), 15))), LeftShift(res.substr((i - 13) * 8, 8), 7)), res.substr((i - 6) * 8, 8));
	}




	for (int i = 0; i < 64; i++) {//根据公式生成64位W'值
		res += XOR(res.substr(i * 8, 8), res.substr((i + 4) * 8, 8));
	}



	return res;
}

string compress(string str1, string str2) {//消息压缩函数

	string IV = str2;
	string blocks[8];
	string T_tmp;
	int tmp;
	for (int i = 0; i < 8; i++) {
		blocks[i] = IV.substr(i * 8, 8);
	}

	for (int j = 0; j < 64; j++) {
		T_tmp = T(j);
		tmp = j * 8;

		string SS1 = LeftShift(ModAdd(ModAdd(LeftShift(blocks[0], 12), blocks[4]), LeftShift(T_tmp, (j % 32))), 7);

		string SS2 = XOR(SS1, LeftShift(blocks[0], 12));

		string TT1 = ModAdd(ModAdd(ModAdd(FF(blocks[0], blocks[1], blocks[2], j), blocks[3]), SS2), str1.substr(tmp + 544, 8));
		string TT2 = ModAdd(ModAdd(ModAdd(GG(blocks[4], blocks[5], blocks[6], j), blocks[7]), SS1), str1.substr(tmp, 8));

		blocks[3] = blocks[2];
		blocks[2] = LeftShift(blocks[1], 9);
		blocks[1] = blocks[0];
		blocks[0] = TT1;
		blocks[7] = blocks[6];
		blocks[6] = LeftShift(blocks[5], 19);
		blocks[5] = blocks[4];
		blocks[4] = P0(TT2);
	}

	string res;
	for (int i = 0; i < 8; i++) {
		res += blocks[i];
	}

	return res;
}

string iteration(string str) {//迭代压缩函数实现
	int num = str.size() / 128;

	string V = "7380166f4914b2b9172442d7da8a0600a96f30bc163138aae38dee4db0fb0e4e";
	string B = "", extensionB = "", compressB = "";
	for (int i = 0; i < num; i++) {

		B = str.substr(i * 128, 128);

		extensionB = extension(B);//


		compressB = compress(extensionB, V);//1


		V = XOR(V, compressB);

	}
	return V;
}

string the_SM3(string str)
{

	string res = "";

	for (int i = 0; i < str.size(); i++) {//首先将输入值转换为16进制字符串
		res += DecToHex((int)str[i]);
	}


	string paddingValue = padding(res);



	string result = iteration(paddingValue);

	return result;

}

int main() {//主函数

	time_t start, end;
	string str = "agaeegaggwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwaaaa";
	string tmp = "";

	string res = "";

	for (int i = 0; i < str.size(); i++) {//首先将输入值转换为16进制字符串
		res += DecToHex((int)str[i]);
	}
	start = clock();

	tmp = the_SM3(str);


	end = clock();

	cout << tmp << endl;
	cout << "time:" << (end - start) << "ms" << endl;


}

