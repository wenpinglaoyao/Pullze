#ifndef  __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <iostream>
//#include <time.h>
//#include <stdio.h>
#include "cocos2d.h"
//#include "cocos-ext.h" //1必须加上这两行代码，不然无法使用Button
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "extensions/GUI/CCControlExtension/CCControlSlider.h"


//USING_NS_CC_EXT; //2必须加上这两行代码，不然无法使用Button
USING_NS_CC;
using namespace std;
using namespace ui;
using namespace CocosDenshion;
using namespace cocos2d::extension;

#define WIDTH 1320
#define HEIGHT 780
#define XX 500
#define YY 300
#define TT 0.15f
#define GAMETIME 45 //游戏的最快过关时间
#define MAXCLOTHES 14 //衣服的最多数量

static int level; //这个是游戏的水平，也就是图片分割的维数
static bool abnormal; //这个决定游戏是否变态模式
static int clothesNum; //这个是衣服的编号
static float zhe; //这个是几折优惠
static bool jishi; //这个代表是否启用计时模式（可以免费领取的）


class HelloWorld : public cocos2d::Scene
{
public:

    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuFontCallBack(cocos2d::Ref* pSender,int,bool ab=false,bool ji=false);
	void menuAboutCallBack(cocos2d::Ref*);
	void menuAttainmentCallBack(cocos2d::Ref*);
	void menuHelpCallBack(cocos2d::Ref*);
	void sliderValueChanged(Ref* pSender,Control::EventType controlType)
	{
		const float volume = _sliderValueVolume->getValue();
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
		SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
	}
	void runFlush();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void update(float delta);

	static cocos2d::Scene* createLevelGame();

	ControlSlider* _sliderValueVolume;//增加一个控制音量的滑动条
	Sprite* _sprite;
	Sprite* _authorSprite;
	bool _authorFlag;
};

class About : public cocos2d::Scene
{
public:
	CREATE_FUNC(About);

	static cocos2d::Scene * createScene()
	{
		return About::create();
	}

	virtual bool init();

	void popSceneCallBack(Ref* pSender) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
		Director::getInstance()->popScene();
	}
};

class XIXI :public cocos2d::Scene {
private:
	string _PNGName;
	Sprite* png; //完工之后可以删除这个
	int _gameState; //游戏的状态，如上下左右是否拼图完成等等
	int _picNum; //当前游戏随机出的图片编号

	Sprite* _pngArr[36]; //被分割图片的数组
	int _arr[36]; //代表各分割图片的二维数组
	int _currPos; //当前空白格子所在的位置
	int _stepNum; //用来计算玩家用了多少步
	int _randNum; //打乱图片所用的步骤数量
	int _stepReport[140]; //倒序（从大到小）是拼图解密步骤，正序是打乱图片的步骤
	float _bianChang; //图块的边长
	//cocos2d::ui::Button* _dir[4];//四个方向
	Sprite* _mixPng, *_maxPng, * _bgPng;
	float _touchX, _touchY;
	bool _isRunTime;//标志时间是否在流动
	long  _initTime; //计时模式下，游戏初始化的时间
	long  _currentTime;//游戏当前的时间
	int _shengyuTime; //剩余的时间
	Text* _shengyuText; //用来显示时间的文本精灵
	

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(XIXI);
	void update(float delta);
	void movePic(int num, int dir)//上下左右移动图片的逻辑实现
	{
		_gameState = 6;//之所以设置成6，是因为move动作还没执行完毕时，游戏状态一直是某个移动态，会让其它的块也跟着移动

		MoveTo* move;
		switch (dir)
		{
		case 0://向上走
			move = MoveTo::create(TT, Vec2(_pngArr[num]->getPositionX() - _bianChang, _pngArr[num]->getPositionY()));
			break;
		case 2://向下走
			move = MoveTo::create(TT, Vec2(_pngArr[num]->getPositionX() + _bianChang, _pngArr[num]->getPositionY()));
			break;
		case 1://向左走
			move = MoveTo::create(TT, Vec2(_pngArr[num]->getPositionX(), _pngArr[num]->getPositionY() - _bianChang));
			break;
		case 3://向右走
			move = MoveTo::create(TT, Vec2(_pngArr[num]->getPositionX(), _pngArr[num]->getPositionY() + _bianChang));
			break;
		}
		
		Sequence* sequence = Sequence::create(move,
			CallFunc::create([&]() {
			_gameState = 5;
			string str = "effect";
			str.append(StringUtils::format("%d.mp3", level));
			if (abnormal == true) str = "effect6.mp3";
			SimpleAudioEngine::getInstance()->playEffect(str.c_str());
		}), 
			NULL);
		_pngArr[num]->runAction(sequence);

		if (true == abnormal) //如果游戏是变态难度的模式
		{
			RotateBy* rotate = RotateBy::create(TT, 90); //那么顺时针旋转90度
			_pngArr[num]->runAction(rotate);
		}
	}
	void popSceneCallBack(Ref* pSender)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Director::getInstance()->popScene();
	}
	long retTimeNum(struct tm * time)
	{
		return  time->tm_yday*86400 + time->tm_hour * 3600 + time->tm_min * 60 + time->tm_sec;
	}
};

class ClothesMenu :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene() { return ClothesMenu::create(); }
	virtual bool init();
	CREATE_FUNC(ClothesMenu);
	void gameCallBack(Ref* pSender, int num)
	{
		SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
		clothesNum = num;
		Director::getInstance()->pushScene(XIXI::createScene());
	}
	void backCallBack(Ref* pSender) {
		Director::getInstance()->popScene();
	}
};

class Attainment :public Scene //增加一个成就类
{
public:
	static cocos2d::Scene* createScene() { return Attainment::create(); }
	virtual bool init();
	CREATE_FUNC(Attainment);
};

class Help :public Scene
{
public:
	static cocos2d::Scene* createScene() { return Help::create(); };
	virtual bool init();
	CREATE_FUNC(Help);
};
#endif // __HELLOWORLD_SCENE_H__