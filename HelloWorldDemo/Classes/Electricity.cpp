#include"Electricity.h"

bool Electricity::init()
{
	_electricity = 50;
	_usedElectricity = 0;
	_electricityString = std::to_string(_electricity);
	_usedElectricityString = std::to_string(_usedElectricity);
	_showElectricity = _usedElectricityString + "/" + _electricityString;
	return initWithString(_showElectricity, "fonts/Maker Felt.ttf", 32);
}

void Electricity::updateLabel()
{
	if (_electricity < _usedElectricity)
	{
		_unitManager->stopAllBuildingUnitUpdate();
		_tempMoneyInPeriod = _money->getMoneyInPeriod();
		_money->setMoneyInPeriod(MONEY_IN_PERIOD);
		powerOff = 1;
	}
	else
	{
		if (powerOff == 1)
		{
			_unitManager->startAllBuildingUnitUpdate();
			_money->setMoneyInPeriod(_tempMoneyInPeriod);
			powerOff = 0;
		}
	}
	_electricityString = std::to_string(_electricity);
	_usedElectricityString = std::to_string(_usedElectricity);
	_showElectricity = _usedElectricityString + " / " + _electricityString;
	setString(_showElectricity);
}

bool Electricity::checkElectricity(int cost) const
{
	return (_electricity - _usedElectricity >= cost);
}

void Electricity::costElectricity(int cost)
{
	_usedElectricity += cost;
	updateLabel();
}

void Electricity::freeElectricity(int free)
{
	_usedElectricity -= free;
	updateLabel();
}

void Electricity::addElectricity(int add)
{
	_electricity += add;
	updateLabel();
}

void Electricity::cutElectricity(int cut)
{
	_electricity -= cut;
	updateLabel();
}

