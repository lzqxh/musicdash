#ifndef __SONGSELECTIONSCENE_H__
#define __SONGSELECTIONSCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include <string>

USING_NS_CC;
USING_NS_CC_EXT;


class SongSelectionScene : 
	public cocos2d::Scene ,
	public cocos2d::extension::TableViewDelegate,
	public cocos2d::extension::TableViewDataSource  {
public:

	virtual bool init();
	CREATE_FUNC(SongSelectionScene);

	void showSongInfo(int);

	virtual void onEnter();
	virtual void onExit();

	bool onTouchBegan( Touch *, Event *);
	void onTouchEnded( Touch *, Event *);

    virtual void scrollViewDidScroll(ScrollView* view);
    virtual void scrollViewDidZoom(ScrollView* view);
    
    //����ĸ�cell
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    //ÿ��cell��size
    virtual cocos2d::CCSize cellSizeForTable(TableView *table);
    //����cell
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    //cell������
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    //����ȥ��ʱ�򣬾��Ǹ�����ʾ������������ø���״̬
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    //�ɿ���ʱ��ȡ������״̬
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
private:
	Layer *bg;
	Layer *fg;
	Menu *menu;
	std::vector<std::string> songs;
	std::vector<int> difficult;
	Point beginPoint;
	int focus;
	Sprite* musicPic;
	Sprite* namePic;
	Sprite* stars[5];
};

#endif