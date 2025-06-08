#include "../include/SignUpWindow.h"
#include "../include/BankApp.h"
#include "../include/Osoba.h"
#include <QMessageBox>
#include <QDebug>
#include <fstream>
SignUpWindow::SignUpWindow(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.PasswordEdit->setEchoMode(QLineEdit::Password);
	ui.PinEdit->setEchoMode(QLineEdit::Password);
	connect(ui.BackButton, &QPushButton::clicked, this, &SignUpWindow::on_BackButton_clicked);
	connect(ui.RegistrationButton, &QPushButton::clicked, this, &SignUpWindow::on_RegistrationButton_clicked);
}

SignUpWindow::~SignUpWindow()
{
}
void SignUpWindow::setLog(BankApp* mainApp)
{
	main = mainApp;
}
void SignUpWindow::on_RegistrationButton_clicked()
{

	QString password = ui.PasswordEdit->text().trimmed();
	QString pin = ui.PinEdit->text().trimmed();
	QString imie = ui.NameEdit->toPlainText();
	QString nazwisko = ui.SurnameEdit->toPlainText();
	QString dataUrodzenia = ui.dateEdit->text();
	QString email = ui.EmailEdit->toPlainText();
	QString miasto = ui.CityEdit->toPlainText();
	QString kodPocztowy = ui.ZipCodeEdit->toPlainText();
	QString ulica = ui.StreetEdit->toPlainText();
	QString numerDomu = ui.HouseNumberEdit->toPlainText();
	if (password.isEmpty() || pin.isEmpty() || imie.isEmpty() || nazwisko.isEmpty() || dataUrodzenia.isEmpty() || email.isEmpty() || miasto.isEmpty() || kodPocztowy.isEmpty() || ulica.isEmpty() || numerDomu.isEmpty()){
		QMessageBox::warning(this, "Blad", "Wszystkie pola musza byc wypelnione!");
		return;
	}
	QRegularExpression pinRegex("^[0-9]{1,4}$");
	if (!pinRegex.match(pin).hasMatch()) {
		QMessageBox::warning(this, "Blad", "PIN moze skladac sie maksymalnie z 4 cyfr!");
		return;
	}
	std::string noweId = Osoba::generujNoweId();
	if (noweId.empty()) {
		QMessageBox::critical(this, "Blad", "Nie mozna wygenerowac nowego numeru konta!");
		return;
	}
	Osoba nowaOsoba(noweId, password.toStdString(), pin.toStdString(), imie.toStdString(),
		nazwisko.toStdString(), dataUrodzenia.toStdString(), email.toStdString(),
		miasto.toStdString(), kodPocztowy.toStdString(), ulica.toStdString(),
		numerDomu.toStdString(), 0.0);
	if (nowaOsoba.dodajDoBazy()) {
		QMessageBox::information(this, "Sukces", "Rejestracja zakonczona pomyslnie!\nNumer konta: " + QString::fromStdString(noweId));
		if (main) {
			main->show();
			close();
		}
		else {
			QMessageBox::critical(this, "Blad", "Glowne okno nie zostalo ustawione");
		}
	}
	else {
		QMessageBox::critical(this, "Blad", "Nie udalo siê zapisac konta!");
	}
}	
void SignUpWindow::on_BackButton_clicked()
{
	if (main) {
		main->show();
		close();
	}
	else
	{
		QMessageBox::critical(this, "Blad", "Glowne okno nie zostalo ustawione");
	}
}

