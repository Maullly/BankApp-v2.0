#pragma once

#include <QWidget>
#include "ui_LoginWindow.h"
#include "Osoba.h"
class BankApp;
class LoggedInWindow;
class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();
	void setLog(BankApp* mainApp);
	void on_LogIntoButton_clicked();
	void on_BackButton_clicked();

private:
    Ui::LoginWindow ui;
	BankApp* main;
	LoggedInWindow* log;
};