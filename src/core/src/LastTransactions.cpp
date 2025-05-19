#include "../include/LastTransactions.h"
#include "../include/LoggedInWindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
LastTransactionsWindow::LastTransactionsWindow(QWidget* parent)
{
	ui.setupUi(this);
	connect(ui.BackButton, &QPushButton::clicked, this, &LastTransactionsWindow::on_BackButton_clicked);
    connect(ui.RefreshButton, &QPushButton::clicked, this, &LastTransactionsWindow::loadTransactions);
}

LastTransactionsWindow::~LastTransactionsWindow()
{

}

void LastTransactionsWindow::setLog(LoggedInWindow* mainWindow)
{
	main = mainWindow;
}

void LastTransactionsWindow::setAccountNumber(std::string accNum)
{
	accountNumber = accNum;
}

void LastTransactionsWindow::on_BackButton_clicked()
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

void LastTransactionsWindow::loadTransactions() {
    QSqlQuery query;
    query.prepare(R"(
        SELECT u.first_name, u.last_name, t.transaction_date, t.transaction_type,
               t.balance_before, t.balance_after
        FROM transactions t
        JOIN users u ON t.user_id = u.id
        WHERE t.account_number = :accountNumber
        ORDER BY t.transaction_date DESC
    )");

    query.bindValue(":accountNumber", QString::fromStdString(accountNumber));

    if (!query.exec()) {
        QMessageBox::critical(this, "B³¹d", "Nie uda³o siê wczytaæ transakcji!");
        return;
    }

    ui.TransactionsTable->setRowCount(0);

    while (query.next()) {
        int row = ui.TransactionsTable->rowCount();
        ui.TransactionsTable->insertRow(row);

        ui.TransactionsTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // imiê
        ui.TransactionsTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // nazwisko
        ui.TransactionsTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // data
        ui.TransactionsTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // typ
        ui.TransactionsTable->setItem(row, 4, new QTableWidgetItem(QString::number(query.value(4).toDouble(), 'f', 2))); // saldo przed
        ui.TransactionsTable->setItem(row, 5, new QTableWidgetItem(QString::number(query.value(5).toDouble(), 'f', 2))); // saldo po
    }
}
