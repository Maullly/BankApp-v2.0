#include "../include/LoginWindow.h"  
#include "../include/BankApp.h"  
#include "../include/Osoba.h"  
#include "../include/LoggedInWindow.h"  
#include <QMessageBox>  
#include <QDebug>  

LoginWindow::LoginWindow(QWidget* parent)  
   : QWidget(parent)  
{  
   ui.setupUi(this);  
   // Corrected the issue by replacing QTextEdit with QLineEdit for PasswordEdit  
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

void LoginWindow::on_LogIntoButton_clicked()  
{  
   QString login = ui.LoginEdit->toPlainText().trimmed();  
   QString credential = ui.PasswordEdit->text().trimmed();


   if (login.isEmpty() || credential.isEmpty()) {  
       QMessageBox::warning(this, "B³¹d", "Wszystkie pola musz¹ byæ wype³nione!");  
       return;  
   }  

   if (Osoba::sprawdzHaslo(login.toStdString(), credential.toStdString())) {
       QMessageBox::information(this, "Sukces", "Zalogowano pomyœlnie!");  
       if (log) { delete log; log = nullptr; }  

       auto w = new LoggedInWindow();  
       w->setLog(main);  
       w->AccNumber(login.toStdString());  
       w->show();  
       close();  
   }  
   else {  
       QMessageBox::warning(this, "B³¹d", "Niepoprawne dane logowania!");  
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
       QMessageBox::critical(this, "Error", "Main window not set");  
   }  
}
