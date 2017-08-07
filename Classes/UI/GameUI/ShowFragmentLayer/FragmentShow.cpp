#include "FragmentShow.h"

USING_NS_CC;

const char* fragment_gray_frag = "shader/gray.frag";

FragmentShow* FragmentShow::create(const std::vector<LightFragmentNode*>& fragmentVec)
{
	FragmentShow *pRet = new FragmentShow();
	if (pRet && pRet->init(fragmentVec))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}


void FragmentShow::createbyNumber(const std::vector<LightFragmentNode*>& fragmentVec)
{
    Size winSize = Director::getInstance()->getWinSize();
	Vec2 origin = Vec2(0.0f, winSize.height*0.1f);
	Size visibleSize = Size(winSize.width, winSize.height*0.8f);
    auto childLayer = LayerColor::create(Color4B(200, 213, 243, 50), visibleSize.width, visibleSize.height);
    childLayer->setPosition(origin);
    this->addChild(childLayer);

	switch (m_number)
	{
	case 1:
		addSprite(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 2.0f, fragmentVec[0]);
		break;
	case 2:
		addSprite(origin.x + visibleSize.width / 4.0f, origin.y + visibleSize.height / 2.0f, fragmentVec[0]);
		addSprite(origin.x + visibleSize.width * 0.75f, origin.y + visibleSize.height / 2.0f, fragmentVec[1]);
		break;
	case 3:
		addSprite(origin.x + visibleSize.width / 3.0f, origin.y + visibleSize.height / 3.0f, fragmentVec[0]);
		addSprite(origin.x + visibleSize.width / 3.0f * 2.0f, origin.y + visibleSize.height / 3.0f, fragmentVec[1]);
		addSprite(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 3.0f * 2.0f, fragmentVec[2]);
		break;
	case 4:
		addSprite(origin.x + visibleSize.width / 3.0f, origin.y + visibleSize.height / 3.0f, fragmentVec[0]);
		addSprite(origin.x + visibleSize.width / 3.0f * 2.0f, origin.y + visibleSize.height / 3.0f, fragmentVec[1]);
		addSprite(origin.x + visibleSize.width / 3.0f, origin.y + visibleSize.height / 3.0f * 2.0f, fragmentVec[2]);
		addSprite(origin.x + visibleSize.width / 3.0f * 2.0f, origin.y + visibleSize.height / 3.0f * 2.0f, fragmentVec[3]);
		break;
	case 5:
		addSprite(origin.x + visibleSize.width / 3.0f, origin.y + visibleSize.height / 4.0f, fragmentVec[0]);
		addSprite(origin.x + visibleSize.width / 3.0f * 2.0f, origin.y + visibleSize.height / 4.0f, fragmentVec[1]);
		addSprite(origin.x + visibleSize.width / 4.0f, origin.y + visibleSize.height * 0.6f, fragmentVec[2]);
		addSprite(origin.x + visibleSize.width * 0.75f, origin.y + visibleSize.height * 0.6f, fragmentVec[3]);
		addSprite(origin.x + visibleSize.width / 2.0f, origin.y + visibleSize.height / 6.0f * 5.0f, fragmentVec[4]);
		break;
	default:
		break;
	}
}


void FragmentShow::addSprite(float x, float y,const LightFragmentNode* p)
{
	Sprite* sp = nullptr; 
	sp = Sprite::create(p->getLargeTextureFileName());

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//此处为比例放缩，根据所选素材自行调试
	sp->setScale(visibleSize.height / 4.0f / sp->getContentSize().height);

	sp->setAnchorPoint(Vec2(0.5, 0.5));
	sp->setPosition(Vec2(x,y));

	if (p->isFragmentCollected() == false)
	{
		addGLShader(sp);
	}
	else
	{
		sp->setColor(p->getColor());
	}
	
	this->addChild(sp);
}

bool FragmentShow::init(const std::vector<LightFragmentNode*>& fragmentVec)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		m_number = fragmentVec.size();

		createbyNumber(fragmentVec);

		auto keyboardlistener = EventListenerKeyboard::create();
		keyboardlistener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){};
		keyboardlistener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event){
			if(keyCode == EventKeyboard::KeyCode::KEY_M || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
				Director::getInstance()->popScene();
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener,this);

		bRet = true;
	} while (0);
	return bRet;
}

void FragmentShow::addGLShader( Node* node )
{
	auto fstr = FileUtils::getInstance()->getStringFromFile(fragment_gray_frag);
	GLProgram* gl = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,(GLchar*)(fstr.c_str()));
	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION,GLProgram::VERTEX_ATTRIB_POSITION);
	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR,GLProgram::VERTEX_ATTRIB_COLOR);
	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD,GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	gl->link();
	gl->updateUniforms();

	auto state = GLProgramState::getOrCreateWithGLProgram(gl);
	//加入节点
	node->setGLProgramState(state);
}

