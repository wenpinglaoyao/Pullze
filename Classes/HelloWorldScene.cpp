#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}


// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	runFlush();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 3. add your codes below...
	
	auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");

	const char * str0 = ((CCString *)chnStrings->objectForKey("about"))->getCString();
	const char * str1 = ((CCString *)chnStrings->objectForKey("easy"))->getCString();
	const char * str2 = ((CCString *)chnStrings->objectForKey("normal"))->getCString();
	const char * str3 = ((CCString *)chnStrings->objectForKey("hard"))->getCString();
	const char * str4 = ((CCString *)chnStrings->objectForKey("veryhard"))->getCString();
	const char * str5 = ((CCString *)chnStrings->objectForKey("abnormal"))->getCString();
	const char * str7 = ((CCString *)chnStrings->objectForKey("jishi"))->getCString();
	const char * attainmentStr = ((CCString*)chnStrings->objectForKey("attainment"))->getCString();
	const char * helpStr = ((CCString*)chnStrings->objectForKey("help"))->getCString();

	const char * str6 = ((CCString *)chnStrings->objectForKey("quit"))->getCString();

	MenuItemFont::setFontSize(70);

	auto quit = MenuItemFont::create(str6, CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	quit->setPosition(Vec2(1280,620));
	quit->setRotation(270);

	auto about = MenuItemFont::create(str0,CC_CALLBACK_1(HelloWorld::menuAboutCallBack,this));
	about->setRotation(270);
	about->setPosition(1280, 160);

	auto attainmentItem = MenuItemFont::create(attainmentStr, CC_CALLBACK_1(HelloWorld::menuAttainmentCallBack,this));
	attainmentItem->setRotation(270);
	attainmentItem->setPosition(40, 160);

	auto helpItem = MenuItemFont::create(helpStr, CC_CALLBACK_1(HelloWorld::menuHelpCallBack, this));
	helpItem->setRotation(270);
	helpItem->setPosition(40, 620);
	
	
	auto easy = MenuItemFont::create(str1, CC_CALLBACK_1(HelloWorld::menuFontCallBack, this, 2, false, false));
	easy->setRotation(270);
	easy->setPosition(100+50, 390);

	auto normal = MenuItemFont::create(str2, CC_CALLBACK_1(HelloWorld::menuFontCallBack,this, 3, false,false));
	normal->setRotation(270);
	normal->setPosition(300+50, 390);

	auto hard = MenuItemFont::create(str3, CC_CALLBACK_1(HelloWorld::menuFontCallBack, this, 4, false,false));
	hard->setRotation(270);
	hard->setPosition(500+50, 390);

	auto very = MenuItemFont::create(str4, CC_CALLBACK_1(HelloWorld::menuFontCallBack, this, 5, false,false));
	very->setRotation(270);
	very->setPosition(700+50, 390);

	auto ab = MenuItemFont::create(str5, CC_CALLBACK_1(HelloWorld::menuFontCallBack, this, 4, true,false));
	ab->setRotation(270);
	ab->setPosition(900+50, 390);

	auto jishiItem = MenuItemFont::create(str7, CC_CALLBACK_1(HelloWorld::menuFontCallBack, this, 4, true, true));
	jishiItem->setRotation(270);
	jishiItem->setPosition(1100+50, 390);

	auto menu = Menu::create(easy,normal,hard,very,ab, jishiItem,attainmentItem,helpItem, about,quit,NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, 1);

	/******�����������߼���ʼ*******/
	{
		_sliderValueVolume = ControlSlider::create("sliderTrack.png", "sliderProgress.png", "sliderThumb.png");
		_sliderValueVolume->setMinimumValue(0.0);
		_sliderValueVolume->setMaximumValue(1.0);
		_sliderValueVolume->setValue(1.0);
		_sliderValueVolume->setRotation(180);
		_sliderValueVolume->setPosition(1200, 390);
		_sliderValueVolume->setAnchorPoint(Vec2(0, 0.5));
		_sliderValueVolume->setScale(2.0);
		_sliderValueVolume->addTargetWithActionForControlEvents(
			this, cccontrol_selector(HelloWorld::sliderValueChanged),
			Control::EventType::VALUE_CHANGED);
		addChild(_sliderValueVolume, 100);
		_sliderValueVolume->setVisible(false);

		Button* volumeBtn = Button::create("volume.png");
		volumeBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				_sliderValueVolume->setVisible(! _sliderValueVolume->isVisible());
				break;
			}
		});
		volumeBtn->setPosition(Vec2(1280, 390));
		addChild(volumeBtn);
	}
	/******�����������߼�����*******/

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void HelloWorld::menuFontCallBack(cocos2d::Ref* pSender,int i,bool ab,bool ji)//��ť���º󿪱ٶ�Ӧ�Ĺؿ�
{
	level = i; //���������Ϸ�Ǽ�ά��
	abnormal = ab; //���������Ϸ�Ƿ�����תģʽ
	jishi = ji;  //���������Ϸ�Ƿ���ü�ʱģʽ
	if (abnormal && jishi) //�����Ϸ����תģʽ���������˼�ʱģʽ
		zhe = 10; //��һ���Żݶ���������������Ĺ����ˣ����ں���Ĵ��������¸�zhe��ֵ)

	else if (abnormal) //��Ȼ��Ϸ����תģʽ����û�����ü�ʱ
		zhe = 5; //�Ǵ����ۺ���

	else  //�ٲ�Ȼ��Ϸ�϶��Ǿ���ģʽ��
		zhe = 11 - level;  //����Ӧ��ά����ɣ��ֱ���9��8��7��6��

	SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
	Director::getInstance()->pushScene(ClothesMenu::createScene());
}
void HelloWorld::menuAboutCallBack(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
	Director::getInstance()->pushScene(About::createScene());
}
void HelloWorld::menuAttainmentCallBack(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
	Director::getInstance()->pushScene(Attainment::createScene());
}
void HelloWorld::menuHelpCallBack(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
	Director::getInstance()->pushScene(Help::createScene());
}
cocos2d::Scene* HelloWorld::createLevelGame()
{
	return nullptr;
}

void HelloWorld::update(float delta)
{
	
}






//���濪ʼ��XIXI��Ķ���
Scene* XIXI::createScene() {
	return XIXI::create();
}

bool XIXI::init()
{
	if (!Scene::init()) return false;

	auto backItem = Button::create("back.png");
	backItem->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		switch (type)
		{
		case Widget::TouchEventType::ENDED:
			SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->popScene();
		}
		return true;
	});
	backItem->setPosition(Vec2(1280, 740));
	addChild(backItem, 99);
	
	/*string musicstr = "";
	musicstr.append(StringUtils::format("%d.mp3", level));
	if (abnormal) musicstr = "6.mp3";*/
	SimpleAudioEngine::getInstance()->playBackgroundMusic(/*musicstr.c_str()*/"4.mp3", true);

	_currPos = level*level;
	_gameState = 6; //�Ȱ���Ϸ״̬����Ϊ�����ƶ���Ȼ����init������󲿷ְ���ϷŪ�ɿɱ��ƶ���
	_stepNum = _randNum = 0;
	_bianChang = int(780 / level);
	_touchX = _touchY = -100;
	_isRunTime = false;
	_shengyuTime = GAMETIME;
	
	/****��������***/
	{
		int mixX = WIDTH - (level+1)*_bianChang;
		_mixPng = Sprite::create("bg.png");
		_mixPng->setContentSize(Size(mixX, _bianChang));
		_mixPng->setPosition(WIDTH - mixX / 2, _bianChang / 2);
		addChild(_mixPng);
		int maxX = WIDTH - HEIGHT;
		int maxY = HEIGHT - _bianChang;
		_maxPng = Sprite::create("bg.png");
		_maxPng->setContentSize(Size(maxX, maxY));
		_maxPng->setPosition(HEIGHT + maxX / 2, maxY / 2 + _bianChang);
		addChild(_maxPng);
	}
	/*****���������ӽ���***/

	/******��Ӵ�������*******/
	{
		EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
		touchListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			Vec2 location = touch->getLocation();
			_touchX = location.x;
			_touchY = location.y;
			return true;
		};
		touchListener->onTouchEnded = [&](Touch* touch, Event* event)
		{
			Vec2 location = touch->getLocation();

			if (location.x < _touchX && (_touchX - location.x)*(_touchX - location.x) >(_touchY - location.y)*(_touchY - location.y))
			{
				if (5 == _gameState)
					_gameState = 0;
			}

			if (location.x > _touchX && (_touchX - location.x)*(_touchX - location.x) >(_touchY - location.y)*(_touchY - location.y))
			{
				if (5 == _gameState)
					_gameState = 2;
			}

			if (location.y < _touchY && (_touchX - location.x)*(_touchX - location.x) <(_touchY - location.y)*(_touchY - location.y))
			{
				if (5 == _gameState)
					_gameState = 1;
			}

			if (location.y > _touchY && (_touchX - location.x)*(_touchX - location.x) <(_touchY - location.y)*(_touchY - location.y))
			{
				if (5 == _gameState)
					_gameState = 3;
			}
			_touchX = _touchY = -100;
		};

		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	}
	/*******��Ӵ�����������*******/

	///*******����һ�����ϵİ�ť*******/
	//{
	//	_dir[0] = Button::create("left.png");
	//	_dir[0]->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
	//		switch (type)
	//		{
	//		case Widget::TouchEventType::BEGAN:
	//			if(5 == _gameState)
	//				_gameState = 0;
	//			break;
	//		}
	//	});
	//	_dir[0]->setPosition(Vec2(1050 - 1500, 500));
	//	DelayTime* delayTime = DelayTime::create(TT * 6);
	//	auto move = MoveTo::create(TT * 10,Vec2(1050 - 135, 500));
	//	Sequence* sequence = Sequence::create(delayTime, move, NULL);
	//	_dir[0]->runAction(sequence);

	//	_dir[0]->setRotation(45);
	//	addChild(_dir[0],99);
	//}
	///*******����һ�����ϵİ�ť**********/

	///********����һ����XIA�İ�ť************************/
	//{
	//	_dir[2] = Button::create("left.png");
	//	_dir[2]->setRotation(180);
	//	_dir[2]->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
	//		switch (type)
	//		{
	//		case Widget::TouchEventType::ENDED:
	//			if (5 == _gameState)
	//				_gameState = 2;
	//			break;
	//		}
	//	});
	//	_dir[2]->setPosition(Vec2(1050 + 1500, 500));
	//	DelayTime* delayTime = DelayTime::create(TT * 6);
	//	auto move = MoveTo::create(TT * 10, Vec2(1050 + 135, 500));
	//	Sequence* sequence = Sequence::create(delayTime, move,
	//		CallFunc::create([&]() {
	//		SimpleAudioEngine::getInstance()->playEffect("daojishi.mp3");
	//	}),
	//		NULL);
	//	_dir[2]->runAction(sequence);
	//	_dir[2]->setRotation(180+45);
	//	addChild(_dir[2],99);
	//}
	///********����һ����XIA�İ�ť**********************/

	///********����һ����ZUO�İ�ť*********/
	//{
	//	_dir[1] = Button::create("left.png");
	//	_dir[1]->setRotation(270);
	//	_dir[1]->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
	//		switch (type)
	//		{
	//		case Widget::TouchEventType::ENDED:
	//			if (5 == _gameState)
	//				_gameState = 1;
	//			break;
	//		}
	//	});
	//	_dir[1]->setPosition(Vec2(1050, 500 - 1500));
	//	DelayTime* delayTime = DelayTime::create(TT * 6);
	//	auto move = MoveTo::create(TT * 10, Vec2(1050, 500 - 135));
	//	Sequence* sequence = Sequence::create(delayTime, move, NULL);
	//	_dir[1]->runAction(sequence);
	//	_dir[1]->setRotation(270 + 45);
	//	addChild(_dir[1],99);
	//}
	///********����һ����ZUO�İ�ť*********/

	///********����һ����YOU�İ�ť*********/
	//{
	//	_dir[3] = Button::create("left.png");
	//	_dir[3]->setRotation(90);
	//	_dir[3]->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
	//		switch (type)
	//		{
	//		case Widget::TouchEventType::ENDED:
	//			if (5 == _gameState)
	//				_gameState = 3;
	//			break;
	//		}
	//	});
	//	_dir[3]->setPosition(Vec2(1050, 500 + 1500));
	//	DelayTime* delayTime = DelayTime::create(TT * 6);
	//	auto move = MoveTo::create(TT * 10, Vec2(1050, 500 + 135));
	//	Sequence* sequence = Sequence::create(delayTime, move, NULL);
	//	_dir[3]->runAction(sequence);
	//	_dir[3]->setRotation(90 + 45);
	//	addChild(_dir[3],99);
	//}
	///********����һ����YOU�İ�ť*********/

	//����ÿһ��СͼƬ��������ȷ��λ��
	for (int i = 0;i <= _currPos;i++) {
		_arr[i] = i; 
	}

	/***********ѡ��һ��ͼƬ�����и������ͼ��********************/
	{
		_PNGName = "";
		_PNGName.append(StringUtils::format("xixi%d.jpg", clothesNum));

		/*****��ע�⣬�����bgpng�ǲο��õĴ�ͼƬ****/
		{
			_bgPng = Sprite::create(_PNGName.c_str());
			_bgPng->setPosition(1100, 600);
			_bgPng->setScale(0.33);
			addChild(_bgPng,66);
		}
		/****�ο�ͼ�߼�����***/

		for (int i = 0;i < level*level;i++)
		{
			_pngArr[i] = Sprite::create(_PNGName.c_str(), Rect((int(i / level))*_bianChang,
				HEIGHT - _bianChang - (int(i%level))*_bianChang, _bianChang, _bianChang));

			srand((unsigned)time(0));
			int randX = random(-XX, XX+WIDTH);
			int randY = random(-YY, YY + HEIGHT);
			_pngArr[i]->setPosition(randX, randY); //��������Сͼ��ѡ��һ����ʼλ��
		}
	}
	/***********�и�ͼ�����***********/
	
	/************��ʼ����ͼƬ***********/
	{
		_arr[_currPos] = _currPos - level; //�Ȱѽǿ���õ�������
		_currPos -= level;  //�ѿհ׸�����һλ
		_arr[_currPos] = level*level; //�������ε����½���Ϊ�հ׸�
		if (abnormal == true) //�����Ϸ�Ǳ�̬ģʽ����ô�ѽ������Ǹ�ͼ����ʱ����ת90��
			_pngArr[_currPos]->setRotation(270);
		for (int i = 0;i < 137;i++)
		{
			srand((unsigned int)time(0));
			int randDir = random(0, 3);
		startmove:
			switch (randDir % 4)
			{
			case 0: //������
				if (_currPos / level < level - 1)
				{
					_arr[_currPos] = _arr[_currPos + level];//�������Ǹ����ƶ���Сͼ�������հ׸�λ��
					_currPos += level; //�հ׸��û��������Ǹ�Сͼ������

					if(true == abnormal) //�����Ϸ�Ǳ�̬ģʽ����ô�������ͼ��ʱ��ʱ����ת90��
						_pngArr[_arr[_currPos]]->setRotation(_pngArr[_arr[_currPos]]->getRotation() + 270);
					
					int jiao = _pngArr[_arr[_currPos]]->getRotation();

					_arr[_currPos] = level*level;
					_stepReport[_randNum] = 0; //��¼����һ���ƶ��Ĳ���
					_randNum++; //�ƶ�ͼ��Ĵ���+1
				}
				else //��Ȼ���������ƶ�ͼ�鲻����
				{
					randDir++;
					goto startmove;
				}
				break;

			case 2: //�����ƶ�ͼ��
				if (_currPos / level > 0)
				{
					_arr[_currPos] = _arr[_currPos - level];
					_currPos -= level;
				
					if (true == abnormal) //�����Ϸ�Ǳ�̬ģʽ����ô�������ͼ��ʱ��ʱ����ת90��
						_pngArr[_arr[_currPos]]->setRotation(_pngArr[_arr[_currPos]]->getRotation() + 270);

					int jiao = _pngArr[_arr[_currPos]]->getRotation();

					_arr[_currPos] = level*level;
					_stepReport[_randNum] = 2;
					_randNum++;
				}
				else //��Ȼ���������ƶ�ͼ�鲻����
				{
					randDir++;
					goto startmove;
				}
				break;

			case 1: //�����ƶ�ͼ��
				if (_currPos%level < level - 1)
				{
					_arr[_currPos] = _arr[_currPos + 1];
					_currPos += 1;
					
					if (true == abnormal) //�����Ϸ�Ǳ�̬ģʽ����ô�������ͼ��ʱ��ʱ����ת90��
						_pngArr[_arr[_currPos]]->setRotation(_pngArr[_arr[_currPos]]->getRotation() + 270);

					_arr[_currPos] = level*level;
					_stepReport[_randNum] = 1;
					_randNum++;
				}
				else //��Ȼ���������ƶ�ͼ�鲻����
				{
					randDir++;
					goto startmove;
				}
				break;

			case 3:
				if (_currPos%level > 0)
				{
					_arr[_currPos] = _arr[_currPos - 1];
					_currPos -= 1;
					
					if (true == abnormal) //�����Ϸ�Ǳ�̬ģʽ����ô�������ͼ��ʱ��ʱ����ת90��
						_pngArr[_arr[_currPos]]->setRotation(_pngArr[_arr[_currPos]]->getRotation() + 270);

					_arr[_currPos] = level*level;
					_stepReport[_randNum] = 3;
					_randNum++;
				}
				else //��Ȼ���������ƶ�ͼ�鲻����
				{
					randDir++;
					goto startmove;
				}
				break;
			}
		}
	}
	/************����ͼƬ����***********/



	/*************��ʼ����Щ�и�õ�ͼ�鰴�մ��ҵ�˳��ڷź�*********/
	{
		for (int i = 0;i < level*level;i++)
		{
			if (level*level > _arr[i])
			{
				MoveTo* move = MoveTo::create(TT*6, Vec2((int(i / level))*_bianChang + _bianChang / 2, (int(i%level))*_bianChang + _bianChang / 2));
				_pngArr[_arr[i]]->runAction(move);

				addChild(_pngArr[_arr[i]]);
			}
		}

	    //�������˰ѽǿ���õ�������
		auto moveTo = MoveTo::create(TT*6, Vec2(level*_bianChang + _bianChang / 2, _bianChang / 2));
		_pngArr[level*level - level]->runAction(moveTo);
		addChild(_pngArr[level*(level - 1)]);
	}
	///*************�и�õ�ͼ�鱻���ҵ�˳��ڷź���*******/

	/******�����Ϸ�Ǽ�ʱģʽ����ô���һ�����־�������ʾʱ��*****/
	if (jishi)
	{
		_shengyuText = Text::create(Value(_shengyuTime).asString(), "", 128);
		_shengyuText->setRotation(270);
		_shengyuText->setTextColor(Color4B::BLUE);
		_shengyuText->setPosition(Vec2(1250, 390));
		addChild(_shengyuText,100);

		time_t now = time(&now);
		struct tm * initTime = localtime(&now);
		_initTime = retTimeNum(initTime);
	}
	/*******������־������******/

	/*****��һ�δ����Ƿ�ֹСͼ�黹û�й�λʱ����Һ��Ұ�����Ļ�ƶ�ͼ��****/
	{
		DelayTime* delayTime = DelayTime::create(TT * 7);
		Sequence* sequence = Sequence::create(delayTime,
			CallFunc::create([&]() {
			_gameState = 5;
		}),
			NULL);
		_mixPng->runAction(sequence);
	}
	/*****��ֹ�������*****/
	
	this->scheduleUpdate();//���ڸ���ÿһ֡�����update�ز�����
	return true;
}

void XIXI::update(float delta)
{
	switch (_gameState)
	{
	case 0://������
	{
		if ((_currPos%level == 0 && _currPos/level<level)|| _currPos / level < level - 1) //����հ׸��������棬�ǾͿ���������
		{
			_arr[_currPos] = _arr[_currPos + level];//�������Ǹ����ƶ���Сͼ�������հ׸�λ��
			movePic(_arr[_currPos + level], 0);

			_currPos += level; //�հ׸��û��������Ǹ�Сͼ������
			_arr[_currPos] = level*level;
			//_stepReport[_randNum] = 0; //��¼����������ߵĲ���
			//_randNum++; //�ƶ�ͼ��Ĵ���+1
			_stepNum++; //����ƶ��Ĳ���+1
		}
		else _gameState = 5;
	}
		/*_gameState = 5;*/
		break;
	case 2://������
	{
		if (_currPos / level > 0) //����հ׸��������棬�ǾͿ���������
		{
			_arr[_currPos] = _arr[_currPos - level];//�������Ǹ����ƶ���Сͼ�������հ׸�λ��
			movePic(_arr[_currPos - level], 2);

			_currPos -= level; //�հ׸��û��������Ǹ�Сͼ������
			_arr[_currPos] = level*level;
			//_stepReport[_randNum] = 1;//��¼����������ߵĲ���
			//_randNum++; //�ƶ�ͼ��Ĵ���+1
			_stepNum++; //����ƶ��Ĳ���+1
		}
		else _gameState = 5;
	}
		/*_gameState = 5;*/
		break;
	case 1://������
	{
		if (_currPos%level < level - 1 && _currPos < level*level) //����հ׸������ұߣ��ǾͿ���������
		{
			_arr[_currPos] = _arr[_currPos + 1];//���ұ��Ǹ����ƶ���Сͼ�������հ׸�λ��
			movePic(_arr[_currPos + 1], 1);

			_currPos += 1; //�հ׸��û����ұ��Ǹ�Сͼ������
			_arr[_currPos] = level*level;
			//_stepReport[_randNum] = 2;//��¼����������ߵĲ���
			//_randNum++;
			_stepNum++;
		}
		else _gameState = 5;
	}
		/*_gameState = 5;*/
		break;
	case 3://������
	{
		if (_currPos%level > 0) //����հ׸�������ߣ��ǾͿ���������
		{
			_arr[_currPos] = _arr[_currPos - 1];//������Ǹ����ƶ���Сͼ�������հ׸�λ��
			movePic(_arr[_currPos - 1], 3);

			_currPos -= 1; //�հ׸��û�������Ǹ�Сͼ������
			_arr[_currPos] = level*level;
			//_stepReport[_randNum] = 3; //��¼����������ߵĲ���
			//_randNum++;
			_stepNum++;
		}
		else _gameState = 5;
	}
		/*_gameState = 5;*/
		break;

	case 4://����һ����ԭͼƬ�İ�ť���깤����Կ���ɾ���������
	{
		if (0 < _randNum)
		{
			_gameState = (_stepReport[--_randNum] + 2) % 4;
		}
	}
		
		break;
	}



	/******�ж��Ƿ���ص��㷨��ʼ******/
	{
		int win = 1;
		if (abnormal) //�����Ϸģʽ����תģʽ
		{
			for (int i = 0;i < level*level;i++)
			{
				win *= (_arr[i] == i);
				win *= (0 == (int(_pngArr[i]->getRotation()) % 360));
			}
		}
		else //��Ȼ��Ϸ���Ǿ���ģʽ
		{
			for (int i = 0;i < level*level;i++)
			{
				win *= (_arr[i] == i);
			}
		}
		if (win) //���ƴ�ɳɹ���
		{
			//for (int i = 0;i < 4;i++) _dir[i]->setVisible(false);
			/******���������д����ȰѰ�ť�ͱ������رյ�********/
			{
				_mixPng->setVisible(false);_maxPng->setVisible(false);
				_bgPng->setContentSize(Size(WIDTH, HEIGHT));
				_bgPng->setScale(1);
				_bgPng->setPosition(1320 / 2, 390);
			}
			/*****�����ر��߼�����******/

			/****��ӡ�ɹ��ַ���****/
			{
				CCDictionary* chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
				string winstr = "congratulations";
				winstr = ((CCString*)chnStrings->objectForKey(winstr))->getCString();
				winstr.append(StringUtils::format("%f", zhe));

				string winstr2 = "congratulations2";
				winstr2 = ((CCString*)chnStrings->objectForKey(winstr2))->getCString();
				winstr.append(winstr2);
				auto text = Text::create(winstr, "", 32);

				text->setPosition(Vec2(1040, 390));
				text->setRotation(270);
				text->setTextColor(Color4B::GREEN);
				addChild(text, 99);
			}
			/****�ַ�����ӡ����****/

			/***���Ź��ر�������***/
			{
				SimpleAudioEngine::getInstance()->stopBackgroundMusic();
				SimpleAudioEngine::getInstance()->playBackgroundMusic("winmusic1.mp3");
			}
			/***���ű������ֽ���***/

			/*****��Ϊ���سɹ����������ö�Ӧ�Żݣ�С���Ǵ󣬱�������*****/
			{
				if (jishi && _shengyuTime>0) //�����Ϸ�Ǽ�ʱģʽ����ʣ��ʱ����
				{
					zhe = 0.0; //��ô�Ȱ��Żݵ���0����Ϊǰ���Żݶ����õ���һ�㶼���Ż�
				}
				else {
					string integerStr = "integer";
					integerStr.append(StringUtils::format("%d", clothesNum));

					int recordInt = UserDefault::getInstance()->getIntegerForKey(integerStr.c_str());

					if (zhe < recordInt)
					{
						UserDefault::getInstance()->setIntegerForKey(integerStr.c_str(), zhe);
						UserDefault::getInstance()->flush();
					}
				}
			}
			/*****�����Żݽ���*****/
			this->unscheduleUpdate();//�����ֹͣ����update����
		}
	}
	/******�ж��Ƿ���ص��߼�����******/

	/*******��������˼�ʱģʽ����ô��ʱ�߼���ʼ*******/
	{
		if (true == jishi) //�����Ϸ�Ǽ�ʱģʽ
		{
			time_t now = time(&now);
			struct tm *currentTime = localtime(&now);
			_currentTime = retTimeNum(currentTime);
			_shengyuTime = GAMETIME - (_currentTime - _initTime);
			_shengyuText->setText(Value(_shengyuTime).asString());

			
			if (_shengyuTime <= 0) //�����ʱģʽ�£�ʱ���Ѿ��Ĺ���
			{
				_shengyuText->setText("time over");
				_shengyuText->setFontSize(48);
				_shengyuText->setTextColor(Color4B::BLACK);
			}
		}
	}
	/*******���ڼ�ʱ���߼��㷨���˽���*******/
}


bool About::init() //������Ϸ��About��ĳ�ʼ��
{
	if (!Scene::init()) return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");

	const char * wishStr = ((CCString *)chnStrings->objectForKey("wish"))->getCString();
	const char * loveStr = ((CCString *)chnStrings->objectForKey("love"))->getCString();

	auto loveText = Text::create(loveStr,"",38);
	loveText->setPosition(Vec2(0, visibleSize.height / 2));
	loveText->setRotation(270);
	auto moveTo = MoveTo::create(1, Vec2(visibleSize.width / 2, visibleSize.height / 2));
	loveText->runAction(moveTo);
	addChild(loveText);


	auto wishBtn = MenuItemFont::create(wishStr, CC_CALLBACK_1(About::popSceneCallBack, this));
	wishBtn->setRotation(270);
	wishBtn->setPosition(1280, 160);

	auto menu = Menu::create(wishBtn, NULL);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	return true;
}

bool ClothesMenu::init()
{
	if (!Scene::init()) return false;
	// ����豸֧�ֵĿɼ�OpenGL��ͼ��С�������ڴ�С��
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Menu* menu = Menu::create();
	for (int i = 0;i < 14;i++)
	{
		string s = "xixi";
		s.append(StringUtils::format("%d.jpg",i));
		auto clothesItem = MenuItemImage::create(s.c_str(), s.c_str(),
			CC_CALLBACK_1(ClothesMenu::gameCallBack, this, i));
		clothesItem->setScale(0.25);
		clothesItem->setPosition(Vec2(i / 3 * 260 + 130, (i % 3) * 260 + 130));
		menu->addChild(clothesItem);
	}

	
	menu->setPosition(Vec2::ZERO);
	addChild(menu);


	auto backItem = Button::create("back.png");
	backItem->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		switch (type)
		{
		case Widget::TouchEventType::ENDED:
			SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->popScene();
		}
		return true;
	});
	backItem->setPosition(Vec2(1280, 740));
	addChild(backItem,99);

	return true;
}

bool Attainment::init()
{
	if (!Scene::init()) return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");

	for (int i = 0;i < MAXCLOTHES;i++)
	{
		string attainmentStr = "xixi";
		attainmentStr.append(StringUtils::format("%d", i));
		attainmentStr = ((CCString *)chnStrings->objectForKey(attainmentStr.c_str()))->getCString();

		string percentStr = "integer";
		percentStr.append(StringUtils::format("%d", i));
		int percentage = UserDefault::getInstance()->getIntegerForKey(percentStr.c_str());

		if (percentage >= 10)
		{
			string incompleteStr = ((CCString*)chnStrings->objectForKey("incomplete"))->getCString();
			attainmentStr.append(incompleteStr);
		}
		else {
			string unlockedStr = ((CCString*)chnStrings->objectForKey("unlocked"))->getCString();
			attainmentStr.append(unlockedStr);
			attainmentStr.append(StringUtils::format("%d%%", percentage * 10));
		}

		Text* attainmentText = Text::create(attainmentStr.c_str(), "", 32);
		if (percentage < 10)
			attainmentText->setTextColor(Color4B::GREEN);
		attainmentText->setRotation(270);
		attainmentText->setPosition(Vec2((i + 1) * 40, 390));
		addChild(attainmentText);
	}

	Button* backBtn = Button::create("back.png");
	backBtn->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		switch (type)
		{
		case Widget::TouchEventType::ENDED:
			SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->popScene();
			break;
		}
	});
	backBtn->setPosition(Vec2(1280, 740));
	addChild(backBtn);
	return true;
}


bool Help::init()
{
	if (!Scene::init()) return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto chnStrings = CCDictionary::createWithContentsOfFile("xixi.xml");
	string helpStr = ((CCString *)chnStrings->objectForKey("helpWord"))->getCString();
	
	
	Text* helpText = Text::create(helpStr, "", 48);
	helpText->setRotation(270);
	helpText->setPosition(Vec2(0, 390));
	MoveTo* moveTo = MoveTo::create(1, Vec2(visibleSize.width / 2, visibleSize.height / 2));
	helpText->runAction(moveTo);
	addChild(helpText);

	Button* backBtn = Button::create("back.png");
	backBtn->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type)
	{
		switch (type)
		{
		case Widget::TouchEventType::ENDED:
			SimpleAudioEngine::getInstance()->playEffect("effect5.mp3");
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->popScene();
		}
	});
	backBtn->setPosition(Vec2(1280, 740));
	addChild(backBtn);
	
	UserDefault::getInstance()->setStringForKey("string1", "string1");
	UserDefault::getInstance()->setStringForKey("string2", "ffff");
	UserDefault::getInstance()->flush();
	return true;
}


void HelloWorld::runFlush()
{
	int flagNum = UserDefault::getInstance()->getIntegerForKey("flag");
	if (999 != flagNum)
	{
		for (int i = 0;i < MAXCLOTHES;i++)
		{
			string s = "integer";
			s.append(StringUtils::format("%d", i));
			UserDefault::getInstance()->setIntegerForKey(s.c_str(), 10);
		}
		UserDefault::getInstance()->setIntegerForKey("flag", 999);
		UserDefault::getInstance()->flush();
	}
}

