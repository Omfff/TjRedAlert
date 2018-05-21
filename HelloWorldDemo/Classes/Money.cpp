#include"Money.h"

bool Money::init()
{
	_money = INITIAL_MONEY;
	_moneyString = std::to_string(_money);
	return initWithString(_moneyString, "fonts/MoneyFont.fnt");
}

void Money::update(float f)
{
	if (++_time % _period == 0) {
		_money += _moneyInPeriod;
		updateMoneyLable();
	}
}

void Money::updateMoneyLable()
{
	_moneyString = std::to_string(_money);
	setString(_moneyString);
}

bool Money::checkMoney(int cost)const
{
	return (_money >= cost);
}

void Money::costMoney(int cost)
{
	_money -= cost;
	updateMoneyLable();
}

void Money::increaseMoney(int amount)
{
	_money += amount;
}

void Money::setMoneyInPeriod(int moneyInPeriod)
{
	_moneyInPeriod = moneyInPeriod;
}

void Money::addMoneyInPeriod(int increase)
{
	_moneyInPeriod += increase;
}