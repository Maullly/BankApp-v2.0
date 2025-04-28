#pragma once
#include <QWidget>
#include <QMessageBox>
#include "ui_LoginBankomat.h"
#include "Osoba.h"
class BankApp;
class BankomatLoggedIn;
class LoginBankomat : public QWidget
{
	Q_OBJECT
public:
	LoginBankomat(QWidget* parent = nullptr);
	~LoginBankomat();
	void setLog(BankApp* mainApp);
	void onNumberClicked();
	void onBackspaceClicked();
	void onClearClicked();
	void onLoginClicked();
	void on_BackButton_clicked();
private:
	Ui::LoginBankomatWindow ui;
	BankomatLoggedIn* log;
	BankApp* main;
	std::string accountNumber;
	QString enteredPin;
};

