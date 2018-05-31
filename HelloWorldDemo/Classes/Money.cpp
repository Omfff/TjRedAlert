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
		updateMoneyLabel();
	}
}

void Money::updateMoneyLabel()
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
	updateMoneyLabel();
}

void Money::addMoney(int add)
{
	_money += add;
}

void Money::setMoneyInPeriod(int moneyInPeriod)
{
	_moneyInPeriod = moneyInPeriod;
}

void Money::addMoneyInPeriod(int add)
{
	_moneyInPeriod += add;
}

void Money::cutMoneyInPeriod(int cut)
{
	_moneyInPeriod -= cut;
}