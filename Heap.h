#pragma once
#include<iostream>
#include<vector>
#include<assert.h>

using namespace std;


//�º�������()
template<class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template <class T>
struct Greater
{
	bool operator()(const T& l, const T& r)
	{
		return l > r;
	}
};

//ע��ģ�����
template<class T, class compare = Greater<T>>
class Heap
{
public:
	//�޲ι��캯��
	Heap()
	{}
	//�вι��캯��
	Heap(const T* a, size_t size)
	{
		//����
		assert(a);
		//��������������ݿ�����˳�����
		for (size_t i = 0; i < size; i++)
		{
			_a.push_back(a[i]);
		}

		//�������Ϲ�����

		//�ҵ���һ����Ҷ�ӽ�㲢�Ӻ���ǰ��ʼ�𲽵���
		for (int i = (_a.size() - 2) / 2; i >= 0; i--)
		{
			_AdjustDown(i);
		}
	}
public:
	//����
	void Push(const T& d)
	{
		//���뵽˳�����
		_a.push_back(d);

		//���ϵ���
		_AdjustUp(_a.size() - 1);
	}
	//ɾ�����Ľڵ�
	void Pop()
	{
		//����˳���Ϊ��
		assert(!_a.empty());

		//������һ���ڵ�����һ���ڵ�
		swap(_a[0], _a[_a.size() - 1]);

		//ɾ�����һ���ڵ�
		_a.pop_back();

		//�ӵ�һ���ڵ㿪ʼ���µ���
		_AdjustDown(0);
	}
	//��ӡ
	void Print()
	{
		for (size_t i = 0; i < _a.size(); i++)
		{
			cout << _a[i] << " ";
		}
		cout << endl;
	}

	//���ضѶ���Ԫ��
	T Top()
	{
		return _a[0];
	}

	//���Ƿ�Ϊ��
	bool Empty()
	{
		return _a.empty();
	}

	size_t size()
	{
		return _a.size();
	}
protected:
	//���µ���
	void _AdjustDown(size_t parent)
	{
		size_t child = parent * 2 + 1;
		//����
		while (child < _a.size())//�ж������Ƿ����
		{
			//�ҵ����Һ����нϴ��
			compare com;
			/*if (child + 1 < _a.size() && _a[child + 1] > _a[child])*/
			if (child + 1 < _a.size() && com(_a[child + 1], _a[child]))
			{
				child++;
			}

			//���ӽڵ�ȸ��׽ڵ�󣬽��������ٴε���
			/*if (_a[child]>_a[parent])*/
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			//���ӽڵ�ȸ��׽ڵ�С������Ҫ����
			else
			{
				break;
			}
		}
	}
	//���ϵ���
	void _AdjustUp(size_t child)
	{
		size_t parent = (child - 1) / 2;
		while (parent<_a.size() && parent >= 0)//�����׽ڵ�Ϊ���ڵ�ʱ���ٵ���
		{
			//������ӽڵ�ȸ��׽ڵ���򽻻�λ�ã����������ϵ���
			/*if (_a[child] > _a[parent])*/
			compare com;
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			//���ӽڵ�ȸ��׽ڵ�С����Ҫ����
			else
			{
				break;
			}
		}
	}
protected:
	vector<T> _a;//˳���
};