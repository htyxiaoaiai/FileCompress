//#include"Huffman.h"
#include "FileCompress.h"
#include <Windows.h>

void Test()
{
	string fileName1 = "InputB";
	string fileName2 = "InputC";
	FILE * fin1 = fopen(fileName1.c_str(), "wb");
	FILE * fin2 = fopen(fileName2.c_str(), "w");
	string str1 = "s/////////////////////////dksjdkfjlkjdfshjah jdkjshfk�ؼҺ÷�jhjka hjhkjahsajk hjkhgjkahk\njhkjhetoiweuoi hj\nhkjtalkjlkahsjfhdjkhaljkjlkjlk!@#$%^&*()_";
	string str2 = "�������@#��%����&\n*����ʱ��/////////////��\n�������˵ķ���";
	for (size_t i = 0; i < str1.size(); i++)
	{
		fputc(str1[i], fin1);
		fputc(str1[i], fin2);
	}
	fputc('\n', fin1);
	fputc('\n', fin2);

	for (size_t i = 0; i < str2.size(); i++)
	{
		fputc(str2[i], fin1);
		fputc(str2[i], fin2);
	}
	
}
int main()
{
	////TestHuffmanTree();
	////ѹ��
	////int begin = GetTickCount();
	////TestCompress();
	////int end = GetTickCount();
	////cout << "ѹ����ʱ��" << end - begin << endl;

	////��ѹ
	////begin = GetTickCount();
	////TestUnCompress();
	////end = GetTickCount();
	////cout << "��ѹ��ʱ��" << end - begin << endl;

	Test();
	getchar();
	return 0;
}
