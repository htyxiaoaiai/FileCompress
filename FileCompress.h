#pragma once

/*
项目描述：
简述：利用霍夫曼编码，对文件进行压缩
开发环境：vs2015、c++
主要技术：堆、霍夫曼编码、贪心算法、位运算
项目特点：
		1、利用一个结构体将字符、字符出现的次数、字符的编码对应，提高了效率
		2、利用霍夫曼编码将不同的字符生成为唯一的最简洁的编码
		3、将生成的编码以单个二进制位的形式，逐个写入到压缩文件，实现压缩
		4、利用压缩的过程中将字符信息写入配置文件
		5、读配置文件，重新构建霍夫曼树进行解压
遇到的问题：
		1、读配置文件由于换行字符的特殊性，需要特殊处理。
		2、读写文件时的一开始以文本文件读写，导致文件解压提前结束
		3、解压时，使用EOF判断结尾，导致文件判断出错
*/
#define _CRT_SECURE_NO_WARNINGS
#include"Huffman.h"
#include<string>

typedef int  LongType;

//字符的对应信息
struct CharInfo
{
	unsigned char _ch;//字符
	LongType	_count;//字符出现的次数
	string		_code;//字符对应的霍夫曼编码

	CharInfo(LongType count = 0)
		:_count(count)
	{}

	bool operator<(const CharInfo&info)const
	{
		return _count < info._count;
	}

	CharInfo operator+(CharInfo& info)//const
	{
		return CharInfo(_count + info._count);
	}

	bool operator!=(const CharInfo& info)//const
	{
		return _count != info._count;
	}
};


class FileCompress
{
public:
	FileCompress()
	{
		//将infos初始化
		for (size_t i = 0; i < 256; i++)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}
	}

	//压缩
	void Compress(const char* filename)
	{
		FILE * fOut = fopen(filename, "r");
		assert(fOut);
		//统计字符出现的次数
		char ch = fgetc(fOut);
		while (!feof(fOut))
		{
			_infos[(unsigned char)ch]._count++;
			//注意需要转换为无符号类型的，因为char的表示范围为-128-127，
			//但是文件中的字符为0-256，访问数组的时候会发生越界
			ch = fgetc(fOut);
		}

		//构建HuffmanTree
		CharInfo Invalid;
		HuffmanTree<CharInfo> tree(_infos, 256, Invalid);

		//构建HuffmanCode
		string Code;
		GetHuffmanCode(tree.GetRoot(), Code);

		//压缩
		string CompressName = filename;
		CompressName += ".compress";
		FILE* fIn = fopen(CompressName.c_str(), "w");
		assert(fIn);

		//将读指针恢复到0初始位置
		fseek(fOut, 0, SEEK_SET);
		ch = fgetc(fOut);
		int size = 0;
		char value = 0;
		while (!feof(fOut))
		{
			string code = _infos[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); i++)
			{
				if (code[i] == '1')
					value |= 1;

				size++;
				//放满一个字节，将内容放入
				if (size == 8)
				{
					fputc(value, fIn);
					size = 0;
					value = 0;
				}
				value <<= 1;
			}
			ch = fgetc(fOut);
		}
		//没有放满
		if (size > 0)
		{
			value <<= (7 - size);
			fputc(value, fIn);
		}

		//写配置文件,记录每个字符出现的次数
		string ConfigName = filename;
		ConfigName += ".config";
		FILE *fConfig = fopen(ConfigName.c_str(), "w");
		assert(fConfig);

		string line;
		for (size_t i = 0; i < 256; i++)
		{
			if (_infos[i] != Invalid)
			{
				line += _infos[i]._ch;
				line += ',';
				char buffer[100];
				_itoa(_infos[i]._count, buffer, 10);
				line += buffer;
				line += '\n';
				fputs(line.c_str(), fConfig);
			}
			line.clear();
		}

		//关闭文件
		fclose(fIn);
		fclose(fOut);
		fclose(fConfig);
	}


	//解压缩
	void UnCopmress(const char* filename)
	{
		//读配置文件
		string ConfigName = filename;
		ConfigName += ".config";
		FILE *fConfig = fopen(ConfigName.c_str(), "r");
		assert(fConfig);
		//一行一行的读取
		string line;
		while (ReadLine(fConfig, line))
		{
			//特殊处理换行
			if (line.empty())
			{
				line += '\n';
			}
			else
			{
				unsigned char ch = line[0];
				LongType count = atoi(line.substr(2).c_str());
				_infos[ch]._count = count;
				line.clear();
			}
		}

		//构建HuffmanTree
		CharInfo Invalid;
		HuffmanTree<CharInfo> tree(_infos, 256, Invalid);

		//解压缩
		string compressFile = filename;
		compressFile += ".compress";
		FILE* fOut = fopen(compressFile.c_str(), "r");
		assert(fOut);

		string UncompressFile = filename;
		UncompressFile += ".uncompress";
		FILE* fIn = fopen(UncompressFile.c_str(), "w");
		assert(fIn);

		HuffManTreeNode<CharInfo>* root = tree.GetRoot();
		HuffManTreeNode<CharInfo>* cur = root;

		char ch = fgetc(fOut);
		int pos = 7;
		LongType count = 0;
		while (!feof(fOut))
		{
			//获取读出字符的每一位
			if (ch&(1 << pos))
				cur = cur->_right;
			else
				cur = cur->_left;

			//读到了根节点
			if (cur->_left == NULL&&cur->_right == NULL)
			{
				fputc(cur->_weight._ch, fIn);
				count++;
				cur = root;
			}
			//解析完成一个字符
			if (pos-- == 0)
			{
				ch = fgetc(fOut);
				pos = 7;
			}

			//补位的那些数字不需要解析，通过记录字符的个数
			if (count >= root->_weight._count)
			{
				break;
			}
		}

		//关闭文件
		fclose(fIn);
		fclose(fOut);
		fclose(fConfig);
	}
protected:
	//获取HuffmanCode
	void GetHuffmanCode(HuffManTreeNode<CharInfo>* root, string code)
	{
		if (root == NULL)
		{
			return;
		}
		if (root->_left == NULL&&root->_right == NULL)
		{
			_infos[root->_weight._ch]._code = code;
		}

		GetHuffmanCode(root->_left, code + '0');
		GetHuffmanCode(root->_right, code + '1');
	}

	//读取一行
	bool ReadLine(FILE* fOut, string & line)
	{
		char ch = fgetc(fOut);
		if (feof(fOut))
			return false;

		//读到文件结尾或者行尾
		while ((!feof(fOut)) && (ch != '\n'))
		{
			line += ch;
			ch = fgetc(fOut);
		}
		return true;
	}
private:
	CharInfo _infos[256];
};


void TestCompress()
{
	char* filename = "Input.BIG";
	FileCompress file;
	file.Compress(filename);

}

void TestUnCompress()
{
	char* filename = "Input.BIG";
	FileCompress file;
	file.UnCopmress(filename);
}

//"二进制读写"：原样的读写
//文本类型读写：特殊字符

//文本文件下：读的时候\n写进去为\r\n  再读出来的时候则为\n

//文件的结尾
//压缩文件可能写入的为全1(-1)，会提前结束，导致压缩的文件比较少