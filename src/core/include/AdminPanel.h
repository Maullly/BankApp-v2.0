#pragma once
#include "ui_AdminPanelWindow.h"
class BankApp;
class AdminPanel : public QWidget
{
	Q_OBJECT
public:
	AdminPanel(QWidget* parent = nullptr);
	~AdminPanel();
	void loadAccounts();
	void deleteAccount();
	void changePin();
	void setLog(BankApp* mainApp);
	void on_LogOutButton_clicked();
private:
	Ui::AdminWindow ui;
	BankApp* main;
};

