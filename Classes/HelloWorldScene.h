#ifndef  __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <iostream>
//#include <time.h>
//#include <stdio.h>
#include "cocos2d.h"
//#include "cocos-ext.h" //1������������д��룬��Ȼ�޷�ʹ��Button
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "extensions/GUI/CCControlExtension/CCControlSlider.h"


//USING_NS_CC_EXT; //2������������д��룬��Ȼ�޷�ʹ��Button
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
#define GAMETIME 45 //��Ϸ��������ʱ��
#define MAXCLOTHES 14 //�·����������

static int level; //�������Ϸ��ˮƽ��Ҳ����ͼƬ�ָ��ά��
static bool abnormal; //���������Ϸ�Ƿ��̬ģʽ
static int clothesNum; //������·��ı��
static float zhe; //����Ǽ����Ż�
static bool jishi; //��������Ƿ����ü�ʱģʽ�����������ȡ�ģ�


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

	ControlSlider* _sliderValueVolume;//����һ�����������Ļ�����
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
	Sprite* png; //�깤֮�����ɾ�����
	int _gameState; //��Ϸ��״̬�������������Ƿ�ƴͼ��ɵȵ�
	int _picNum; //��ǰ��Ϸ�������ͼƬ���

	Sprite* _pngArr[36]; //���ָ�ͼƬ������
	int _arr[36]; //������ָ�ͼƬ�Ķ�ά����
	int _currPos; //��ǰ�հ׸������ڵ�λ��
	int _stepNum; //��������������˶��ٲ�
	int _randNum; //����ͼƬ���õĲ�������
	int _stepReport[140]; //���򣨴Ӵ�С����ƴͼ���ܲ��裬�����Ǵ���ͼƬ�Ĳ���
	float _bianChang; //ͼ��ı߳�
	//cocos2d::ui::Button* _dir[4];//�ĸ�����
	Sprite* _mixPng, *_maxPng, * _bgPng;
	float _touchX, _touchY;
	bool _isRunTime;//��־ʱ���Ƿ�������
	long  _initTime; //��ʱģʽ�£���Ϸ��ʼ����ʱ��
	long  _currentTime;//��Ϸ��ǰ��ʱ��
	int _shengyuTime; //ʣ���ʱ��
	Text* _shengyuText; //������ʾʱ����ı�����
	

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(XIXI);
	void update(float delta);
	void movePic(int num, int dir)//���������ƶ�ͼƬ���߼�ʵ��
	{
		_gameState = 6;//֮�������ó�6������Ϊmove������ûִ�����ʱ����Ϸ״̬һֱ��ĳ���ƶ�̬�����������Ŀ�Ҳ�����ƶ�

		MoveTo* move;
		switch (dir)
		{
		case 0://������
			move = MoveTo::create(TT, Vec2(_pngArr[num]->getPositionX() - _bianChang, _pngArr[num]->getPositionY()));
			break;
		case 2://������
			move = MoveTo::create(TT, Vec2(_pngArr[num]->getPositionX() + _bianChang, _pngArr[num]->getPositionY()));
			break;
		case 1://������
			move = MoveTo::create(TT, Vec2(_pngArr[num]->getPositionX(), _pngArr[num]->getPositionY() - _bianChang));
			break;
		case 3://������
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

		if (true == abnormal) //�����Ϸ�Ǳ�̬�Ѷȵ�ģʽ
		{
			RotateBy* rotate = RotateBy::create(TT, 90); //��ô˳ʱ����ת90��
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

class Attainment :public Scene //����һ���ɾ���
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