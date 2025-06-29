#include "../include/Transfer.h"
#include "../include/LoggedInWindow.h"
#include "../include/Osoba.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>

TransferWindow::TransferWindow(QWidget* parent)
{
    ui.setupUi(this);
    connect(ui.BackButton, &QPushButton::clicked, this, &TransferWindow::on_BackButton_clicked);
    connect(ui.TransferButton, &QPushButton::clicked, this, &TransferWindow::on_TransferButton_clicked);
}

TransferWindow::~TransferWindow()
{
}

void TransferWindow::setLog(LoggedInWindow* mainWindow)
{
    main = mainWindow;
}

void TransferWindow::on_TransferButton_clicked()
{
    QString recipientAccount = ui.AccountNumberEdit->text();
    double amount = ui.TransferAmountEdit->text().toDouble();

    if (recipientAccount.isEmpty() || amount <= 0) {
        QMessageBox::warning(this, "Blad", "Wprowadz poprawny numer i kwote!");
        return;
    }
    // Krok 1: Autoryzacja has�em
    bool ok;
    QString inputPassword = QInputDialog::getText(this, "Autoryzacja", "Wprowadz haslo:", QLineEdit::Password, "", &ok);
    if (!ok || inputPassword.isEmpty()) {
        QMessageBox::warning(this, "Anulowano", "Nie wprowadzono hasla.");
        return;
    }
    if (!Osoba::sprawdzHaslo(accountNumber, inputPassword.toStdString())) {
        QMessageBox::critical(this, "Blad", "Niepoprawne haslo!");
        return;
    }

    // Krok 2: Autoryzacja PIN-em
    QString inputPin = QInputDialog::getText(this, "Autoryzacja", "Wprowadz PIN:", QLineEdit::Password, "", &ok);
    if (!ok || inputPin.isEmpty()) {
        QMessageBox::warning(this, "Anulowano", "Nie wprowadzono PIN-u.");
        return;
    }
    if (!Osoba::sprawdzPin(accountNumber, inputPin.toStdString())) {
        QMessageBox::critical(this, "Blad", "Niepoprawny PIN!");
        return;
    }

    // Pobierz dane nadawcy
    QSqlQuery senderQuery;
    senderQuery.prepare("SELECT * FROM users WHERE id = :id");
    senderQuery.bindValue(":id", QString::fromStdString(accountNumber));
    if (!senderQuery.exec() || !senderQuery.next()) {
        QMessageBox::critical(this, "Blad", "Nie znaleziono Twojego konta!");
        return;
    }

    double senderBalanceBefore = senderQuery.value("balance").toDouble();
    if (senderBalanceBefore < amount) {
        QMessageBox::warning(this, "Blad", "Niewystarczajace srodki!");
        return;
    }

    double senderBalanceAfter = senderBalanceBefore - amount;

    // Pobierz dane odbiorcy
    QSqlQuery recipientQuery;
    recipientQuery.prepare("SELECT * FROM users WHERE id = :id");
    recipientQuery.bindValue(":id", recipientAccount);
    if (!recipientQuery.exec() || !recipientQuery.next()) {
        QMessageBox::critical(this, "Blad", "Konto odbiorcy nie istnieje!");
        return;
    }

    double recipientBalanceBefore = recipientQuery.value("balance").toDouble();
    double recipientBalanceAfter = recipientBalanceBefore + amount;

    // Rozpocznij transakcj� SQL
    QSqlDatabase::database().transaction();

    // Zaktualizuj saldo nadawcy
    QSqlQuery updateSender;
    updateSender.prepare("UPDATE users SET balance = :balance WHERE id = :id");
    updateSender.bindValue(":balance", senderBalanceAfter);
    updateSender.bindValue(":id", QString::fromStdString(accountNumber));
    if (!updateSender.exec()) {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Blad", "Nie udalo sie zaktualizowac salda nadawcy!");
        return;
    }

    // Zaktualizuj saldo odbiorcy
    QSqlQuery updateRecipient;
    updateRecipient.prepare("UPDATE users SET balance = :balance WHERE id = :id");
    updateRecipient.bindValue(":balance", recipientBalanceAfter);
    updateRecipient.bindValue(":id", recipientAccount);
    if (!updateRecipient.exec()) {
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, "Blad", "Nie udalo sie zaktualizowac salda odbiorcy!");
        return;
    }

    // Zatwierd� transakcj�
    QSqlDatabase::database().commit();

    // Obiekt Osoba � nadawca
    Osoba sender(
        senderQuery.value("id").toString().toStdString(),
        std::to_string(senderQuery.value("pin").toInt()),
        senderQuery.value("password").toString().toStdString(),
        senderQuery.value("first_name").toString().toStdString(),
        senderQuery.value("last_name").toString().toStdString(),
        senderQuery.value("birth_date").toString().toStdString(),
        senderQuery.value("email").toString().toStdString(),
        senderQuery.value("city").toString().toStdString(),
        senderQuery.value("postal_code").toString().toStdString(),
        senderQuery.value("street").toString().toStdString(),
        senderQuery.value("house_number").toString().toStdString(),
        senderBalanceAfter
    );
    sender.dodajTransakcje("Przelew - wyslano", senderBalanceBefore, senderBalanceAfter);

    // Obiekt Osoba � odbiorca
    Osoba recipient(
        recipientQuery.value("id").toString().toStdString(),
        std::to_string(recipientQuery.value("pin").toInt()),
        recipientQuery.value("password").toString().toStdString(),
        recipientQuery.value("first_name").toString().toStdString(),
        recipientQuery.value("last_name").toString().toStdString(),
        recipientQuery.value("birth_date").toString().toStdString(),
        recipientQuery.value("email").toString().toStdString(),
        recipientQuery.value("city").toString().toStdString(),
        recipientQuery.value("postal_code").toString().toStdString(),
        recipientQuery.value("street").toString().toStdString(),
        recipientQuery.value("house_number").toString().toStdString(),
        recipientBalanceAfter
    );
    recipient.dodajTransakcje("Przelew - otrzymano", recipientBalanceBefore, recipientBalanceAfter);

    QMessageBox::information(this, "Sukces", "Przelew wykonany pomyslnie!");

    if (main) {
        main->refreshBalance();
    }
}



void TransferWindow::on_BackButton_clicked()
{
    qDebug() << "Back button clicked";
    if (main) {
        main->show();
        close();
    }
    else
    {
        QMessageBox::critical(this, "Blad", "Glowne okno nie zostalo ustawione");
    }
}

void TransferWindow::setAccountNumber(std::string accNum)
{
    accountNumber = accNum;
}
