#ifndef  _GameMenu_Scene_H_
#define  _GameMenu_Scene_H_

#include "cocos2d.h"
#include"SocketClient.h"
#include"SocketServer.h"
#include "ui/UIEditBox/UIEditBox.h"

class GameMenu :public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuItemStartCallback(cocos2d::Ref*pSender);
	void menuItemSetCallback(cocos2d::Ref*pSender);
	void menuItemQuitCallback(cocos2d::Ref*pSender);
	void menuItemHelpCallback(cocos2d::Ref*pSender);
	CREATE_FUNC(GameMenu);
};

namespace cocos2d {
	namespace ui {
		class EditBox;
	}
}
class ServerMenu : public cocos2d::Layer, cocos2d::ui::EditBoxDelegate
{
public:
	static cocos2d::Scene* createScene();

	bool init() override;
	// a selector callback
	void menuStartServerCallback(cocos2d::Ref* pSender);
	void menuStartGameCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(ServerMenu);
private:
	void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
	void connectionSchdeule(float f);

	cocos2d::Label * connection_msg_;
	socket_server * socket_server_{ nullptr };
	boost::shared_ptr<talk_to_server>  socket_client_{ nullptr };
};

class ClientMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	bool init() override;
	// a selector callback
	//	void menuStartServerCallback(cocos2d::Ref* pSender);
	void menuStartGameCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(ClientMenu);
private:
	void wait_start();
	void startSchedule(float f);

	boost::shared_ptr<talk_to_server> socket_client_{ nullptr };
	cocos2d::Label* connection_msg_;
	int timer_ = 0;
};

class StartMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	bool init() override;
	// a selector callback
	void menuServerCallback(cocos2d::Ref* pSender);
	void menuClientCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(StartMenu);
};

#endif