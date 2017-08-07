#include "QuadTree.h"

USING_NS_CC;

QuadTree::QuadTree( const Rect& rect ):
	m_root(nullptr)
{
	m_root = new QuadNode(rect);
	m_depth = 1;
}

QuadTree::~QuadTree( void )
{
	delete m_root;
}

void QuadTree::insert(const NodeGroupPtr& data, QuadNode* node, unsigned int depth)
{
	if (depth == QuadTreeOption::MaxDepth)
	{
		//直接插入
		insertToCurrentNode(data, node);
	}
	else
	{
		int index = getChildIndex(data, node->_rect);
        if (index == (int)QuadEnum::Overflow)
        {
            return;
        }
		if (index == (int)QuadEnum::Self)
		{
			//插入当前节点
			insertToCurrentNode(data, node);
		}
		else
		{
			//该子节点不存在，创建之
            if (node->_child[index] == nullptr)
            {
                node->_child[index] = new QuadNode(getRectByChildIndex(index, node->_rect));
            }
			//插入对应子节点，depth + 1
			insert(data, node->_child[index], depth + 1);
		}
	}
}

NodeGroupPtr QuadTree::findData(const Edge & data)
{
    //TODO 此处效率太低
// 	QuadNode* current_node;
// 	std::vector<QuadNode*> node_queue;
// 	node_queue.push_back(m_root);
// 
// 	do
// 	{
// 		current_node = node_queue.back();
// 		node_queue.pop_back();
// 
// 		if (current_node)
// 		{
// 			auto result = std::find(current_node->_node_group_list.begin(), current_node->_node_group_list.end(), data);
// 			if (result != current_node->_node_group_list.end())
// 			{
// 				return current_node;
// 			}
// 
// 			for (unsigned int i = 0; i < 4; ++i)
// 			{
// 				if (current_node->_child[i])
// 				{
// 					node_queue.push_back(current_node->_child[i]);
// 				}
// 			}
// 		}
// 	} while (0 != node_queue.size());
    for (auto it : group_container_)
    {
        ListNode<Edge>* p = it->findNode(data);
        if (p)
        {
            return it;
        }
    }
	return nullptr;
}

void QuadTree::insert(const NodeGroupPtr& data)
{
	insert(data, m_root, m_depth);
}

void QuadTree::remove(const NodeGroupPtr & data)
{
    std::vector<NodeGroupPtr>& group_list = data->quad_node_ref_->_node_group_list;
    group_list.erase(std::find_if(group_list.begin(), group_list.end(), [&data](const NodeGroupPtr& p) {
        return p.get() == data.get();
    }));
}

void QuadTree::removeByGroup(int group_name)
{
}

void QuadTree::searchByRect(const cocos2d::Rect & search_rect, std::vector<Edge>* result)
{
	searchByRect(search_rect, m_root, result);

//	//非递归解法，实测效率比递归要低
// 	QuadNode<Line>* current_node;
// 	std::vector<QuadNode<Line>*> node_queue;
// 	node_queue.push_back(m_root);
// 
// 	do
// 	{
// 		current_node = node_queue.back();
// 		node_queue.pop_back();
// 
// 		if (current_node &&  current_node->_rect.intersectsRect(search_rect))
// 		{
// 			result->insert(result->end(), current_node->_node_group_list.begin(), current_node->_node_group_list.end());
// 
// 			for (unsigned int i = 0; i < 4; ++i)
// 			{
// 				if (current_node->_child[i])
// 				{
// 					node_queue.push_back(current_node->_child[i]);
// 				}
// 			}
// 		}
// 	} while (0 != node_queue.size());
}

void QuadTree::searchByRay(const Line & ray, std::vector<Edge>* result)
{
    //未处理
// 	QuadNode* current_node;
// 	std::vector<QuadNode*> node_queue;
// 	node_queue.push_back(m_root);
// 
// 	do
// 	{
// 		current_node = node_queue.back();
// 		node_queue.pop_back();
// 
// 		if (current_node &&  isRayIntersectRect(ray, current_node->_rect))
// 		{
// 			result->insert(result->end(), current_node->_node_group_list.begin(), current_node->_node_group_list.end());
// 
// 			for (unsigned int i = 0; i < 4; ++i)
// 			{
// 				if (current_node->_child[i])
// 				{
// 					node_queue.push_back(current_node->_child[i]);
// 				}
// 			}
// 		}
// 	} while (0 != node_queue.size());
}

void QuadTree::alter(const NodeGroupPtr & data)
{
	//如果已经不属于该象限，则移除再从新加入四叉树
	if (getChildIndex(data, data->quad_node_ref_->_rect) != (int)QuadEnum::Self)
	{
        std::vector<NodeGroupPtr>& group_list = data->quad_node_ref_->_node_group_list;
        group_list.erase(std::find_if(group_list.begin(), group_list.end(), [&data](const NodeGroupPtr& p) {
            return p.get() == data.get();
        }));
		insert(data);
    }
}

int QuadTree::getChildIndex(const cocos2d::Vec2 & pos, const cocos2d::Rect & rect)
{
	Vec2 origin = rect.origin;
	Size size = rect.size;

	if (pos.x < origin.x + size.width / 2.0f && pos.y > origin.y + size.height / 2.0f)
	{
		return (int)QuadEnum::UL;
	}
	if (pos.x > origin.x + size.width / 2.0f && pos.y > origin.y + size.height / 2.0f)
	{
		return (int)QuadEnum::UR;
	}
	if (pos.x < origin.x + size.width / 2.0f && pos.y < origin.y + size.height / 2.0f)
	{
		return (int)QuadEnum::DL;
	}
	if (pos.x > origin.x + size.width / 2.0f && pos.y < origin.y + size.height / 2.0f)
	{
		return (int)QuadEnum::DR;
	}
	if (rect.containsPoint(pos))
	{
		return (int)QuadEnum::Self;
	}
	return (int)QuadEnum::Overflow;
}

int QuadTree::getChildIndex( const NodeGroupPtr& line_group, const Rect& rect )
{
    Rect group_rect = line_group->rect_;
	int p1_index = (int)getChildIndex(group_rect.origin, rect);
    int p2_index = (int)getChildIndex(group_rect.origin + Vec2(group_rect.size), rect);
	
	if (p1_index == p2_index)
	{
		return p1_index;
	}
	else if (p1_index != (int)QuadEnum::Overflow && p2_index != (int)QuadEnum::Overflow)
	{
		return (int)QuadEnum::Self;
	}
	else
	{
		return  (int)QuadEnum::Overflow;
	}
}

cocos2d::Rect QuadTree::getRectByChildIndex(int index, const cocos2d::Rect & rect)
{
    Vec2 origin = rect.origin;
    Size size = rect.size;
    switch (index)
    {
    case (int)QuadEnum::UL:
        return Rect(origin.x, origin.y + size.height / 2.0f, size.width / 2.0f, size.height / 2.0f);
        break;
    case (int)QuadEnum::UR:
        return  Rect(origin.x + size.width / 2.0f, origin.y + size.height / 2.0f, size.width / 2.0f, size.height / 2.0f);
        break;
    case (int)QuadEnum::DL:
        return Rect(origin.x, origin.y, size.width / 2.0f, size.height / 2.0f);
        break;
    case  (int)QuadEnum::DR:
        return Rect(origin.x + size.width / 2.0f, origin.y, size.width / 2.0f, size.height / 2.0f);
        break;
    case (int)QuadEnum::Self:
        return rect;
        break;
    default:
        return Rect();
        break;
    }
}

void QuadTree::insertToCurrentNode(const NodeGroupPtr & line_group, QuadNode* node)
{
    line_group->quad_node_ref_ = node;
    node->_node_group_list.push_back(line_group);
}

void QuadTree::searchByRect(const cocos2d::Rect & search_rect, const QuadNode* node, std::vector<Edge>* result)
{
	if (node  == nullptr || false == node->_rect.intersectsRect(search_rect))
	{
		return;
	}

	searchByRect(search_rect, node->_child[(int)QuadEnum::UL], result);
	searchByRect(search_rect, node->_child[(int)QuadEnum::UR], result);
	searchByRect(search_rect, node->_child[(int)QuadEnum::DL], result);
	searchByRect(search_rect, node->_child[(int)QuadEnum::DR], result);

    for (auto it : node->_node_group_list)
    {
        ListNode<Edge>* p = it->head_;
        while (p != nullptr)
        {
            result->push_back(p->_node);
            p = p->_next;
        }
    }
	//result->insert(result->end(), node->_node_group_list.begin(), node->_node_group_list.end());
}

void QuadTree::printQuadTree()
{
	printQuadTree(m_root, "");
}

void QuadTree::printQuadTree(const QuadNode* node, std::string index)
{
	if (node == nullptr)
	{
		return;
	}

    if (node->_node_group_list.size() == 0)
    {
        log("index: %s is null", index.c_str());
    }
    else
    {
        for (auto it : node->_node_group_list)
        {
            ListNode<Edge>* p = it->head_;
            while (p != nullptr)
            {
                log("index: %s  p1(%.1f, %.1f)  p2(%.1f, %.1f)",
                    index.c_str(), p->_node._p1.x, p->_node._p1.y, p->_node._p2.x, p->_node._p2.y);
                p = p->_next;
            }
            
        }
    }

	printQuadTree(node->_child[(int)QuadEnum::UL], index + "0");
	printQuadTree(node->_child[(int)QuadEnum::UR], index + "1");
	printQuadTree(node->_child[(int)QuadEnum::DL], index + "2");
	printQuadTree(node->_child[(int)QuadEnum::DR], index + "3");
}

bool QuadTree::isNull(const QuadNode* node)
{
	if (nullptr == node)
	{
		return true;
	}
	else
	{
		return isNull(node->_child[(int)QuadEnum::UL]) &&
			isNull(node->_child[(int)QuadEnum::UR]) &&
			isNull(node->_child[(int)QuadEnum::DL]) &&
			isNull(node->_child[(int)QuadEnum::DR]) &&
			node->_node_group_list.size() == 0;
	}
}

bool QuadTree::isRayIntersectSegment(const Line& ray, const Edge& segment)
{
	float s, t;
	if (Vec2::isLineIntersect(ray._p1, ray._p2, segment._p1, segment._p2, &s, &t))
	{
		if (s > 0 && t > 0 && t < 1)
			return true;
	}
	return false;
}

bool QuadTree::isRayIntersectRect(const Line& ray, const cocos2d::Rect& rect)
{
	if (rect.containsPoint(ray._p1) || rect.containsPoint(ray._p2))
	{
		return true;
	}

	Line l1(rect.getMinX(), rect.getMaxY(), rect.getMaxX(), rect.getMinY());
	Line l2(rect.getMinX(), rect.getMinY(), rect.getMaxX(), rect.getMaxY());
	return isRayIntersectSegment(ray, l1) || isRayIntersectSegment(ray, l2);
}
