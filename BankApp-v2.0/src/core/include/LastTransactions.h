#pragma once
#include <QWidget>
#include <QMessageBox>
#include "..\..\..\BankApp\x64\Debug\qt\uic\ui_LastTransactionsWindow.h"
class LoggedInWindow;
class LastTransactionsWindow : public QWidget
{
	Q_OBJECT
public:
	LastTransactionsWindow(QWidget* parent = nullptr);
	~LastTransactionsWindow();
	void setLog(LoggedInWindow* mainWindow);
	void setAccountNumber(std::string accNum);
	void on_BackButton_clicked();
	void loadTransactions();
	void showEvent(QShowEvent* event);
private:
	Ui::LastTransactionsWindow ui;
	LoggedInWindow* main;
	std::string accountNumber;
};

