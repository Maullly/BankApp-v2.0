#pragma once
#include <QWidget>
#include <QString>
#include "..\..\..\BankApp\x64\Debug\qt\uic\ui_TransferWindow.h"
class LoggedInWindow;
class TransferWindow : public QWidget
{
	Q_OBJECT
public:
	TransferWindow(QWidget* parent = nullptr);
	~TransferWindow();
	void setLog(LoggedInWindow* mainWindow);
	void on_TransferButton_clicked();
	void on_BackButton_clicked();
	void setAccountNumber(std::string accNum);
private:
	Ui::TransferWindow ui;
	LoggedInWindow* main;
	std::string accountNumber;
	QString enteredPassword;
	QString enteredPin;
};

