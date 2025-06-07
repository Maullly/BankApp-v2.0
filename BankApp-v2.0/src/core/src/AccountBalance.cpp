#include "../include/AccountBalance.h"
#include "../include/LoggedInWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
AccountBalanceWindow::AccountBalanceWindow(QWidget* parent)
{
	ui.setupUi(this);
	connect(ui.BackButton, &QPushButton::clicked, this, &AccountBalanceWindow::on_BackButton_clicked);
}

AccountBalanceWindow::~AccountBalanceWindow()
{
}
void AccountBalanceWindow::setAccountNumber(std::string accNum)
{
    accountNumber = accNum;
    qDebug() << "Ustawiono numer konta:" << QString::fromStdString(accountNumber);
    ui.AccountNameLabel->setText("Numer konta: " + QString::fromStdString(accountNumber));

    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "B³¹d", "Brak po³¹czenia z baz¹ danych!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE id = :id");
    query.bindValue(":id", QString::fromStdString(accountNumber).trimmed());

    if (!query.exec()) {
        qDebug() << "B³¹d zapytania SQL:" << query.lastError().text();
        QMessageBox::critical(this, "B³¹d", "B³¹d zapytania SQL: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        double balance = query.value("balance").toDouble();
        qDebug() << "Saldo konta:" << balance;
        ui.BalanceLabel->setText("Stan konta: " + QString::number(balance, 'f', 2) + " PLN");
    }
    else {
        QMessageBox::warning(this, "B³¹d", "Nie znaleziono konta o podanym numerze.");
    }
}


void AccountBalanceWindow::setBalanceText(QString balance)
{
    ui.BalanceLabel->setText("Stan konta: " + balance + " PLN");
}


void AccountBalanceWindow::setLog(LoggedInWindow* mainWindow)
{
	main = mainWindow;
}

void AccountBalanceWindow::on_BackButton_clicked()
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
