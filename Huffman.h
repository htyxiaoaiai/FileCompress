#pragma once
#include "Heap.h"

//���������Ľڵ�
template<class T>
struct HuffManTreeNode
{
	HuffManTreeNode(const T& weight)
		:_left(NULL)
		,_right(NULL)
		,_weight(weight)
	{}
	HuffManTreeNode<T>* _left;//������
	HuffManTreeNode<T>* _right;//������
	T _weight;//Ȩ��
};


template<class T>
class HuffmanTree
{
	typedef HuffManTreeNode<T> Node;
public:
	//���캯��
	HuffmanTree(T* data, size_t n,const T& invalid)
	{
		//���ڲ�����º���
		struct Comp
		{
			bool operator()(const Node* left, const Node* right)
			{
				return left->_weight < right->_weight;
			}

		};
		//�����е�Ԫ�ع���һ��С��(��Ž��)��ֻ��Ȩ�صĻ��Ҳ������Ӧ�����ҽڵ�
		Heap<Node*, Comp> hp;
		//����С��
		for (size_t i = 0; i < n; i++)
		{
			if (data[i] != invalid)
			{
				hp.Push(new Node(data[i]));
			}	
		}
		//������������
		while (hp.size()>1)
		{
			Node* left= hp.Top();
			hp.Pop();
			Node* right = hp.Top();
			hp.Pop();
			Node* parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;
			hp.Push(parent);
		}
		_root = hp.Top();
	}

	Node* GetRoot()
	{
		return _root;
	}
private:
	Node* _root;
};


void TestHuffmanTree()
{
	int a[] = { 0,1,2,3,4,5,6,7,8,9 };
	HuffmanTree<int> hf(a, sizeof(a) / sizeof(a[0]),-1);
	cout << endl;
}