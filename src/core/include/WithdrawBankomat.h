#pragma once
#include "ui_WithdrawBankomatWindow.h"
class BankomatLoggedIn;
class WithdrawBankomatWindow : public QWidget
{
	Q_OBJECT

public:
	WithdrawBankomatWindow(QWidget* parent = nullptr);
	~WithdrawBankomatWindow();
	void setLog(BankomatLoggedIn* mainApp);
	void setAccountNumber(std::string accNum);
	void onNumberClicked();
	void onBackspaceClicked();
	void onClearClicked();
	void onWithdrawClicked();
	void on_BackButton_clicked();
private:
	Ui::WithdrawBankomatWindow ui;
	BankomatLoggedIn* main;
	std::string accountNumber;
};

