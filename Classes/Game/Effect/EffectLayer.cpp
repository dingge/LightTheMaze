#include "EffectLayer.h"

USING_NS_CC;

EffectLayer::EffectLayer():
	m_shadeRT(nullptr),
	m_isShine(false)
{
}

EffectLayer::~EffectLayer()
{
	m_shadeRT->retain();
	m_shadeRT->autorelease();//延迟释放，防止RenderCommand野指针
}

EffectLayer * EffectLayer::create()
{
	EffectLayer* ret = new (std::nothrow)EffectLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		delete ret;
		return nullptr;
	}
}

bool EffectLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		auto winsize = Director::getInstance()->getVisibleSize();
		m_shadeRT = RenderTexture::create((int)winsize.width,(int)winsize.height);
		m_shadeRT->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		m_shadeRT->setPosition(Vec2::ANCHOR_BOTTOM_LEFT);
		m_shadeRT->getSprite()->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		m_shadeRT->getSprite()->getTexture()->setAntiAliasTexParameters();
		addGLShader(m_shadeRT->getSprite());
		this->addChild(m_shadeRT,2);

		bRet = true;
	} while (0);
	return bRet;
}


void EffectLayer::drawEffectBegin()
{
	//暗层清屏(全透明)
	m_shadeRT->beginWithClear(0,0,0,0);
}

void EffectLayer::addLightEffect( RenderTexture* rt )
{
	//绘制
	rt->getSprite()->setBlendFunc(BlendFunc::ADDITIVE);
	rt->cocos2d::Node::visit();
}

void EffectLayer::drawEffectEnd()
{
	//绘制暗层
	m_shadeRT->end();

	//m_shadeRT->saveToFile("Map Area.png",Image::Format::PNG);
	//Director::getInstance()->end();
}

void EffectLayer::addGLShader( Node* node )
{
	const GLchar* const f = 
		"varying vec4 v_fragmentColor;  \n \
		varying vec2 v_texCoord;  \n \
		void main(void)  \n \
		{  \n \
		gl_FragColor = vec4(0,0,0,1.0-(v_fragmentColor * texture2D(CC_Texture0, v_texCoord)).a);  \n \
		}";
	auto fileUtils = FileUtils::getInstance();
	auto vertexSource = FileUtils::getInstance()->getStringFromFile(VSH_SHADER_PATH);
	auto fragmentSource = FileUtils::getInstance()->getStringFromFile(FSH_MAP_SHADER_PATH);
	log("%s",vertexSource.c_str());
	log("%s",fragmentSource.c_str());

	log("create shader begin"); 
	GLProgram* gl = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragmentSource.c_str());
	log("create shader finish");
	if(gl == nullptr)
		log("fail!");
	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION,GLProgram::VERTEX_ATTRIB_POSITION);
	log("bind position");
	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR,GLProgram::VERTEX_ATTRIB_COLOR);
	log("bind color");
	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD,GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	log("bind texturecoord");
	gl->link();
	log("link");
	gl->updateUniforms();
	log("update uniform");
	auto state = GLProgramState::getOrCreateWithGLProgram(gl);
	log("create shader end");
	//加入节点
	node->setGLProgramState(state);
	log("add shader to node");
}

bool EffectLayer::isPointInLight( const Vec2& worldPos )
{
	bool isInLight = false;
	Vec2 correctPos = m_shadeRT->convertToNodeSpace(worldPos);
	do 
	{
		CC_BREAK_IF(correctPos.x < 0 || correctPos.y < 0);

		//在3.0此处获取image必须写上这个,因为渲染是异步的
		//Director::getInstance()->getRenderer()->render();

		//以下代码改了RenderTexture的源码，为了拿到RenderTexture对应的缓冲区做颜色判断
		//要在RenderTexture类中添加public方法：
		//	inline GLuint getFBO()const {return _FBO;}
		GLubyte buffer[4] = {0,0,0,0};
		GLint oldFBO;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadeRT->getFBO());
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels((int)correctPos.x,(int)correctPos.y,1, 1,GL_RGBA,GL_UNSIGNED_BYTE, buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
		//alpha值大于某阈值则认为在光内
		if(buffer[3] > 10)
			isInLight = true;

	} while (0);
	
	return isInLight;
}

void EffectLayer::closeDark()
{
	m_isShine = true;
	m_shadeRT->getSprite()->runAction(Sequence::createWithTwoActions(
		FadeOut::create(3.0f),
		CallFunc::create([&](){this->setVisible(false);})
		));
}
