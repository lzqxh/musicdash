#include "SongSelectionScene.h"
#include "consts\ResolutionConst.h"
#include "GameWorld.h"
#include "OptionsScene.h"

using namespace cocos2d;


bool SongSelectionScene::init() {
	if (!Scene::init()) return false;

	bg = Layer::create();
	addChild(bg);

	auto bgPic = Sprite::create("songselectscence/bg.png");
	bgPic->setAnchorPoint(Point(0,0));
	auto fgPic = Sprite::create("songselectscence/fg.png");
	fgPic->setAnchorPoint(Point(0,0));
	bg->addChild(bgPic, -1);
	bg->addChild(fgPic);

	auto backButton = MenuItemImage::create("songselectscence/button_back_n.png", "songselectscence/button_back_s.png",
		[this](Ref *){Director::getInstance()->popScene();});
	auto startButton = MenuItemImage::create("songselectscence/button_start_n.png", "songselectscence/button_start_s.png",
		[this](Ref *){
			DataVo::inst()->load(songs[focus]);
			DataVo::inst()->setMode(mode);
			auto scene = GameWorld::create();
			Director::getInstance()->pushScene(scene);
		});
	backButton->setPosition(ccp(161, 122));
	backButton->setAnchorPoint(ccp(0.5, 0.5));
	startButton->setPosition(ccp(404, 122));
	startButton->setAnchorPoint(ccp(0.5, 0.5));
	menu = Menu::create(backButton, startButton, nullptr);
	menu->setPosition(ccp(0, 0));
	bg->addChild(menu);

//	auto title = Sprite::create();
//	title->setSpriteFrame("xuanzeyinyue.png");
//	title->setAnchorPoint(Point(0, 1));
//	title->setScale(0.7f);
//	title->setPosition(30, designHeight-50);
//	bg->addChild(title, 2);

	for (int i = 0; i < 5; i++) {
		stars[i] = Sprite::create("songselectscence/star_1.png");
		stars[i]->setAnchorPoint(ccp(0.5, 0));
		stars[i]->setPosition(ccp(225 + i * 78 , 511));
		bg->addChild(stars[i]);
	}
	//just for test;
	for (int i = 1; i <= 3; i++) {
		std::string musicname = "songs/song" + std::to_string(i);
		songs.push_back(musicname);
	}
	difficult.push_back(3);
	difficult.push_back(1);
	difficult.push_back(4);

 //添加列表
	float w = 98;
	float h = 198;

	focus = 0;
	mode = 0;
	namePic = musicPic = nullptr;
	showSongInfo(0);

	for (int i = 0; i < 3; i++) {
		modes[i] = Sprite::create();
		modes[i]->setAnchorPoint(ccp(0.5, 0.5));
		modes[i]->setPosition(180+ i * 154, 447);
		bg->addChild(modes[i], -1);
	}
	showModeInfo(0);

    TableView *tableView= TableView::create(this, Size(464, 224));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
    tableView->setAnchorPoint(ccp(0, 0));
    tableView->setPosition(98, 196);
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    bg->addChild(tableView, -1);
    tableView->reloadData();

	return true;
}

void SongSelectionScene::onEnter() {
	Scene::onEnter();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(SongSelectionScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SongSelectionScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void SongSelectionScene::onExit() {
	Scene::onExit();
}

bool SongSelectionScene::onTouchBegan( Touch * touch, Event *event) {
	return true;
}

void SongSelectionScene::onTouchEnded( Touch * touch, Event *event) {
	for (int i = 0; i < 2; i++) {
		if (modes[i]->boundingBox().containsPoint(touch->getLocation())) 
			showModeInfo(i);
	}
}

void SongSelectionScene::showModeInfo(int idx) {
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("songselectscence/button_mode.plist");
	mode = idx;
	for (int i = 0; i < 3; i++) {
		std::string url = "button_mode" + std::to_string(i+1) + "_" + (i == mode ? "s" : "n") + ".png";
		modes[i]->setSpriteFrame(cache->getSpriteFrameByName(url));
	}
}

void SongSelectionScene::showSongInfo(int idx) {
	std::string musicname = songs[idx];
	if (musicPic) {
		bg->removeChild(musicPic);
	}
	musicPic = Sprite::create(musicname + "/pic.png");
	musicPic->setPosition(ccp(334, 831));
	musicPic->setAnchorPoint(ccp(0.5, 0.5));
	musicPic->setScale(0.9f);
	bg->addChild(musicPic);

	if (namePic) {
		bg->removeChild(namePic);
	}
	namePic = Sprite::create(musicname + "/name.png");
	namePic->setPosition(ccp(334, 585));
	namePic->setAnchorPoint(ccp(0.5, 0.5));
	namePic->setScale(0.9f);
	bg->addChild(namePic);

	CCTexture2D *aTexture=CCTextureCache::sharedTextureCache()->addImage("songselectscence/star_1.png");
	CCTexture2D *bTexture=CCTextureCache::sharedTextureCache()->addImage("songselectscence/star_2.png");
	for (int i = 0; i < difficult[idx]; i++) 
		stars[i]->setTexture(aTexture);
	for (int i = difficult[idx]; i < 5; i++)
		stars[i]->setTexture(bTexture);

	if (OptionsScene::ifMusic)
		CocosDenshion::SimpleAudioEngine::getInstance()->
			playBackgroundMusic((musicname + "/demo.mp3").c_str());
}
void SongSelectionScene::scrollViewDidScroll(ScrollView *view) { }
void SongSelectionScene::scrollViewDidZoom(ScrollView *view) { }


void SongSelectionScene::tableCellTouched(TableView *table, TableViewCell *cell) {
	auto lastCell = table->cellAtIndex(focus);
	if (lastCell) {
		CCTexture2D *aTexture=CCTextureCache::sharedTextureCache()->addImage("songselectscence/table_list_bg.png");
		CCSprite *pSprite=(CCSprite *)lastCell->getChildByTag(221);
		pSprite->setTexture(aTexture);
	}

	focus = cell->getIdx();
	CCTexture2D *aTexture=CCTextureCache::sharedTextureCache()->addImage("songselectscence/table_list_bg_s.png");
	CCSprite *pSprite=(CCSprite *)cell->getChildByTag(221);
	pSprite->setTexture(aTexture);
	CCBlink *blink_ = CCBlink::create(0.2f, 1);
   cell->runAction(blink_);

	showSongInfo(focus);

}

CCSize SongSelectionScene::cellSizeForTable(TableView *table) {
    return CCSizeMake(464, 76);
}

//生成cell
TableViewCell* SongSelectionScene::tableCellAtIndex(TableView *table, ssize_t idx) {
    CCString *nameString=CCString::createWithFormat("%s/name.png", songs[idx].data());
    
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();
	}
	else cell->removeAllChildren();

	Sprite *iconBg = Sprite::create(
		focus == idx ? "songselectscence/table_list_bg_s.png" : "songselectscence/table_list_bg.png");
	Sprite *iconSprite = Sprite::create(nameString->getCString());
	iconSprite->setNormalizedPosition(ccp(0.5, 0.5));
	iconBg->addChild(iconSprite);
	iconBg->setAnchorPoint(CCPointZero);
	iconBg->setPosition(ccp(0, 0));
	iconBg->setTag(221);
	cell->addChild(iconBg);
    return cell;
}

ssize_t SongSelectionScene::numberOfCellsInTableView(TableView *table) {
	return songs.size();
}

void SongSelectionScene::tableCellHighlight(TableView *table, TableViewCell *cell) {
}

void SongSelectionScene::tableCellUnhighlight(TableView *table, TableViewCell *cell) {
}


