#include "FullMapEffect.h"
#include "Game/GameManage.h"
//#include <windows.h>

USING_NS_CC;

FullMapEffect* FullMapEffect::create( int w, int h )
{
	FullMapEffect *ret = new FullMapEffect();
	if (ret && ret->initWithWidthAndHeight(w,h,Texture2D::PixelFormat::RGBA8888))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return ret;
}
// double GetTime()
// {
// 	LARGE_INTEGER f,time;
// 	QueryPerformanceFrequency(&f);             //获取硬件支持的高精度计数器的频率，若不支持则返回0
// 	QueryPerformanceCounter(&time);            //获取当前计数器的计数
// 	return time.QuadPart/(double)f.QuadPart;   //除以频率，就是当前时间，以秒作单位
// }

bool FullMapEffect::initWithWidthAndHeight( int w, int h, Texture2D::PixelFormat format )
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!RenderTexture::initWithWidthAndHeight(w,h,format));
		this->getSprite()->setBlendFunc(BlendFunc::ADDITIVE);
		this->beginWithClear(0,0,0,0);

		//auto nowTime = GetTime();

		auto p = GameManage::getInstance()->getMap()->getStaticDrawNode();
		p->visit();
		this->end();

		//log("visit time:%f",GetTime()-nowTime);nowTime = GetTime();

		Director::getInstance()->getRenderer()->render();

		//log("render time:%f",GetTime()-nowTime);nowTime = GetTime();

		//this->saveToFile("mappath.png");
		auto& reachVec = GameManage::getInstance()->getPotalPositionVec();
        reachVec.push_back(GameManage::getInstance()->getBeaconPos());

		int BufferWidth = (int)this->getSprite()->getTexture()->getContentSizeInPixels().width;
		int BufferHeight = (int)this->getSprite()->getTexture()->getContentSizeInPixels().height;

		GLubyte* buffer = new GLubyte[BufferWidth * BufferHeight * 4];

		GLint oldFBO;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, this->getFBO());
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0,0,BufferWidth, BufferHeight,GL_RGBA,GL_UNSIGNED_BYTE, buffer);
		
		//log("read pixel time:%f",GetTime()-nowTime);nowTime = GetTime();

		Color4B fillColor(Color4B::YELLOW);
		fillColor.a = 100;
		Rect rc = Rect(0.0f, 0.0f, (float)BufferWidth - 1, (float)BufferHeight - 1);
		std::queue<Vec2> q;
		bool* visit = new bool[BufferWidth * BufferHeight]();
		Vec2 temp = Vec2(0,0);
		for (auto it:reachVec)
		{
			if (buffer[(int)it.y*BufferWidth*4 + (int)it.x*4+3] == 0)
			{
				q.push(it);
				visit[(int)it.y*BufferWidth + (int)it.x] = true;
				while (!q.empty())
				{
					temp = q.front();
					q.pop();

// 					if (rc.containsPoint(temp + Vec2(0,1)) && visit[(int)(temp.y+1)*BufferWidth + (int)temp.x] == false &&
// 						buffer[(int)(temp.y+1)*BufferWidth*4 + (int)temp.x*4+3] == 0)
// 					{
// 						visit[(int)(temp.y+1)*BufferWidth + (int)temp.x] = true;
// 						q.push(temp + Vec2(0,1));
// 					}
// 					if (rc.containsPoint(temp + Vec2(0,-1)) && visit[(int)(temp.y-1)*BufferWidth + (int)temp.x] == false &&
// 						buffer[(int)(temp.y-1)*BufferWidth*4 + (int)temp.x*4+3] == 0)
// 					{
// 						q.push(temp + Vec2(0,-1));
// 						visit[(int)(temp.y-1)*BufferWidth + (int)temp.x] = true;
// 					}
// 					if (rc.containsPoint(temp + Vec2(1,0)) && visit[(int)(temp.y)*BufferWidth + (int)temp.x+1] == false &&
// 						buffer[(int)(temp.y)*BufferWidth*4 + (int)(temp.x+1)*4+3] == 0)
// 					{
// 						q.push(temp + Vec2(1,0));
// 						visit[(int)(temp.y)*BufferWidth + (int)temp.x+1] = true;
// 					}
// 					if (rc.containsPoint(temp + Vec2(-1,0)) && visit[(int)(temp.y)*BufferWidth + (int)temp.x-1] == false &&
// 						buffer[(int)(temp.y)*BufferWidth*4 + (int)(temp.x-1)*4+3] == 0)
// 					{
// 						q.push(temp + Vec2(-1,0));
// 						visit[(int)(temp.y)*BufferWidth + (int)temp.x-1] = true;
// 					}
// 					buffer[(int)temp.y*BufferWidth*4 + (int)temp.x*4] = fillColor.r;
// 					buffer[(int)temp.y*BufferWidth*4 + (int)temp.x*4+1] = fillColor.g;
// 					buffer[(int)temp.y*BufferWidth*4 + (int)temp.x*4+2] = fillColor.b;
// 					buffer[(int)temp.y*BufferWidth*4 + (int)temp.x*4+3] = fillColor.a;
					if (rc.containsPoint(temp + Vec2(0,1)) && *(visit+(int)(temp.y+1)*BufferWidth + (int)temp.x) == false &&
						*(buffer+(int)(temp.y+1)*BufferWidth*4 + (int)temp.x*4+3) == 0)
					{
						*(visit+(int)(temp.y+1)*BufferWidth + (int)temp.x) = true;
						q.push(temp + Vec2(0,1));
					}
					if (rc.containsPoint(temp + Vec2(0,-1)) && *(visit+(int)(temp.y-1)*BufferWidth + (int)temp.x) == false &&
						*(buffer+(int)(temp.y-1)*BufferWidth*4 + (int)temp.x*4+3) == 0)
					{
						q.push(temp + Vec2(0,-1));
						*(visit+(int)(temp.y-1)*BufferWidth + (int)temp.x) = true;
					}
					if (rc.containsPoint(temp + Vec2(1,0)) && *(visit+(int)(temp.y)*BufferWidth + (int)temp.x+1) == false &&
						*(buffer+(int)(temp.y)*BufferWidth*4 + (int)(temp.x+1)*4+3) == 0)
					{
						q.push(temp + Vec2(1,0));
						*(visit+(int)(temp.y)*BufferWidth + (int)temp.x+1) = true;
					}
					if (rc.containsPoint(temp + Vec2(-1,0)) && *(visit+(int)(temp.y)*BufferWidth + (int)temp.x-1) == false &&
						*(buffer+(int)(temp.y)*BufferWidth*4 + (int)(temp.x-1)*4+3) == 0)
					{
						q.push(temp + Vec2(-1,0));
						*(visit+(int)(temp.y)*BufferWidth + (int)temp.x-1) = true;
					}
					*(buffer+(int)temp.y*BufferWidth*4 + (int)temp.x*4) = fillColor.r;
					*(buffer+(int)temp.y*BufferWidth*4 + (int)temp.x*4+1) = fillColor.g;
					*(buffer+(int)temp.y*BufferWidth*4 + (int)temp.x*4+2) = fillColor.b;
					*(buffer+(int)temp.y*BufferWidth*4 + (int)temp.x*4+3) = fillColor.a;
				}
			}
		}

		//log("bfs time:%f",GetTime()-nowTime);nowTime = GetTime();

        Image* img = new Image();
        img->initWithRawData(buffer, BufferWidth*BufferHeight * 4, BufferWidth, BufferHeight, 4, true);
        img->saveToFile(FileUtils::getInstance()->getWritablePath() + "mappath.png", false);
        delete img;

		this->getSprite()->getTexture()->initWithData(
			buffer,
			BufferWidth*BufferHeight*4,
			Texture2D::PixelFormat::RGBA8888,
			BufferWidth,
			BufferHeight,
			Size((float)BufferWidth, (float)BufferHeight));
		//glDrawPixels(BufferWidth,BufferHeight,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
		delete[] buffer;

		//log("write data time:%f",GetTime()-nowTime);nowTime = GetTime();

		//this->saveToFile("mappath.png");
		
		//log("save file time:%f",GetTime()-nowTime);nowTime = GetTime();

		bRet = true;
	} while (0);
	return bRet;
}

void FullMapEffect::drawPixel( GLubyte* buffer,int x,int y,int bufferWidth,const Color4B& fillcolor )
{
	buffer[y*bufferWidth*4 + x*4] = fillcolor.r;
	buffer[y*bufferWidth*4 + x*4+1] = fillcolor.g;
	buffer[y*bufferWidth*4 + x*4+2] = fillcolor.b;
	buffer[y*bufferWidth*4 + x*4+3] = fillcolor.a;
}
