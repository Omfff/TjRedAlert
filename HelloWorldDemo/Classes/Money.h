#ifndef __MONEY_H__
#define __MONEY_H__

#include"cocos2d.h"
#include"SystemHeader.h"
#include<string>

class Money:public cocos2d::LabelTTF
{
public:

	CREATE_FUNC(Money);

	//set initial money
	bool init() override;

	//update money every period
	void update(float f);
	
	//update the money label
	void updateMoneyLabel();

	//check if money is enough
	bool checkMoney(int cost) const;

	//cost money for buidldings or soldiers
	void costMoney(int cost);

	//when destroy enemy's buidldings or soldiers ,get money
	void addMoney(int add);

	//change money increase in specific time to any number
	void setMoneyInPeriod(int moneyInPeriod);

	//addmoney increase in specific time
	void addMoneyInPeriod(int add);

	//when some buidings are destroyed, decrease money increase in specific time
	void cutMoneyInPeriod(int cut);

	int getMoneyInPeriod() { return _moneyInPeriod; }

private:
	int _money = 0;
	int _time = 0;
	std::string _moneyString;

	//money increase in a period
	int _moneyInPeriod = MONEY_IN_PERIOD;
	//money increase period
	int _period = PERIOD;
	
};


#endif // !__MONEY_H__

