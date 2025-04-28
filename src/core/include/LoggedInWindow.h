#pragma once

#include <QWidget>
#include "ui_LoggedInWindow.h"

class DepositWindow;
class WithdrawWindow;
class QuickCashWindow;
class LastTransactionsWindow;
class AccountBalanceWindow;
class PinChangeWindow;
class BankApp;
class TransferWindow;
class LoggedInWindow : public QWidget
{
    Q_OBJECT

public:
    LoggedInWindow(QWidget* parent = nullptr);
    ~LoggedInWindow();
	void setLog(BankApp* mainApp);
	void AccNumber(std::string accNum);
	void refreshBalance();
private slots:
	void on_AccountBalanceButton_clicked();
	void on_ChangePinButton_clicked();
	void on_TransferButton_clicked();
	void on_TransactionsButton_clicked();
	void on_LogOutButton_clicked();
	void on_WithdrawButton_clicked();
	void on_DepositButton_clicked();
private:
    Ui::LoggedInWindow ui;
	BankApp* main = nullptr;
	DepositWindow* dep;
	WithdrawWindow* with;
	QuickCashWindow* quick;
	LastTransactionsWindow* last;
	AccountBalanceWindow* bal;
	PinChangeWindow* pin;
	TransferWindow* trans;
	std::string accountNumber;
};