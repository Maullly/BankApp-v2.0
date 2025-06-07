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
        loginWindow->setLog(this);  // Przekazanie wska�nika do g��wnego okna
    }
    loginWindow->show();  // Poka� okno LoggedInWindow
    close();
}

void BankApp::on_CreateAccButton_clicked()
{
    qDebug() << "Create Account button clicked";

    if (!signUpWindow) {
        signUpWindow = new SignUpWindow();
		signUpWindow->setLog(this);  // Przekazanie wska�nika do g��wnego okna
    }
    signUpWindow->show();  // Poka� okno rejestracji
    close();
}

void BankApp::on_ExitButton_clicked()
{
    qDebug() << "Exit button clicked";
    close();  // Zamknij g��wne okno aplikacji
}

void BankApp::on_LoginBankomatButton_clicked()
{
    qDebug() << "Login ATM button clicked";
     if (!loginBankomat) {
            loginBankomat = new LoginBankomat();
            loginBankomat->setLog(this);  // Przekazanie wska�nika do g��wnego okna
        }
     loginBankomat->show();  // Poka� okno LoggedInWindow
     close();
}

void BankApp::on_AdminLoginButton_clicked()
{
    qDebug() << "Admin button clicked";

    // �cie�ka wzgl�dna do AdminPanel.exe
    QString adminExePath = QDir::toNativeSeparators(
        QCoreApplication::applicationDirPath() + "/AdminPanel.exe"
    );

    qDebug() << "Uruchamiam: " << adminExePath;

    if (QFile::exists(adminExePath)) {
        QProcess::startDetached(adminExePath);
        this->close(); 
    }
    else {
        qDebug() << "Nie znaleziono AdminPanel.exe pod �cie�k�:" << adminExePath;
        QMessageBox::warning(this, "B��d", "Nie znaleziono pliku AdminPanel.exe!");
    }
}