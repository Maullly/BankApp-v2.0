#pragma once
#include "ui_DepositBankomatWindow.h"
class BankomatLoggedIn;
class DepositBankomat : public QWidget
{
	Q_OBJECT

public:
	DepositBankomat(QWidget* parent = nullptr);
	~DepositBankomat();
	void setLog(BankomatLoggedIn* mainApp);
	void setAccountNumber(std::string accNum);
	void onNumberClicked();
	void onBackspaceClicked();
	void onClearClicked();
	void onDepositClicked();
	void on_BackButtonClicked();
private:
	Ui::BankomatDepositWindow ui;
	BankomatLoggedIn* main;
	std::string accountNumber;
};


