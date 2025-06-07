#pragma once

#include <QtWidgets/QMainWindow>
#include "../../../BankApp/x64/Debug/qt/uic/ui_BankMainWindow.h"
#include "LoginWindow.h"
#include "SignUpWindow.h"
#include "LoggedInWindow.h"
#include "LoginBankomat.h"
#include "../../../../AdminPanel/AdminPanel.h"
#include <QProcess>
class BankApp : public QMainWindow
{
    Q_OBJECT

public:
    BankApp(QWidget *parent = nullptr);
    ~BankApp();
private slots:
	void on_LoginButton_clicked();
	void on_CreateAccButton_clicked();
	void on_ExitButton_clicked();
	void on_LoginBankomatButton_clicked();
	void on_AdminLoginButton_clicked();
private:
    Ui::BankMainWindow ui;
	LoginWindow* loginWindow = nullptr;;
	SignUpWindow* signUpWindow;
	LoggedInWindow* loggedInWindow = nullptr;;
	LoginBankomat* loginBankomat;
	AdminPanel* adminPanel;
};
