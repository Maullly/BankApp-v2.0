#include "../include/Withdraw.h"
#include "../include/LoggedInWindow.h"
#include "../include/Osoba.h"
#include <QFile>
#include <QTextStream>

WithdrawWindow::WithdrawWindow(QWidget* parent) : QWidget(parent), main(nullptr)
{
    ui.setupUi(this);
    connect(ui.BackButton, &QPushButton::clicked, this, &WithdrawWindow::on_BackButton_clicked);
    connect(ui.WithdrawButton, &QPushButton::clicked, this, &WithdrawWindow::on_WithdrawButton_clicked);
}

WithdrawWindow::~WithdrawWindow()
{
}

void WithdrawWindow::setLog(LoggedInWindow* mainWindow)
{
    main = mainWindow;
}
void WithdrawWindow::setAccountNumber(std::string accNum) {
    accountNumber = accNum;
}

void WithdrawWindow::on_WithdrawButton_clicked()
{
    double amount = ui.WithdrawEdit->text().toDouble();
    if (amount <= 0) {
        QMessageBox::warning(this, "Blad", "Kwota musi byc wieksza od zera!");
        return;
    }

    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT * FROM users WHERE id = :id");
    selectQuery.bindValue(":id", QString::fromStdString(accountNumber));

    if (!selectQuery.exec() || !selectQuery.next()) {
        QMessageBox::critical(this, "B³¹d", "Nie znaleziono konta!");
        return;
    }

    double balanceBefore = selectQuery.value("balance").toDouble();
    QString imie = selectQuery.value("first_name").toString();
    QString nazwisko = selectQuery.value("last_name").toString();
    double balanceAfter = balanceBefore - amount;

    if (balanceAfter < 0) {
        QMessageBox::warning(this, "B³¹d", "Niewystarczaj¹ce œrodki!");
        return;
    }

    // Aktualizacja stanu konta
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE users SET balance = :balance WHERE id = :id");
    updateQuery.bindValue(":balance", balanceAfter);
    updateQuery.bindValue(":id", QString::fromStdString(accountNumber));

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "B³¹d", "Nie uda³o siê zaktualizowaæ salda!");
        return;
    }

    // Pe³ne utworzenie obiektu Osoba
    Osoba osoba(
        selectQuery.value("id").toString().toStdString(),
        std::to_string(selectQuery.value("pin").toInt()),
        selectQuery.value("password").toString().toStdString(),
        imie.toStdString(),
        nazwisko.toStdString(),
        selectQuery.value("birth_date").toString().toStdString(),
        selectQuery.value("email").toString().toStdString(),
        selectQuery.value("city").toString().toStdString(),
        selectQuery.value("postal_code").toString().toStdString(),
        selectQuery.value("street").toString().toStdString(),
        selectQuery.value("house_number").toString().toStdString(),
        balanceAfter
    );

    osoba.dodajTransakcje("Wyp³ata", balanceBefore, balanceAfter);


    QMessageBox::information(this, "Sukces", "Wyp³ata zakoñczona sukcesem!");
    if (main) {
        main->refreshBalance();
    }
}

void WithdrawWindow::on_BackButton_clicked()
{
    if (main) {
        main->show();
        this->hide();
    }
    else {
        QMessageBox::critical(this, "Error", "Main window not set");
    }
}
