/********************************************************************
	date  :	2016/02/23   12:25

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef DRAWNODESPEC_H_
#define DRAWNODESPEC_H_

#include "cocos2d.h"

class DrawNodeSpec : public cocos2d::DrawNode
{
public:
	static const char* _vertFileName;
	static const char* _fragFileName;
    static const char* _gradualFragFileName;

	static DrawNodeSpec* create();

    void drawGradualTriangle(const cocos2d::Vec2 &phead, const cocos2d::Vec2 &pwhite, const cocos2d::Vec2 &pblack, const cocos2d::Color4F &color);

    void drawGradualRectangle(const cocos2d::Vec2 &p1, const cocos2d::Vec2 &p2, const cocos2d::Vec2 &p3, const cocos2d::Vec2 &p4, const cocos2d::Color4F &color);

    void clear();

    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;

    void onDrawGradual(const cocos2d::Mat4 &transform, uint32_t flags);
protected:
	DrawNodeSpec();
	virtual ~DrawNodeSpec();

	virtual bool init() override;

    void ensureGradualCapacity(int count);

	cocos2d::GLProgram* gradual_gl_program_;
	
    GLuint      _vaoGradual;
    GLuint      _vboGradual;

    int         _bufferCapacityGradual;
    GLsizei     _bufferCountGradual;
    cocos2d::V2F_C4B_T2F *_bufferGradual;

    cocos2d::CustomCommand _customCommandGradual;

    bool        _dirtyGradual;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(DrawNodeSpec);
};
#endif // DRAWNODESPEC_H_