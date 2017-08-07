#include "LightNode.h"
#include "Game/Light/Action/LightRadiusAction.h"
#include "Game/Light/Action/LightColorAction.h"
#include "Game/Dark/DarkSprite.h"
#include "Game/GameManage.h"
#include "Setting.h"
#include "SimpleAudioEngine.h"

#ifdef GAME_DEBUG && CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "Test/TestTimeElapsed.h"
#define TIME_ELAPSED_BEGIN(s) TestTimeElapsed::getInstance()->start(s);
#define TIME_ELAPSED_END(s) TestTimeElapsed::getInstance()->end(s);
#else
#define TIME_ELAPSED_BEGIN(s) do {} while (0);
#define TIME_ELAPSED_END(s) do {} while (0);
#endif

USING_NS_CC;
using namespace CocosDenshion;

//----------------------------------LightShadow----------------------------------
LightNode::LightNode():
	m_lightDraw(nullptr),
    m_translucentDraw(nullptr),
    m_shadowDraw(nullptr),
	m_lightPos(Vec2(0,0)),
    m_lightRadius(10.0f),
	m_lightMaxRangeRadius(LIGHT_STANDRAD_RADIUS),
	m_lightColor(Color4F(1.0,1.0,1.0,1.0)),
	m_rt(nullptr)
{
}

LightNode::~LightNode()
{
	m_lightDraw->autorelease();
    m_translucentDraw->autorelease();
    m_shadowDraw->autorelease();

	m_rt->autorelease();//延迟释放，防止RenderCommand野指针
}

bool LightNode::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		m_lightDraw = DrawNodeSpec::create();
		m_lightDraw->retain();

        m_translucentDraw = DrawNodeSpec::create();
        m_translucentDraw->retain();

        m_shadowDraw = DrawNodeSpec::create();
        m_shadowDraw->retain();

		auto winsize = Director::getInstance()->getVisibleSize();
		m_rt = RenderTexture::create((int)winsize.width, (int)winsize.height);
		m_rt->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		m_rt->setPosition(Vec2::ZERO);
		m_rt->getSprite()->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		m_rt->getSprite()->getTexture()->setAntiAliasTexParameters();
		addGLShader(m_rt->getSprite());
		m_rt->getSprite()->setBlendFunc(BlendFunc::ADDITIVE);
		m_rt->retain();
		this->addChild(m_rt);

		//待机动作
// 		auto action1 = LightRadiusScaleBy::create(20,0.5);
// 		auto action2 = LightRadiusScaleBy::create(-20,0.5);
// 		this->runAction(RepeatForever::create(Sequence::createWithTwoActions(action1,action2)));

		bRet = true; 
	} while (0);
	return bRet;
}

static bool FasterRayAndSegmentIntersection(const Line& ray, const Line& segment, float* len) {

    const Vec2 a = ray._p2 - ray._p1;
    const Vec2 b = segment._p1 - segment._p2;
    const Vec2 c = ray._p1 - segment._p1;

    float alphaNumerator = b.y*c.x - b.x*c.y;    //e
    float betaNumerator = a.x*c.y - a.y*c.x;     //d
    float denominator = a.y*b.x - a.x*b.y;   //f

    bool doIntersect = true;

    if (denominator == 0) {
        doIntersect = false;
    }
    else {
        if (denominator > 0) {
            if (alphaNumerator < 0 || alphaNumerator > denominator || betaNumerator < 0 || betaNumerator > denominator)
            {
                doIntersect = false;
            }
        }
        else if (alphaNumerator > 0 || alphaNumerator < denominator || betaNumerator > 0 || betaNumerator < denominator)
        {
            doIntersect = false;
        }
        
        if (doIntersect && len != nullptr)
        {
            *len = alphaNumerator / denominator;
        }
    }

    return doIntersect;
}

static bool linesIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double* s) {
    // Return false if either of the lines have zero length
    if (x1 == x2 && y1 == y2 ||
        x3 == x4 && y3 == y4) {
        return false;
    }
    // Fastest method, based on Franklin Antonio's "Faster Line Segment Intersection" topic "in Graphics Gems III" book (http://www.graphicsgems.org/)
    double ax = x2 - x1;
    double ay = y2 - y1;
    double bx = x3 - x4;
    double by = y3 - y4;
    double cx = x1 - x3;
    double cy = y1 - y3;

    double alphaNumerator = by*cx - bx*cy;
    double commonDenominator = ay*bx - ax*by;
    if (commonDenominator > 0) {
        if (alphaNumerator < 0 || alphaNumerator > commonDenominator) {
            return false;
        }
    }
    else if (commonDenominator < 0) {
        if (alphaNumerator > 0 || alphaNumerator < commonDenominator) {
            return false;
        }
    }
    double betaNumerator = ax*cy - ay*cx;
    if (commonDenominator > 0) {
        if (betaNumerator < 0 || betaNumerator > commonDenominator) {
            return false;
        }
    }
    else if (commonDenominator < 0) {
        if (betaNumerator > 0 || betaNumerator < commonDenominator) {
            return false;
        }
    }
    if (commonDenominator == 0) {
        return false;
    }
    if (s!=nullptr)
    {
        *s = alphaNumerator / commonDenominator;
    }
    return true;
}
static bool linesIntersect(const Line& ray, const Line& segment, double ray_len, double* s)
{
    bool is_intersect = linesIntersect(ray._p1.x, ray._p1.y, ray._p2.x, ray._p2.y, segment._p1.x, segment._p1.y, segment._p2.x, segment._p2.y, s);
    *s *= ray_len;
    return is_intersect;
}

void LightNode::createLight()
{
	//重建角集合
    TIME_ELAPSED_BEGIN("createShapeAngle");
	createShapeAngle();
    TIME_ELAPSED_END("createShapeAngle");

    TIME_ELAPSED_BEGIN("loop angle");
	//计算每个角光线能到达的坐标
	m_lightVec.clear();
	int angleVecLen = m_shapeAngleVec.size();
    double templen;
    double len;
    Line ray(m_lightPos.x, m_lightPos.y, m_lightPos.x, m_lightPos.y);
    for (auto i = 0; i < angleVecLen; ++i)
	{
        ray._p2.x = m_lightPos.x + m_lightMaxRangeRadius*cos(m_shapeAngleVec[i]);
        ray._p2.y = m_lightPos.y + m_lightMaxRangeRadius*sin(m_shapeAngleVec[i]);
	    len = MAX_NUMBER;

        TIME_ELAPSED_BEGIN("loop line");
		//以下改为C风格的遍历，效率高好多
		int vecLen = m_shapeVec.size();
        for (auto j = 0; j < vecLen; ++j)
		{
            if (linesIntersect(ray, m_shapeVec[j], m_lightMaxRangeRadius, &templen) && templen < len)
            {
                len = templen;
            }
		}
        TIME_ELAPSED_END("loop line");

		//这里len可能会等于MAX_NUMBER，因为前面加了窗口的虚框四角的ray，但实际上那里并不存在shape
		//但是不会影响最终绘制结果，但len太大会不会影像绘制效率暂时未知，暂时不作处理。
 		m_lightVec.push_back(
 			convertToScreenPos(m_lightPos) + Vec2(len*cos(m_shapeAngleVec[i]),len*sin(m_shapeAngleVec[i])));
	}
    TIME_ELAPSED_END("loop angle");
}

void LightNode::renderLight()
{
	//绘制亮区多边形
    TIME_ELAPSED_BEGIN("drawConcavePolygons");
	//drawConcavePolygons(convertToScreenPos(m_lightPos),m_lightColor);
    TIME_ELAPSED_END("drawConcavePolygons");

    //绘制透明区域
    drawTranslucentAndShadow();

	//Shader处理
    TIME_ELAPSED_BEGIN("handleTexture");
	handleTexture(m_lightMaxRangeRadius);
    TIME_ELAPSED_END("handleTexture");
}

bool LightNode::isCross(const Line& ray,const Line& segment,Vec2* crossPos,float* len)
{
	float s,t;
    if (Vec2::isLineIntersect(ray._p1, ray._p2, segment._p1, segment._p2, &s, &t) && s >= 0 && t >= 0 && t <= 1)
	{
        //*crossPos = ray._p1 + (ray._p2 - ray._p1)*s;
		*len = s;
		return true;
	}
	return false;
}

void LightNode::createShapeAngle()
{
	m_shapeAngleVec.clear();
	//额外测试点偏移角度
	const float offsetAngle = 0.001f;
	unsigned int vecLen = m_shapeVec.size();
	for (unsigned int i = 0; i < vecLen; ++i)
	{
		//左端不连续点，需要添加两根光线(分别左右偏移一个小角度，因为精度误差使得该角度与该线段在边缘的相交检测可能失败)
        if (i == 0 || (m_shapeVec[i]._p1 - m_shapeVec[i - 1]._p2).getLengthSq() >  0.1f)
		{
			float p1Angle = Vec2(m_shapeVec[i]._p1 - m_lightPos).getAngle();
			m_shapeAngleVec.push_back(p1Angle + offsetAngle);
			m_shapeAngleVec.push_back(p1Angle - offsetAngle);
		}
        float p2Angle = Vec2(m_shapeVec[i]._p2 - m_lightPos).getAngle();
		//右端不连续点，需要添加两根光线(做法同上)
		if (i == vecLen - 1|| (m_shapeVec[i]._p2 - m_shapeVec[i + 1]._p1).getLengthSq() > 0.1f)
		{
			m_shapeAngleVec.push_back(p2Angle + offsetAngle);
			m_shapeAngleVec.push_back(p2Angle - offsetAngle);
		}
        else
        {
            //右端连续点，只需添加一根光线
            m_shapeAngleVec.push_back(p2Angle);
        }
	}

	//一定补上屏幕四个角，保证显示正常
	auto winsize = Director::getInstance()->getWinSize();
	m_shapeAngleVec.push_back((Vec2(0,0)-convertToScreenPos(m_lightPos)).getAngle());
	m_shapeAngleVec.push_back((Vec2(0,winsize.height)-convertToScreenPos(m_lightPos)).getAngle());
	m_shapeAngleVec.push_back((Vec2(winsize.width,0)-convertToScreenPos(m_lightPos)).getAngle());
	m_shapeAngleVec.push_back((Vec2(winsize.width,winsize.height)-convertToScreenPos(m_lightPos)).getAngle());

	//按照角度重新排列，保证后面生成正确的亮区多边形
	std::sort(m_shapeAngleVec.begin(),m_shapeAngleVec.end());
}

void LightNode::drawConcavePolygons(const Vec2& screenPos,Color4F color)
{
    m_lightDraw->clear();

#ifdef GAME_DEBUG
    //绘制各光线的终点
    //TODO 似乎有绘制终点错误，但并未影响多边形绘制，待检查
    m_lightDraw->drawCircle(screenPos, m_lightMaxRangeRadius, 3.14f, 20, false, Color4F::YELLOW);
    for (auto it : m_lightVec)
    {
        m_lightDraw->drawLine(screenPos, it, Color4F::RED);
        m_lightDraw->drawDot(it, 5, Color4F(1, 0.5, 1, 1));
    }
    for (auto it : m_shapeVec)
    {
        m_lightDraw->drawSegment(convertToScreenPos(it._p1), convertToScreenPos(it._p2), 10, Color4F::GREEN);
    }
#endif // GAME_DEBUG

	//因为亮区多边形可能是凹多边形，而不能通过opengl绘制多边形的方法进行绘制。
	//可以发现规律，由于光的直线传播，光照亮区多边形所有顶点到光源的连线都不会与多边形任意一条边相交（可能重合）
	//因此可以按顺序取一条边与光源组成的三角形绘制，遍历一次后闭合开口即可。
	//注意切割的三角形内角不能大于180度，直接忽略绘制，否则产生凹多边形错误
	unsigned int i = 0;
	auto vecLen = m_lightVec.size();
	if (vecLen < 2)
	{
		return;
	}
	Vec2 aPos = m_lightVec[i];
	Vec2 bPos;
	do
	{
		++i;
		bPos = m_lightVec[i];
		if ((aPos - screenPos).getAngle(bPos - screenPos) > 0.0f &&
			false == Vec2::isLineParallel(screenPos, aPos, bPos, aPos))
			m_lightDraw->drawTriangle(screenPos,aPos,bPos,color);
		aPos = bPos;
	}while((i+1) <vecLen);
	if((m_lightVec[i] - screenPos).getAngle(*m_lightVec.begin() - screenPos) > 0)
		m_lightDraw->drawTriangle(screenPos,m_lightVec[i],m_lightVec[0],color);
}

void LightNode::drawTranslucentAndShadow()
{
    //这里将建立阴影区蒙版
    m_translucentDraw->clear();
    m_shadowDraw->clear();

    const unsigned int vecLen = m_shapeVec.size();
    Vec2 pre_umbra_r;
    for (unsigned int i = 0; i < vecLen; ++i)
    {
//         //左端不连续点
//         if ((m_shapeVec[i]._p1 - m_shapeVec[(i+vecLen - 1)%vecLen]._p2).getLengthSq() >  0.1f)
//         {
//             const Vec2 line = m_shapeVec[i]._p2 - m_shapeVec[i]._p1;
//             const Vec2 light_l = m_lightPos + line.getNormalized()*m_lightRadius;
//             const Vec2 light_r = m_lightPos - line.getNormalized()*m_lightRadius;
//             const Vec2 ray_umbra_r = light_r - m_shapeVec[i]._p1;
//             const Vec2 ray_penumbra_r = light_l - m_shapeVec[i]._p1;
//             const Vec2 umbra_r = convertToScreenPos(m_lightPos) + (-ray_umbra_r).getNormalized()*MAX_NUMBER;
//             const Vec2 penumbra_r = convertToScreenPos(m_lightPos) + (-ray_penumbra_r).getNormalized()*MAX_NUMBER;
//             Color4F color = Color4F(m_shapeVec[i].getColor());
//             Color4F shadow_color = Color4F(color.r*(1.0f - color.a), color.g*(1.0f - color.a), color.b*(1.0f - color.a), color.a);
//             color.a = 1.0f - color.a;
//             m_translucentDraw->drawTriangle(convertToScreenPos(m_shapeVec[i]._p1), convertToScreenPos(m_shapeVec[i]._p2), umbra_r, color);
//             m_translucentDraw->drawGradualTriangle(convertToScreenPos(m_shapeVec[i]._p1), umbra_r, penumbra_r, m_lightColor);
//             //m_shadowDraw->drawGradualTriangle(convertToScreenPos(m_shapeVec[i]._p1), penumbra_r, umbra_r, shadow_color);
//             pre_umbra_r = umbra_r;
//         }
//         else
//         {
//             const Vec2 c = convertToScreenPos(m_lightPos) + (m_shapeVec[i]._p1 - m_lightPos).getNormalized()*MAX_NUMBER;
//             Color4F color = Color4F(m_shapeVec[i].getColor());
//             color.a = 1.0f - color.a;
//             m_translucentDraw->drawTriangle(convertToScreenPos(m_shapeVec[i]._p1), convertToScreenPos(m_shapeVec[i]._p2), c, color);
//             pre_umbra_r = c;
//         }
// 
//         //右端不连续点
//         if ((m_shapeVec[i]._p2 - m_shapeVec[(i + 1)%vecLen]._p1).getLengthSq() > 0.1f)
//         {
//             const Vec2 line = m_shapeVec[i]._p2 - m_shapeVec[i]._p1;
//             const Vec2 light_l = m_lightPos + line.getNormalized()*m_lightRadius;
//             const Vec2 light_r = m_lightPos - line.getNormalized()*m_lightRadius;
//             const Vec2 ray_umbra_l = light_l - m_shapeVec[i]._p2;
//             const Vec2 ray_penumbra_l = light_r - m_shapeVec[i]._p2;
//             const Vec2 umbra_l = convertToScreenPos(m_lightPos) + (-ray_umbra_l).getNormalized()*MAX_NUMBER;
//             const Vec2 penumbra_l = convertToScreenPos(m_lightPos) + (-ray_penumbra_l).getNormalized()*MAX_NUMBER;
//             Color4F color = Color4F(m_shapeVec[i].getColor());
//             Color4F shadow_color = Color4F(color.r*(1.0f - color.a), color.g*(1.0f - color.a), color.b*(1.0f - color.a), color.a);
//             color.a = 1.0f - color.a;
//             m_translucentDraw->drawTriangle(convertToScreenPos(m_shapeVec[i]._p2), umbra_l, pre_umbra_r, color);
//             m_translucentDraw->drawGradualTriangle(convertToScreenPos(m_shapeVec[i]._p2), umbra_l, penumbra_l, m_lightColor);
//             //m_shadowDraw->drawGradualTriangle(convertToScreenPos(m_shapeVec[i]._p2), penumbra_l, umbra_l, shadow_color);
//         }
//         else
//         {
//             //右端连续点
//             const Vec2 d = convertToScreenPos(m_lightPos) + (m_shapeVec[i]._p2 - m_lightPos).getNormalized()*MAX_NUMBER;
//             Color4F color = Color4F(m_shapeVec[i].getColor());
//             color.a = 1.0f - color.a;
//             m_translucentDraw->drawTriangle(convertToScreenPos(m_shapeVec[i]._p2), pre_umbra_r, d, color);
//             m_translucentDraw->drawTriangle(convertToScreenPos(m_shapeVec[i]._p2), d, convertToScreenPos(m_shapeVec[(i + 1) % vecLen]._p2), color);
//             pre_umbra_r = d;
//         }
        const Vec2 c = convertToScreenPos(m_lightPos) + (m_shapeVec[i]._p1 - m_lightPos).getNormalized()*MAX_NUMBER;
        const Vec2 d = convertToScreenPos(m_lightPos) + (m_shapeVec[i]._p2 - m_lightPos).getNormalized()*MAX_NUMBER;
        Color4F color = Color4F(m_shapeVec[i].getColor());
        color.a = 1.0f - color.a;
        m_translucentDraw->drawTriangle(convertToScreenPos(m_shapeVec[i]._p1), convertToScreenPos(m_shapeVec[i]._p2), c, color);
        m_translucentDraw->drawTriangle(convertToScreenPos(m_shapeVec[i]._p2), d, c, color);
        //左端不连续点
        if ((m_shapeVec[i]._p1 - m_shapeVec[(i+vecLen - 1)%vecLen]._p2).getLengthSq() >  0.1f)
        {
            const Vec2 line = m_shapeVec[i]._p2 - m_shapeVec[i]._p1;
            const Vec2 light_l = m_lightPos + line.getNormalized()*m_lightRadius;
            const Vec2 light_r = m_lightPos - line.getNormalized()*m_lightRadius;
            const Vec2 ray_umbra_r = light_r - m_shapeVec[i]._p1;
            const Vec2 ray_penumbra_r = light_l - m_shapeVec[i]._p1;
            const Vec2 umbra_r = convertToScreenPos(m_lightPos) + (-ray_umbra_r).getNormalized()*MAX_NUMBER;
            const Vec2 penumbra_r = convertToScreenPos(m_lightPos) + (-ray_penumbra_r).getNormalized()*MAX_NUMBER;
            Color4F color = Color4F(m_shapeVec[i].getColor());
            Color4F shadow_color = Color4F(color.r*(1.0f - color.a), color.g*(1.0f - color.a), color.b*(1.0f - color.a), color.a);
            color.a = 1.0f - color.a;
            m_translucentDraw->drawGradualTriangle(convertToScreenPos(m_shapeVec[i]._p1), umbra_r, penumbra_r, m_lightColor);
            //m_shadowDraw->drawGradualTriangle(convertToScreenPos(m_shapeVec[i]._p1), penumbra_r, umbra_r, shadow_color);
        }

        //右端不连续点
        if ((m_shapeVec[i]._p2 - m_shapeVec[(i + 1)%vecLen]._p1).getLengthSq() > 0.1f)
        {
            const Vec2 line = m_shapeVec[i]._p2 - m_shapeVec[i]._p1;
            const Vec2 light_l = m_lightPos + line.getNormalized()*m_lightRadius;
            const Vec2 light_r = m_lightPos - line.getNormalized()*m_lightRadius;
            const Vec2 ray_umbra_l = light_l - m_shapeVec[i]._p2;
            const Vec2 ray_penumbra_l = light_r - m_shapeVec[i]._p2;
            const Vec2 umbra_l = convertToScreenPos(m_lightPos) + (-ray_umbra_l).getNormalized()*MAX_NUMBER;
            const Vec2 penumbra_l = convertToScreenPos(m_lightPos) + (-ray_penumbra_l).getNormalized()*MAX_NUMBER;
            Color4F color = Color4F(m_shapeVec[i].getColor());
            Color4F shadow_color = Color4F(color.r*(1.0f - color.a), color.g*(1.0f - color.a), color.b*(1.0f - color.a), color.a);
            color.a = 1.0f - color.a;
            m_translucentDraw->drawGradualTriangle(convertToScreenPos(m_shapeVec[i]._p2), umbra_l, penumbra_l, m_lightColor);
            //m_shadowDraw->drawGradualTriangle(convertToScreenPos(m_shapeVec[i]._p2), penumbra_l, umbra_l, shadow_color);
        }
    }
}

void LightNode::handleTexture( float maxRadius )
{	
	//设置光照位置以及最大光照距离
	auto state = m_rt->getSprite()->getGLProgramState();
	Vec2 screenLightPos = convertToScreenPos(m_lightPos);
	state->setUniformVec2("u_lightPos",screenLightPos);
	state->setUniformFloat("u_maxLightRadius",(float)maxRadius);
    
    m_rt->beginWithClear(m_lightColor.r, m_lightColor.g, m_lightColor.b, m_lightColor.a);
    //m_rt->beginWithClear(0,0,0,0);
	//m_lightDraw->visit();

    //阴影与光照的混合选项，第二参数仍待斟酌，候选GL_SRC_ALPHA
    m_translucentDraw->setBlendFunc({ GL_ONE_MINUS_SRC_ALPHA ,GL_SRC_COLOR });
    m_translucentDraw->visit();

    //m_shadowDraw->visit();

	m_rt->end();

	//m_rt->saveToFile("Light Area.png",Image::Format::PNG);
	//Director::getInstance()->end();
}

void LightNode::setAlpha( unsigned int* pixel,const unsigned char alpha )
{
	unsigned int rgb = *pixel & 0xffffff;
	*pixel = (alpha << 24) + rgb;
}

unsigned char LightNode::getAlpha( const unsigned int* pixel ) const
{
	return (*pixel >> 24) & 0xff;
}

void LightNode::addGLShader( Node* node )
{
	GLProgram* gl = GLProgram::createWithFilenames(VSH_SHADER_PATH,FSH_SHADER_PATH);
	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION,GLProgram::VERTEX_ATTRIB_POSITION);
	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR,GLProgram::VERTEX_ATTRIB_COLOR);
	gl->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD,GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	gl->link();
	gl->updateUniforms();

	auto state = GLProgramState::getOrCreateWithGLProgram(gl);
	//设置像素的size
	Size size = static_cast<Sprite*>(node)->getTexture()->getContentSizeInPixels();
	state->setUniformVec2("u_resolution",Vec2(size.width,size.height));
	//加入节点
	node->setGLProgramState(state);
}

void LightNode::updateLight()
{
    TIME_ELAPSED_BEGIN("getNearbyLine");
	m_shapeVec = ShapeManage::getInstance()->getNearbyLine(this->getLightPos(),this->getLightMaxRadius());
    TIME_ELAPSED_END("getNearbyLine");

    TIME_ELAPSED_BEGIN("preRender");
    //createLight();
    TIME_ELAPSED_END("preRender");

    TIME_ELAPSED_BEGIN("renderLight");
	renderLight();
    TIME_ELAPSED_END("renderLight");
}

void LightNode::moveLight( const Vec2& delta )
{
	m_lightPos += delta;
}

cocos2d::Vec2 LightNode::convertToScreenPos( const Vec2& pos )
{
	return Vec2(pos + m_mapPos);
}

void LightNode::scaleLightRadiusTo( float nextRadius ,float duration )
{
	this->runAction(LightRadiusScaleTo::create(nextRadius,duration));
}

void LightNode::scaleLightRadiusBy( float deltaRadius ,float duration )
{
	this->runAction(LightRadiusScaleBy::create(deltaRadius,duration));
}

bool LightNode::isPointInLight( const Vec2& worldPos,const Vec2& lightPos )
{
    TIME_ELAPSED_BEGIN("isPointInLight");

	bool isInLight = false;
	Vec2 correctPos = worldPos;//m_rt->convertToNodeSpace(worldPos);
	do 
	{
		CC_BREAK_IF(correctPos.x < 0 || correctPos.y < 0);
		CC_BREAK_IF((correctPos-lightPos).getLength() > m_lightMaxRangeRadius);

		//以下代码改了RenderTexture的源码，为了拿到RenderTexture对应的缓冲区做颜色判断
		//要在RenderTexture类中添加public方法：
		//	inline GLuint getFBO()const {return _FBO;}
		GLubyte buffer[4] = {0,0,0,0};
		GLint oldFBO;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rt->getFBO());
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels((int)correctPos.x,(int)correctPos.y,1, 1,GL_RGBA,GL_UNSIGNED_BYTE, buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
		//alpha值大于某阈值则认为在光内
		if(buffer[3] > 20)
			isInLight = true;
	} while (0);

    TIME_ELAPSED_END("isPointInLight");

	return isInLight;
}

bool LightNode::isLightInScreen()
{
    Vec2 screen_pos = convertToScreenPos(m_lightPos);
    Size screen_size = Director::getInstance()->getVisibleSize();
    Rect light_box(screen_pos.x - m_lightMaxRangeRadius, screen_pos.y - m_lightMaxRangeRadius, m_lightMaxRangeRadius*2, m_lightMaxRangeRadius*2);
    Rect screen_box(0, 0, screen_size.width, screen_size.height);
    return screen_box.intersectsRect(light_box);
}
