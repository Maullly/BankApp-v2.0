#pragma once

#include <QWidget>
#include "ui_DepositWindow.h"
#include <QMessageBox>
class LoggedInWindow;
class DepositWindow : public QWidget
{
    Q_OBJECT

public:
    DepositWindow(QWidget* parent = nullptr);
    ~DepositWindow();
	void setLog(LoggedInWindow* mainWindow);
	void on_DepositButton_clicked();
	void on_BackButton_clicked();
	void setAccountNumber(std::string accNum);
private:
    Ui::DepositWindow ui;
	LoggedInWindow* main;
	std::string accountNumber;
};