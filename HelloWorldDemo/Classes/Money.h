#ifndef __MONEY_H__
#define __MONEY_H__

#include"cocos2d.h"
#include"SystemHeader.h"
#include<string>

class Money:public cocos2d::LabelBMFont
{
public:

	CREATE_FUNC(Money);

	//set initial money
	bool init() override;

	//update _money every period
	void update(float f);
	
	//when _money changed, update the lable
	void updateMoneyLable();

	bool checkMoney(int cost) const;

	void costMoney(int cost);

	void increaseMoney(int amount);

	void setMoneyInPeriod(int moneyInPeriod);

	void addMoneyInPeriod(int increase);

private:
	int _money = 0;
	int _time = 0;
	std::string _moneyString;

	int _moneyInPeriod = MONEY_IN_PERIOD;
	int _period = PERIOD;
};
#endif // !__MONEY_H__

