#pragma once
#include <QWidget>
#include <QMessageBox>
#include "ui_LastTransactionsWindow.h"
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
private:
	Ui::LastTransactionsWindow ui;
	LoggedInWindow* main;
	std::string accountNumber;
};

