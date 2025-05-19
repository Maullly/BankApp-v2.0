#include "../include/LoggedInWindow.h"
#include "../include/Deposit.h"
#include "../include/Withdraw.h"
#include "../include/QuickCash.h"
#include "../include/LastTransactions.h"
#include "../include/AccountBalance.h"
#include "../include/PinChange.h"
#include "../include/BankApp.h"
#include "../include/Transfer.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
LoggedInWindow::LoggedInWindow(QWidget* parent)
{
	ui.setupUi(this);
}

LoggedInWindow::~LoggedInWindow()
{
}
void LoggedInWindow::setLog(BankApp* mainApp)
{
	main = mainApp;
}

void LoggedInWindow::AccNumber(std::string accNum)
{
	accountNumber = accNum;
	ui.AccNumLabel->setText("Numer konta: " + QString::fromStdString(accNum));
}

void LoggedInWindow::on_DepositButton_clicked()
{
	qDebug() << "Deposit button clicked";
	if (!dep) {
		dep = new DepositWindow();
		dep->setLog(this); // Przeka¿ wskaŸnik do tego okna
		dep->setAccountNumber(accountNumber);
	}
	dep->show(); // Poka¿ okno logowania
	this->hide(); // Ukryj okno logowania
	
}
void LoggedInWindow::on_WithdrawButton_clicked()
{
	qDebug() << "Withdraw button clicked";
	if (!with) {
		with = new WithdrawWindow();
		with->setLog(this); // Przeka¿ wskaŸnik do tego okna
		with->setAccountNumber(accountNumber);
	}
	with->show(); // Poka¿ okno logowania
	this->hide(); // Ukryj okno logowania

}

void LoggedInWindow::on_TransactionsButton_clicked()
{
	qDebug() << "Transactions button clicked";
	if (!last) {
		last = new LastTransactionsWindow();
		last->setLog(this); // Przeka¿ wskaŸnik do tego okna
		last->setAccountNumber(accountNumber);
	}
	last->show();
	this->hide();
}

void LoggedInWindow::on_AccountBalanceButton_clicked()
{
	qDebug() << "Account Balance button clicked";
	if (!bal) {
		bal = new AccountBalanceWindow();
		bal->setLog(this); // Przeka¿ wskaŸnik do tego okna
		bal->setAccountNumber(accountNumber);
	}
	bal->show();
	this->hide();
}

void LoggedInWindow::on_ChangePinButton_clicked()
{
	qDebug() << "Change Pin button clicked";
	if (!pin) {
		pin = new PinChangeWindow();
		pin->setLog(this); // Przeka¿ wskaŸnik do tego okna
		pin->setAccountNumber(accountNumber);
	}
	pin->show();
	this->hide();
}

void LoggedInWindow::on_TransferButton_clicked()
{
	qDebug() << "Transfer button clicked";
	if (!trans) {
		trans = new TransferWindow();
		trans->setLog(this);
		trans->setAccountNumber(accountNumber);
	}
	trans->show();
	this->hide();
}

void LoggedInWindow::on_LogOutButton_clicked()
{
	qDebug() << "Log Out button clicked";
	if (main) {
		main->show();
		close();
	}
	else
	{
		QMessageBox::critical(this, "Error", "Main window not set");
	}
}
void LoggedInWindow::refreshBalance()
{
	if (!bal) return;

	QSqlQuery query;
	query.prepare("SELECT balance FROM users WHERE id = :id");
	query.bindValue(":id", QString::fromStdString(accountNumber));

	if (!query.exec()) {
		QMessageBox::critical(this, "B³¹d", "B³¹d zapytania SQL: " + query.lastError().text());
		return;
	}

	if (query.next()) {
		double balance = query.value("balance").toDouble();
		bal->setBalanceText(QString::number(balance, 'f', 2));
	}
	else {
		QMessageBox::warning(this, "B³¹d", "Nie znaleziono konta.");
	}
}

