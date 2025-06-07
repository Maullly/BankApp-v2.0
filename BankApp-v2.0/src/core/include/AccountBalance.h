#pragma once
#include <QWidget>
#include "..\..\..\BankApp\x64\Debug\qt\uic\ui_BalanceWindow.h"
class LoggedInWindow;
class AccountBalanceWindow : public QWidget
{
    Q_OBJECT
public:
    AccountBalanceWindow(QWidget* parent = nullptr);
    ~AccountBalanceWindow();
    void setLog(LoggedInWindow* mainWindow);
    void on_BackButton_clicked();
    void setAccountNumber(std::string accNum);
    void setBalanceText(QString balance);
private:
    Ui::BalanceWindow ui;
    LoggedInWindow* main;
    std::string accountNumber;
};
