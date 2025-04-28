#pragma once

#include <QWidget>

#include "ui_SignUpWindow.h"
class BankApp;
class AdminPanel;
class SignUpWindow : public QWidget
{
    Q_OBJECT

public:
    SignUpWindow(QWidget* parent = nullptr);
    ~SignUpWindow();
	void setLog(BankApp* mainApp);
	void on_RegistrationButton_clicked();
	void on_BackButton_clicked();
private:
    Ui::SignUpWindow ui;
	BankApp* main;
};