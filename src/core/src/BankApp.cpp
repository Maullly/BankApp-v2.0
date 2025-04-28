#include "../include/BankApp.h"
#include <QDebug>
#include <QMessageBox>

BankApp::BankApp(QWidget* parent)
    : QMainWindow(parent), loggedInWindow(nullptr), signUpWindow(nullptr)
{
    ui.setupUi(this);
}

BankApp::~BankApp()
{
    delete loggedInWindow;
    delete signUpWindow;
}

void BankApp::on_LoginButton_clicked()
{
    qDebug() << "Login button clicked";
    if (!loginWindow) {
        loginWindow = new LoginWindow();
        loginWindow->setLog(this);  // Przekazanie wskaŸnika do g³ównego okna
    }
    loginWindow->show();  // Poka¿ okno LoggedInWindow
    close();
}

void BankApp::on_CreateAccButton_clicked()
{
    qDebug() << "Create Account button clicked";

    if (!signUpWindow) {
        signUpWindow = new SignUpWindow();
		signUpWindow->setLog(this);  // Przekazanie wskaŸnika do g³ównego okna
    }
    signUpWindow->show();  // Poka¿ okno rejestracji
    close();
}

void BankApp::on_ExitButton_clicked()
{
    qDebug() << "Exit button clicked";
    close();  // Zamknij g³ówne okno aplikacji
}

void BankApp::on_LoginBankomatButton_clicked()
{
    qDebug() << "Login ATM button clicked";
     if (!loginBankomat) {
            loginBankomat = new LoginBankomat();
            loginBankomat->setLog(this);  // Przekazanie wskaŸnika do g³ównego okna
        }
     loginBankomat->show();  // Poka¿ okno LoggedInWindow
     close();
}

void BankApp::on_AdminLoginButton_clicked()
{
    qDebug() << "Admin button clicked";
    if (!adminPanel) {
        adminPanel = new AdminPanel();
        adminPanel->setLog(this);
    }
    adminPanel->show();
    close();
}

