#ifndef __ELECTRICITY_H__
#define __ELECTRICITY_H__

#include"cocos2d.h"

class Electricity:public cocos2d::LabelBMFont
{
public:

	CREATE_FUNC(Electricity);

	//set initial electricity
	bool init() override;

	//update the electricity lable
	void updateLabel();

	//check if electricity is enough
	bool checkElectricity(int cost);

	//cost electricity for building
	void costElectricity(int cost);

	//when buildings are destroyed, free electricity
	void freeElectricity(int free);

	//when build Power Plant or Base
	void addElectricity(int add);

	//when destroy Power Plant or Base
	void cutElectricity(int cut);


private:
	int _electricity = 0;

	//the electricity that has been distributed
	int _usedElectricity = 0;

	std::string _electricityString;
	std::string _usedElectricityString;

	//combine _usedElectricityString with _electricityString
	std::string _showElectricity;

};


#endif // !__ELECTRICITY_H__

