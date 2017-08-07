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
 *	单链表
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
 *	结点组
 *  归为一组的结点作为整体在四叉树中运作
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
 *	四叉树结点
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
 *	四叉树
 */
class QuadTree
{
public:
	QuadTree(const cocos2d::Rect& rect);
	~QuadTree(void);

	///把指定数据插入四叉树
	void insert(const NodeGroupPtr& data);

	///从四叉树中删除指定数据
	void remove(const NodeGroupPtr& data);
	void removeByGroup(int group_name);

	///查找四叉树(矩阵查询)
	void searchByRect(const cocos2d::Rect& search_rect, std::vector<Edge>* result);

	///查找四叉树(射线查询)
	void searchByRay(const Line& ray, std::vector<Edge>* result);

	///在四叉树中修改指定数据
	void alter(const NodeGroupPtr& data);

	///打印整棵四叉树
	void printQuadTree();
protected:
	enum QuadEnum
	{
		UL = 0,      ///<左上
		UR = 1,      ///<右上
		DL = 2,      ///<左下
		DR = 3,      ///<右下
		Self = 4,    ///<自身
		Overflow = 5 ///<越界
	};

	enum QuadTreeOption
	{
		MaxDepth = 4  ///<四叉树最大深度
	};

	///递归插入数据到当前四叉树结点
	void insert(const NodeGroupPtr& data, QuadNode* node, unsigned int depth);

	///查找指定数据所在结点
    NodeGroupPtr findData(const Edge& data);
	
	///获取当前点/线所在矩形的四个子区域对应的下标
	int getChildIndex(const cocos2d::Vec2& pos, const cocos2d::Rect& rect);
	int getChildIndex(const NodeGroupPtr& line_group, const cocos2d::Rect& rect);
    ///从下标获取对应子区域的Rect
    cocos2d::Rect getRectByChildIndex(int index,const cocos2d::Rect& rect);
	
	///直接插入数据到给定结点
	void insertToCurrentNode(const NodeGroupPtr& line_group, QuadNode* node);

	///递归查找当前四叉树结点
	void searchByRect(const cocos2d::Rect& search_rect, const QuadNode* node, std::vector<Edge>* result);

	///递归打印当前四叉树结点
	void printQuadTree(const QuadNode* node, std::string index);

	///判断当前结点及以下子结点是否为空
	bool isNull(const QuadNode* node);

	///判断射线是否与线段相交
	bool isRayIntersectSegment(const Line& ray, const Edge& segment);
	///判断射线与矩形是否相交
	bool isRayIntersectRect(const Line& ray, const cocos2d::Rect& rect);
private:
	QuadNode* m_root;
    std::vector<NodeGroupPtr> group_container_;
	unsigned int m_depth;
};

#endif // !QUAD_TREE_H