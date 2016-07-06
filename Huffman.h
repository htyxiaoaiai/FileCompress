#pragma once
#include "Heap.h"

//霍夫曼树的节点
template<class T>
struct HuffManTreeNode
{
	HuffManTreeNode(const T& weight)
		:_left(NULL)
		,_right(NULL)
		,_weight(weight)
	{}
	HuffManTreeNode<T>* _left;//左子树
	HuffManTreeNode<T>* _right;//右子树
	T _weight;//权重
};


template<class T>
class HuffmanTree
{
	typedef HuffManTreeNode<T> Node;
public:
	//构造函数
	HuffmanTree(T* data, size_t n,const T& invalid)
	{
		//在内部构造仿函数
		struct Comp
		{
			bool operator()(const Node* left, const Node* right)
			{
				return left->_weight < right->_weight;
			}

		};
		//将所有的元素构成一个小堆(存放结点)，只放权重的话找不到其对应的左右节点
		Heap<Node*, Comp> hp;
		//构建小堆
		for (size_t i = 0; i < n; i++)
		{
			if (data[i] != invalid)
			{
				hp.Push(new Node(data[i]));
			}	
		}
		//构建霍夫曼树
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