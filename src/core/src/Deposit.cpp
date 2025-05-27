#include "../include/Deposit.h"
#include "../include/LoggedInWindow.h"
#include "../include/Osoba.h"
#include <QMessageBox>
#include <QDebug>
#include <fstream>
#include <QTextStream>
#include <QFile>
DepositWindow::DepositWindow(QWidget* parent) : QWidget(parent), main(nullptr)
{
    ui.setupUi(this);
    connect(ui.BackButton, &QPushButton::clicked, this, &DepositWindow::on_BackButton_clicked);
    connect(ui.DepositButton, &QPushButton::clicked, this, &DepositWindow::on_DepositButton_clicked);

}

DepositWindow::~DepositWindow()
{
}
void DepositWindow::setLog(LoggedInWindow* mainWindow)
{
    main = mainWindow;
}

void DepositWindow::setAccountNumber(std::string accNum) {
    accountNumber = accNum;
}

void DepositWindow::on_DepositButton_clicked()
{
    double amount = ui.DepositEdit->text().toDouble();
    if (amount <= 0) {
        QMessageBox::warning(this, "B³¹d", "Kwota musi byæ wiêksza od zera!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", QString::fromStdString(accountNumber));
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "B³¹d", "Nie znaleziono konta!");
        return;
    }

    double balanceBefore = query.value("balance").toDouble();
    double balanceAfter = balanceBefore + amount;

    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE users SET balance = :balance WHERE id = :id");
    updateQuery.bindValue(":balance", balanceAfter);
    updateQuery.bindValue(":id", QString::fromStdString(accountNumber));
    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "B³¹d", "Nie uda³o siê zaktualizowaæ salda!");
        return;
    }

    // Utwórz obiekt Osoba
    Osoba osoba(
        query.value("id").toString().toStdString(),
        std::to_string(query.value("pin").toInt()),
        query.value("password").toString().toStdString(),
        query.value("first_name").toString().toStdString(),
        query.value("last_name").toString().toStdString(),
        query.value("birth_date").toString().toStdString(),
        query.value("email").toString().toStdString(),
        query.value("city").toString().toStdString(),
        query.value("postal_code").toString().toStdString(),
        query.value("street").toString().toStdString(),
        query.value("house_number").toString().toStdString(),
        balanceAfter
    );

    osoba.dodajTransakcje("Wplata", balanceBefore, balanceAfter);

    QMessageBox::information(this, "Sukces", "Wp³ata zakoñczona sukcesem!");

    if (main) {
        main->refreshBalance();
    }
}


void DepositWindow::on_BackButton_clicked()
{
    qDebug() << "Back button clicked";
    if (main) {
        main->show();
        close();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Main window not set");
    }
}
