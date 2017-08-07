#include "TestScene.h"
#include "Game/Extension/QuadTree.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio::timeline;

Scene* TestLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TestLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TestLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();

    auto bg = LayerColor::create(Color4B::WHITE, visibleSize.width, visibleSize.height);
    this->addChild(bg);

	draw_node_ = DrawNodeSpec::create();
	draw_node_->drawPoint(Vec2(visibleSize.width / 2, visibleSize.height / 2), 10, Color4F::RED);
	draw_node_->drawSegment(
		Vec2(visibleSize.width*0.25, visibleSize.height / 4*3), Vec2(visibleSize.width*0.75f, visibleSize.height / 4),
		50,Color4F::BLACK);
	draw_node_->drawLine(
		Vec2(visibleSize.width*0.25+200, visibleSize.height / 4 * 3), Vec2(visibleSize.width*0.75f + 200, visibleSize.height / 4), Color4F::BLACK);
	draw_node_->drawTriangle(Vec2(10.0,10.0), Vec2(200.0,200.0), Vec2(400.0,10.0), Color4F::BLACK);
    draw_node_->drawGradualTriangle(Vec2(50.0, 50.0), Vec2(250.0, 250.0), Vec2(450.0, 50.0), Color4F::BLACK);
    draw_node_->drawGradualRectangle(Vec2(500.0, 500.0), Vec2(500.0, 700.0), Vec2(700.0, 700.0), Vec2(700.0, 500.0), Color4F::BLACK);
	draw_node_->retain();
    this->addChild(draw_node_);

// 	Sprite* sprite = Sprite::create("test.png");
// 	sprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
// 	this->addChild(sprite);
// 
// 	RenderTexture* rt = RenderTexture::create(visibleSize.width, visibleSize.height);
// 	rt->setPosition(Vec2::ZERO);
// 	rt->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
// 	rt->getSprite()->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
// 	rt->getSprite()->getTexture()->setAntiAliasTexParameters();
// 	this->addChild(rt);
// 
// 	rt->beginWithClear(255, 255, 255, 255);
// 	//draw_node_->visit();
// 	sprite->visit();
// 	rt->end();
// 	Director::getInstance()->getRenderer()->render();
// 
// 	GLProgram* gl = GLProgram::createWithFilenames("shader/common.vert", "shader/test.frag");
// 	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
// 	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
// 	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
// 	gl->link();
// 	gl->updateUniforms();
// 
// 	auto state = GLProgramState::getOrCreateWithGLProgram(gl);
// 	//设置像素的size
// 	Size size = rt->getSprite()->getTexture()->getContentSizeInPixels();
// 	state->setUniformVec2("u_resolution", Vec2(size.width, size.height));
// 	state->setUniformVec2("u_focus_pos", Vec2(size.width/2.0, size.height/2.0));
// 	//加入节点
// 	rt->getSprite()->setGLProgramState(state);
// 	rt->saveToFile("result.png");

// 	const size_t pixel_width = 64;
// 	GLubyte buffer[pixel_width * pixel_width * 4];
// 	for (size_t i = 0; i < pixel_width * pixel_width * 4; i = i + 4)
// 	{
// 		buffer[i] = 0;
// 		buffer[i + 1] = 0;
// 		buffer[i + 2] = 255;
// 		buffer[i + 3] = 255;
// 	}
// 	GLint oldFBO;
// 	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
// 	glBindFramebuffer(GL_FRAMEBUFFER, rt->getFBO());
// 	glPixelStorei(GL_PACK_ALIGNMENT, 1);
// 	glRasterPos2i(0, 0);
// 	glDrawPixels(pixel_width, pixel_width, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
// 	//glReadPixels((int)correctPos.x, (int)correctPos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
// 	glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
// 	Director::getInstance()->getRenderer()->render();

// 	QuadTree tree(Rect(0,0,visibleSize.width,visibleSize.height));
// 
// 	tree.insert(Line(0, 0, visibleSize.width, visibleSize.height));
// 	tree.insert(Line(0, 0, visibleSize.width / 2, visibleSize.height / 2));
// 	tree.insert(Line(0, 0, visibleSize.width / 4, visibleSize.height / 4));
// 	tree.insert(Line(0, 0, visibleSize.width / 8, visibleSize.height / 8));
// 	tree.insert(Line(0, 0, visibleSize.width / 16, visibleSize.height / 16));
// 	tree.insert(Line(0, 0, visibleSize.width / 32, visibleSize.height / 32));
// 	tree.insert(Line(visibleSize.width / 4, visibleSize.height / 4, visibleSize.width / 4 * 3, visibleSize.height / 4));
// 	tree.insert(Line(visibleSize.width/4+18, visibleSize.height/4*3+10, visibleSize.width / 4+20, visibleSize.height / 4*3+10));
// 
// 	tree.printQuadTree();
// 
// 	std::vector<Line> result;
// 	tree.searchByRect(Rect(0,visibleSize.height/2,visibleSize.width/2,visibleSize.height/2), &result);
// 	for (auto it : result)
// 	{
// 		log("(%.1f, %.1f)---(%.1f, %.1f)", it._p1.x, it._p1.y, it._p2.x, it._p2.y);
// 	}

    return true;
}

void TestLayer::onExit()
{
	Layer::onExit();

	draw_node_->release();
}
