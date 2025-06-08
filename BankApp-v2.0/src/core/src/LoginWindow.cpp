#include "../include/LoginWindow.h"  
#include "../include/BankApp.h"  
#include "../include/Osoba.h"  
#include "../include/LoggedInWindow.h"  
#include <QMessageBox>  
#include <QDebug>  
#include <QDir>
#include <QProcess>
LoginWindow::LoginWindow(QWidget* parent)  
   : QWidget(parent)  
{  
   ui.setupUi(this);  
   ui.PasswordEdit->setEchoMode(QLineEdit::Password);  

   connect(ui.BackButton, &QPushButton::clicked, this, &LoginWindow::on_BackButton_clicked);  
   connect(ui.LogIntoButton, &QPushButton::clicked, this, &LoginWindow::on_LogIntoButton_clicked);  
}  

LoginWindow::~LoginWindow()  
{  

}  

void LoginWindow::setLog(BankApp* mainApp)  
{  
   main = mainApp;  
}  

void LoginWindow::setAdminMode(bool mode)
{
    adminMode = mode;
}

void LoginWindow::on_LogIntoButton_clicked()
{
    QString login = ui.LoginEdit->toPlainText().trimmed();
    QString credential = ui.PasswordEdit->text().trimmed();

    if (login.isEmpty() || credential.isEmpty()) {
        QMessageBox::warning(this, "Blad", "Wszystkie pola musza byc wypelnione!");
        return;
    }

    if (!Osoba::sprawdzHaslo(login.toStdString(), credential.toStdString())) {
        QMessageBox::warning(this, "Blad", "Niepoprawne dane logowania!");
        return;
    }
    // Weryfikacja u¿ytkownika
    if (!Osoba::czyZweryfikowany(login.toStdString()) && adminMode == false) {
        QMessageBox::warning(this, "Blad", "Uzytkownik nie zostal jeszcze zweryfikowany.");
        return;
    }
    // Sprawdzenie uprawnieñ administratora
    bool jestAdminem = Osoba::czyAdmin(login.toStdString());

    if (adminMode) {
        if (!jestAdminem) {
            QMessageBox::warning(this, "Blad", "Tylko administrator moze sie tu zalogowac!");
            return;
        }

        QString adminExePath = QDir::toNativeSeparators(
            QCoreApplication::applicationDirPath() + "/AdminPanel.exe"
        );

        if (QFile::exists(adminExePath)) {
            QProcess::startDetached(adminExePath);
            close();
        }
        else {
            QMessageBox::warning(this, "Blad", "Nie znaleziono pliku AdminPanel.exe!");
        }
    }
    else {
        // Tryb u¿ytkownika - admin te¿ mo¿e siê zalogowaæ jak zwyk³y u¿ytkownik
        QMessageBox::information(this, "Sukces", "Zalogowano pomyslnie!");
        if (log) { delete log; log = nullptr; }

        auto w = new LoggedInWindow();
        w->setLog(main);
        w->AccNumber(login.toStdString());
        w->show();
        close();
    }
}


void LoginWindow::on_BackButton_clicked()  
{  
   if (main) {  
       main->show();  
       close();  
   }  
   else  
   {  
       QMessageBox::critical(this, "Blad", "Glowne okno nie zostalo ustawione");  
   }  
}


