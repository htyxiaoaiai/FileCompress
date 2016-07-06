#pragma once

/*
��Ŀ������
���������û��������룬���ļ�����ѹ��
����������vs2015��c++
��Ҫ�������ѡ����������롢̰���㷨��λ����
��Ŀ�ص㣺
		1������һ���ṹ�彫�ַ����ַ����ֵĴ������ַ��ı����Ӧ�������Ч��
		2�����û��������뽫��ͬ���ַ�����ΪΨһ������ı���
		3�������ɵı����Ե���������λ����ʽ�����д�뵽ѹ���ļ���ʵ��ѹ��
		4������ѹ���Ĺ����н��ַ���Ϣд�������ļ�
		5���������ļ������¹��������������н�ѹ
���������⣺
		1���������ļ����ڻ����ַ��������ԣ���Ҫ���⴦��
		2����д�ļ�ʱ��һ��ʼ���ı��ļ���д�������ļ���ѹ��ǰ����
		3����ѹʱ��ʹ��EOF�жϽ�β�������ļ��жϳ���
*/
#define _CRT_SECURE_NO_WARNINGS
#include"Huffman.h"
#include<string>

typedef int  LongType;

//�ַ��Ķ�Ӧ��Ϣ
struct CharInfo
{
	unsigned char _ch;//�ַ�
	LongType	_count;//�ַ����ֵĴ���
	string		_code;//�ַ���Ӧ�Ļ���������

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
		//��infos��ʼ��
		for (size_t i = 0; i < 256; i++)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
		}
	}

	//ѹ��
	void Compress(const char* filename)
	{
		FILE * fOut = fopen(filename, "r");
		assert(fOut);
		//ͳ���ַ����ֵĴ���
		char ch = fgetc(fOut);
		while (!feof(fOut))
		{
			_infos[(unsigned char)ch]._count++;
			//ע����Ҫת��Ϊ�޷������͵ģ���Ϊchar�ı�ʾ��ΧΪ-128-127��
			//�����ļ��е��ַ�Ϊ0-256�����������ʱ��ᷢ��Խ��
			ch = fgetc(fOut);
		}

		//����HuffmanTree
		CharInfo Invalid;
		HuffmanTree<CharInfo> tree(_infos, 256, Invalid);

		//����HuffmanCode
		string Code;
		GetHuffmanCode(tree.GetRoot(), Code);

		//ѹ��
		string CompressName = filename;
		CompressName += ".compress";
		FILE* fIn = fopen(CompressName.c_str(), "w");
		assert(fIn);

		//����ָ��ָ���0��ʼλ��
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
				//����һ���ֽڣ������ݷ���
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
		//û�з���
		if (size > 0)
		{
			value <<= (7 - size);
			fputc(value, fIn);
		}

		//д�����ļ�,��¼ÿ���ַ����ֵĴ���
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

		//�ر��ļ�
		fclose(fIn);
		fclose(fOut);
		fclose(fConfig);
	}


	//��ѹ��
	void UnCopmress(const char* filename)
	{
		//�������ļ�
		string ConfigName = filename;
		ConfigName += ".config";
		FILE *fConfig = fopen(ConfigName.c_str(), "r");
		assert(fConfig);
		//һ��һ�еĶ�ȡ
		string line;
		while (ReadLine(fConfig, line))
		{
			//���⴦����
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

		//����HuffmanTree
		CharInfo Invalid;
		HuffmanTree<CharInfo> tree(_infos, 256, Invalid);

		//��ѹ��
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
			//��ȡ�����ַ���ÿһλ
			if (ch&(1 << pos))
				cur = cur->_right;
			else
				cur = cur->_left;

			//�����˸��ڵ�
			if (cur->_left == NULL&&cur->_right == NULL)
			{
				fputc(cur->_weight._ch, fIn);
				count++;
				cur = root;
			}
			//�������һ���ַ�
			if (pos-- == 0)
			{
				ch = fgetc(fOut);
				pos = 7;
			}

			//��λ����Щ���ֲ���Ҫ������ͨ����¼�ַ��ĸ���
			if (count >= root->_weight._count)
			{
				break;
			}
		}

		//�ر��ļ�
		fclose(fIn);
		fclose(fOut);
		fclose(fConfig);
	}
protected:
	//��ȡHuffmanCode
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

	//��ȡһ��
	bool ReadLine(FILE* fOut, string & line)
	{
		char ch = fgetc(fOut);
		if (feof(fOut))
			return false;

		//�����ļ���β������β
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

//"�����ƶ�д"��ԭ���Ķ�д
//�ı����Ͷ�д�������ַ�

//�ı��ļ��£�����ʱ��\nд��ȥΪ\r\n  �ٶ�������ʱ����Ϊ\n

//�ļ��Ľ�β
//ѹ���ļ�����д���Ϊȫ1(-1)������ǰ����������ѹ�����ļ��Ƚ���