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
    if (balanceBefore < amount) {
        QMessageBox::warning(this, "B³¹d", "Niewystarczaj¹ce œrodki na koncie!");
        return;
    }

    double balanceAfter = balanceBefore - amount;

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
        query.value("password").toString().toStdString(),
        query.value("pin").toString().toStdString(),
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
