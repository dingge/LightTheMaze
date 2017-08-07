/********************************************************************
	date  :	2016/02/02   12:55

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <iostream>
#include "cocos2d.h"
#include "Game/GameUtils/Line.h"

/**
 *	������
 */
template<class Tnode>
struct ListNode
{
    Tnode _node;
    ListNode* _next;

    ListNode() :_next(nullptr) {}
    ~ListNode() 
    {
        if (_next)
        {
            delete _next;
        }
    }
};

class QuadNode;
/**
 *	�����
 *  ��Ϊһ��Ľ����Ϊ�������Ĳ���������
 */
template<class Tnode>
class NodeGroup
{
public:
    ListNode<Tnode>* head_;
    ListNode<Tnode>* last_;
    cocos2d::Rect rect_;
    QuadNode* quad_node_ref_;

    NodeGroup() :
        head_(nullptr), last_(nullptr), 
        rect_(0.0f,0.0f,0.0f,0.0f), quad_node_ref_(nullptr) {}
    ~NodeGroup() 
    {
        if (head_)
        {
            delete head_;
        }
    }

    void addNode(Tnode node)
    {
        if (head_)
        {
            last_->_next = new ListNode<Tnode>();
            last_ = last_->_next;
            last_->_node = node;
        }
        else
        {
            head_ = last_ = new ListNode<Tnode>();
            head_->_node = node;
        }
    }

    ListNode<Tnode>* findNode(Tnode node)
    {
        ListNode<Tnode>* p = head_;
        while (p != nullptr)
        {
            if (p->_node == node)
            {
                return p;
            }
        }
        return nullptr;
    }

    void fixUpRect()
    {
        rect_ = cocos2d::Rect::ZERO;
        ListNode<Tnode>* p = head_;
        if (p != nullptr)
        {
            rect_ = head_->_node.getRect();
            p = p->_next;
        }
        while (p != nullptr)
        {
            rect_ = rect_.unionWithRect(p->_node.getRect());
            p = p->_next;
        }
    }
};

typedef std::shared_ptr<NodeGroup<Edge>> NodeGroupPtr;
/**
 *	�Ĳ������
 */
class QuadNode
{
public:
	cocos2d::Rect _rect;
    std::vector<NodeGroupPtr> _node_group_list;
	QuadNode* _child[4];
public:
	QuadNode():
		QuadNode(cocos2d::Rect(0.0f,0.0f,0.0f,0.0f)){}
	
	QuadNode(const cocos2d::Rect& rect):
		_rect(rect) 
	{
		for (int i = 0; i < 4; ++i)
		{
			_child[i] = nullptr;
		}
	}

	~QuadNode()
	{
		for (int i = 0;i < 4;++i)
		{
			if (_child[i])
			{
				delete _child[i];
				_child[i] = nullptr;
			}
		}
	}
};

/**
 *	�Ĳ���
 */
class QuadTree
{
public:
	QuadTree(const cocos2d::Rect& rect);
	~QuadTree(void);

	///��ָ�����ݲ����Ĳ���
	void insert(const NodeGroupPtr& data);

	///���Ĳ�����ɾ��ָ������
	void remove(const NodeGroupPtr& data);
	void removeByGroup(int group_name);

	///�����Ĳ���(�����ѯ)
	void searchByRect(const cocos2d::Rect& search_rect, std::vector<Edge>* result);

	///�����Ĳ���(���߲�ѯ)
	void searchByRay(const Line& ray, std::vector<Edge>* result);

	///���Ĳ������޸�ָ������
	void alter(const NodeGroupPtr& data);

	///��ӡ�����Ĳ���
	void printQuadTree();
protected:
	enum QuadEnum
	{
		UL = 0,      ///<����
		UR = 1,      ///<����
		DL = 2,      ///<����
		DR = 3,      ///<����
		Self = 4,    ///<����
		Overflow = 5 ///<Խ��
	};

	enum QuadTreeOption
	{
		MaxDepth = 4  ///<�Ĳ���������
	};

	///�ݹ�������ݵ���ǰ�Ĳ������
	void insert(const NodeGroupPtr& data, QuadNode* node, unsigned int depth);

	///����ָ���������ڽ��
    NodeGroupPtr findData(const Edge& data);
	
	///��ȡ��ǰ��/�����ھ��ε��ĸ��������Ӧ���±�
	int getChildIndex(const cocos2d::Vec2& pos, const cocos2d::Rect& rect);
	int getChildIndex(const NodeGroupPtr& line_group, const cocos2d::Rect& rect);
    ///���±��ȡ��Ӧ�������Rect
    cocos2d::Rect getRectByChildIndex(int index,const cocos2d::Rect& rect);
	
	///ֱ�Ӳ������ݵ��������
	void insertToCurrentNode(const NodeGroupPtr& line_group, QuadNode* node);

	///�ݹ���ҵ�ǰ�Ĳ������
	void searchByRect(const cocos2d::Rect& search_rect, const QuadNode* node, std::vector<Edge>* result);

	///�ݹ��ӡ��ǰ�Ĳ������
	void printQuadTree(const QuadNode* node, std::string index);

	///�жϵ�ǰ��㼰�����ӽ���Ƿ�Ϊ��
	bool isNull(const QuadNode* node);

	///�ж������Ƿ����߶��ཻ
	bool isRayIntersectSegment(const Line& ray, const Edge& segment);
	///�ж�����������Ƿ��ཻ
	bool isRayIntersectRect(const Line& ray, const cocos2d::Rect& rect);
private:
	QuadNode* m_root;
    std::vector<NodeGroupPtr> group_container_;
	unsigned int m_depth;
};

#endif // !QUAD_TREE_H