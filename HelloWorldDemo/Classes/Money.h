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

	//update money every period
	void update(float f);
	
	//update the money label
	void updateMoneyLabel();

	bool checkMoney(int cost) const;

	void costMoney(int cost);

	void addMoney(int add);

	void setMoneyInPeriod(int moneyInPeriod);

	void addMoneyInPeriod(int add);

	void cutMoneyInPeriod(int cut);

private:
	int _money = 0;
	int _time = 0;
	std::string _moneyString;

	int _moneyInPeriod = MONEY_IN_PERIOD;
	int _period = PERIOD;
};
#endif // !__MONEY_H__

