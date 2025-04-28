#pragma once
#include <QWidget>
#include "ui_ChangePinWindow.h"
#include "Osoba.h"
class LoggedInWindow;
class PinChangeWindow : public QWidget
{
	Q_OBJECT
public:
	PinChangeWindow(QWidget* parent = nullptr);
	~PinChangeWindow();
	void setLog(LoggedInWindow* mainWindow);
	void on_BackButton_clicked();
	void on_ChangePinButton_clicked();
	void setAccountNumber(std::string accNum);
private:
	Ui::ChangePinWindow ui;
	LoggedInWindow* main;
	std::string accountNumber;
};