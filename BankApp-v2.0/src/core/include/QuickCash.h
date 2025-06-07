#pragma once
#include <QWidget>
#include <QMessageBox>
#include "..\..\..\BankApp\x64\Debug\qt\uic\ui_QuickCashWindow.h"
class BankomatLoggedIn;
class QuickCashWindow : public QWidget
{
	Q_OBJECT

public:
	QuickCashWindow(QWidget* parent = nullptr);
	~QuickCashWindow();
	void setLog(BankomatLoggedIn* mainWindow);
	void on_BackButton_clicked();
	void setAccountNumber(std::string accNum);
	void processWithdrawal(double withdrawalamount);

private:
	Ui::QuickCashWindow ui;
	BankomatLoggedIn* main;
	std::string accountNumber;
};

