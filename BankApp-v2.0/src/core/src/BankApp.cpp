#include "../include/BankApp.h"
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
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

    // Œcie¿ka wzglêdna do AdminPanel.exe
    QString adminExePath = QDir::toNativeSeparators(
        QCoreApplication::applicationDirPath() + "/AdminPanel.exe"
    );

    qDebug() << "Uruchamiam: " << adminExePath;

    if (QFile::exists(adminExePath)) {
        QProcess::startDetached(adminExePath);
        this->close(); 
    }
    else {
        qDebug() << "Nie znaleziono AdminPanel.exe pod œcie¿k¹:" << adminExePath;
        QMessageBox::warning(this, "B³¹d", "Nie znaleziono pliku AdminPanel.exe!");
    }
}