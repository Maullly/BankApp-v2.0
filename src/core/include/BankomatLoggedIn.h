#pragma once
#include <QWidget>
#include "ui_BankomatWindow.h"
class BankApp;
class DepositBankomat;
class WithdrawBankomatWindow;
class QuickCashWindow;
class BankomatLoggedIn : public QWidget
{
	Q_OBJECT
public:
	BankomatLoggedIn(QWidget* parent = nullptr);
	~BankomatLoggedIn();
	void setLog(BankApp* mainApp);
	void AccNumber(std::string accNum);
private slots:
	void on_DepositButton_clicked();
	void on_WithdrawButton_clicked();
	void on_QuickCashButton_clicked();
	void on_ChangePinButton_clicked();
	void on_LogOutButton_clicked();
private:
	Ui::BankomatWindow ui;
	BankApp* main = nullptr;
	DepositBankomat* dep;
	WithdrawBankomatWindow* with;
	QuickCashWindow* quick;
	std::string accountNumber;
};


