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
	if (!bal) return; // Jeœli okno nie zosta³o jeszcze utworzone, nie rób nic

	QFile file("./src/login.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::critical(this, "B³¹d", "Nie mo¿na otworzyæ pliku!");
		return;
	}

	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		QStringList parts = line.split(",");
		if (parts.size() >= 11 && parts[0].toStdString() == accountNumber) {
			bal->setBalanceText(parts[10]);  // Nowa metoda do aktualizacji labelki
			break;
		}
	}
	file.close();
}

