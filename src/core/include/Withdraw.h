#pragma once

#include <QWidget>
#include "ui_WithdrawWindow.h"
#include <QMessageBox>

class LoggedInWindow;

class WithdrawWindow : public QWidget
{
    Q_OBJECT

public:
    WithdrawWindow(QWidget* parent = nullptr);
    ~WithdrawWindow();
    void setLog(LoggedInWindow* mainWindow);
    void on_WithdrawButton_clicked();
    void on_BackButton_clicked();
	void setAccountNumber(std::string accNum);

private:
    Ui::WithdrawWindow ui;
    LoggedInWindow* main;
	std::string accountNumber;
};
