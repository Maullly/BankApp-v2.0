#pragma once
#include "./x64/Debug/qt/uic/ui_AdminPanel.h"
class BankApp;
class AdminPanel : public QWidget
{
	Q_OBJECT
public:
	AdminPanel(QWidget* parent = nullptr);
	~AdminPanel();
	void loadAccounts();
	void changePin();
	void setLog(BankApp* mainApp);
	void on_LogOutButton_clicked();
	void showEvent(QShowEvent* event);
	void changePassword();
	void acceptAccount();
	void initDatabase();
private:
	Ui::AdminWindow ui;
	BankApp* main;
};

