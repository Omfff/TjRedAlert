#include"Electricity.h"

bool Electricity::init()
{
	_electricity = 100;
	_usedElectricity = 0;
	_electricityString = std::to_string(_electricity);
	_usedElectricityString = std::to_string(_usedElectricity);
	_showElectricity = _usedElectricityString + "/" + _electricityString;
	return initWithString(_showElectricity, "fonts/BMFont.fnt");
}

void Electricity::updateLabel()
{
	_electricityString = std::to_string(_electricity);
	_usedElectricityString = std::to_string(_usedElectricity);
	_showElectricity = _electricityString + " / " + _usedElectricityString;
	setString(_showElectricity);
}

bool Electricity::checkElectricity(int cost)
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

