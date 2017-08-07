#include "DrawNodeSpec.h"

USING_NS_CC;

const char* DrawNodeSpec::_vertFileName = "shader/shader_drawnode_spec.vert";
const char* DrawNodeSpec::_fragFileName = "shader/shader_drawnode_spec.frag";
const char* DrawNodeSpec::_gradualFragFileName = "shader/shader_drawnode_spec_gradual.frag";

static const int DEFAULT_LINE_WIDTH = 2;

DrawNodeSpec::DrawNodeSpec() :
    gradual_gl_program_(nullptr),
    _vaoGradual(0),
    _vboGradual(0), 
    _bufferCapacityGradual(0), 
    _bufferCountGradual(0),
    _bufferGradual(nullptr),
    _dirtyGradual(false)
{
}

DrawNodeSpec::~DrawNodeSpec()
{
    CC_SAFE_RELEASE(gradual_gl_program_);

    free(_bufferGradual);
    _bufferGradual = nullptr;

    glDeleteBuffers(1, &_vboGradual);
    _vboGradual = 0;

    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
        glDeleteVertexArrays(1, &_vaoGradual);
        _vaoGradual = 0;
    }
}

bool DrawNodeSpec::init()
{
	_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR));
#else
    static GLProgram* gl = GLProgram::createWithFilenames(_vertFileName, _fragFileName);
    setGLProgramState(GLProgramState::getOrCreateWithGLProgram(gl));
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    gradual_gl_program_ = GLProgram::createWithFilenames(_vertFileName, _gradualFragFileName);
    gradual_gl_program_->retain();

	ensureCapacity(512);
	ensureCapacityGLPoint(64);
	ensureCapacityGLLine(256);
    ensureGradualCapacity(64);

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		glGenVertexArrays(1, &_vao);
		GL::bindVAO(_vao);
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
		// vertex
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
		// color
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
		// texcood
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));

		glGenVertexArrays(1, &_vaoGLLine);
		GL::bindVAO(_vaoGLLine);
		glGenBuffers(1, &_vboGLLine);
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
		// vertex
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
		// color
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
		// texcood
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));

		glGenVertexArrays(1, &_vaoGLPoint);
		GL::bindVAO(_vaoGLPoint);
		glGenBuffers(1, &_vboGLPoint);
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
		// vertex
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
		// color
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
		// Texture coord as pointsize
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));

        glGenVertexArrays(1, &_vaoGradual);
        GL::bindVAO(_vaoGradual);
        glGenBuffers(1, &_vboGradual);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGradual);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGradual, _bufferGradual, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // Texture coord as pointsize
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));

		GL::bindVAO(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	else
	{
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);

		glGenBuffers(1, &_vboGLLine);
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);

		glGenBuffers(1, &_vboGLPoint);
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);

        glGenBuffers(1, &_vboGradual);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGradual);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGradual, _bufferGradual, GL_STREAM_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	CHECK_GL_ERROR_DEBUG();

	_dirty = true;
	_dirtyGLLine = true;
	_dirtyGLPoint = true;
    _dirtyGradual = true;

#if CC_ENABLE_CACHE_TEXTURE_DATA
	// Need to listen the event only when not use batchnode, because it will use VBO
	auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event) {
		/** listen the event that renderer was recreated on Android/WP8 */
		this->init();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	return true;
}

void DrawNodeSpec::ensureGradualCapacity(int count)
{
    CCASSERT(count >= 0, "capacity must be >= 0");

    if (_bufferCountGradual + count > _bufferCapacityGradual)
    {
        _bufferCapacityGradual += MAX(_bufferCapacityGradual, count);
        _bufferGradual = (V2F_C4B_T2F*)realloc(_bufferGradual, _bufferCapacityGradual*sizeof(V2F_C4B_T2F));
    }
}

DrawNodeSpec * DrawNodeSpec::create()
{
	DrawNodeSpec* ret = new (std::nothrow) DrawNodeSpec();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

void DrawNodeSpec::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
{
    DrawNode::draw(renderer, transform, flags);

    if (_bufferCountGradual)
    {
        _customCommandGradual.init(_globalZOrder, transform, flags);
        _customCommandGradual.func = CC_CALLBACK_0(DrawNodeSpec::onDrawGradual, this, transform, flags);
        renderer->addCommand(&_customCommandGradual);
    }
}

void DrawNodeSpec::onDrawGradual(const Mat4 & transform, uint32_t flags)
{
    auto glProgram = gradual_gl_program_;
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);

    GL::blendFunc(_blendFunc.src, _blendFunc.dst);

    if (_dirtyGradual)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGradual);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGradual, _bufferGradual, GL_STREAM_DRAW);

        _dirtyGradual = false;
    }
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vaoGradual);
    }
    else
    {
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

        glBindBuffer(GL_ARRAY_BUFFER, _vboGradual);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcood
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }

    glDrawArrays(GL_TRIANGLES, 0, _bufferCountGradual);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }

    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCountGradual);
    CHECK_GL_ERROR_DEBUG();
}

void DrawNodeSpec::drawGradualTriangle(const Vec2 & phead, const Vec2 & pwhite, const Vec2 & pblack, const Color4F & color)
{
    unsigned int vertex_count = 3;
    ensureGradualCapacity(vertex_count);

    Color4B col = Color4B(color);
    V2F_C4B_T2F a = { Vec2(phead.x, phead.y), col, Tex2F(0.0, 1.0) };
    V2F_C4B_T2F b = { Vec2(pwhite.x, pwhite.y), col, Tex2F(0.0,  0.0) };
    V2F_C4B_T2F c = { Vec2(pblack.x, pblack.y), col, Tex2F(1.0,  1.0) };

    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_bufferGradual + _bufferCountGradual);
    V2F_C4B_T2F_Triangle triangle = { a, b, c };
    triangles[0] = triangle;

    _bufferCountGradual += vertex_count;
    _dirtyGradual = true;
}

void DrawNodeSpec::drawGradualRectangle(const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2, const cocos2d::Vec2 &p3, const cocos2d::Vec2 &p4, const cocos2d::Color4F &color)
{
    unsigned int vertex_count = 6;
    ensureGradualCapacity(vertex_count);

    Color4B col = Color4B(color);
    V2F_C4B_T2F a = { Vec2(p1.x, p1.y), col, Tex2F(0.0, 0.0) };
    V2F_C4B_T2F b = { Vec2(p2.x, p2.y), col, Tex2F(0.0,  1.0) };
    V2F_C4B_T2F c = { Vec2(p3.x, p3.y), col, Tex2F(1.0,  1.0) };
    V2F_C4B_T2F d = { Vec2(p4.x, p4.y), col, Tex2F(1.0,  0.0) };

    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_bufferGradual + _bufferCountGradual);
    V2F_C4B_T2F_Triangle triangle1 = { a, b, c };
    V2F_C4B_T2F_Triangle triangle2 = { a, c, d };
    triangles[0] = triangle1;
    triangles[1] = triangle2;

    _bufferCountGradual += vertex_count;
    _dirtyGradual = true;
}

void DrawNodeSpec::clear()
{
    DrawNode::clear();
    
    _bufferCountGradual = 0;
    _dirtyGradual = true;
}

