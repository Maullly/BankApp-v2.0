#include "../include/BankomatLoggedIn.h"
#include "../include/DepositBankomat.h"
#include "../include/WithdrawBankomat.h"
#include "../include/QuickCash.h"
#include "../include/BankApp.h"
BankomatLoggedIn::BankomatLoggedIn(QWidget* parent)
{
	ui.setupUi(this);
}

BankomatLoggedIn::~BankomatLoggedIn()
{
}

void BankomatLoggedIn::on_DepositButton_clicked()
{
	qDebug() << "Deposit button clicked";
	if (!dep) {
		dep = new DepositBankomat();
		dep->setLog(this); // Przeka¿ wskaŸnik do tego okna
		dep->setAccountNumber(accountNumber);
	}
	dep->show(); // Poka¿ okno logowania
	this->hide(); // Ukryj okno logowania
}

void BankomatLoggedIn::on_WithdrawButton_clicked()
{
	qDebug() << "Deposit button clicked";
	if (!with) {
		with = new WithdrawBankomatWindow();
		with->setLog(this); // Przeka¿ wskaŸnik do tego okna
		with->setAccountNumber(accountNumber);
	}
	with->show(); // Poka¿ okno logowania
	this->hide(); // Ukryj okno logowania
}

void BankomatLoggedIn::on_QuickCashButton_clicked()
{
	qDebug() << "Fast Withdraw button clicked";
	if (!quick) {
		quick = new QuickCashWindow();
		quick->setLog(this); // Przeka¿ wskaŸnik do tego okna
		quick->setAccountNumber(accountNumber);
	}
	quick->show();
	this->hide();
	
}
void BankomatLoggedIn::on_ChangePinButton_clicked()
{
}

void BankomatLoggedIn::on_LogOutButton_clicked()
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

void BankomatLoggedIn::setLog(BankApp* mainApp)
{
	main = mainApp;
}

void BankomatLoggedIn::AccNumber(std::string accNum)
{
	accountNumber = accNum;
	ui.AccNumLabel->setText("Numer konta: " + QString::fromStdString(accNum));
}
