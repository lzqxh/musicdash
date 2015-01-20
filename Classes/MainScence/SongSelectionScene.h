#ifndef __SONGSELECTIONSCENE_H__
#define __SONGSELECTIONSCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"


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
	void showModeInfo(int);

	virtual void onEnter();
	virtual void onExit();

	bool onTouchBegan( Touch *, Event *);
	void onTouchEnded( Touch *, Event *);

    virtual void scrollViewDidScroll(ScrollView* view);
    virtual void scrollViewDidZoom(ScrollView* view);
    
    //点击哪个cell
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    //每个cell的size
    virtual cocos2d::CCSize cellSizeForTable(TableView *table);
    //生成cell
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    //cell的数量
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    //按下去的时候，就是高亮显示，这里可以设置高亮状态
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    //松开的时候，取消高亮状态
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
private:
	Layer *bg;
	Layer *fg;
	Menu *menu;
	std::vector<std::string> songs;
	std::vector<int> difficult;
	Point beginPoint;
	int focus, mode;
	Sprite* musicPic;
	Sprite* namePic;
	Sprite* stars[5];
	Sprite* modes[3];
};

#endif
