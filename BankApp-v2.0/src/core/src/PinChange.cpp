#include "../include/PinChange.h"
#include "../include/LoggedInWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
PinChangeWindow::PinChangeWindow(QWidget* parent)
{
	ui.setupUi(this);
    ui.NewPinEdit->setEchoMode(QLineEdit::Password);
    ui.ConfirmPinEdit->setEchoMode(QLineEdit::Password);
	connect(ui.BackButton, &QPushButton::clicked, this, &PinChangeWindow::on_BackButton_clicked);
	connect(ui.ChangePinButton, &QPushButton::clicked, this, &PinChangeWindow::on_ChangePinButton_clicked);
}

PinChangeWindow::~PinChangeWindow()
{
}

void PinChangeWindow::setLog(LoggedInWindow* mainWindow)
{
	main = mainWindow;
}

void PinChangeWindow::on_BackButton_clicked()
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

void PinChangeWindow::on_ChangePinButton_clicked() {
    QString newPin = ui.NewPinEdit->text();
    QString confirmPin = ui.ConfirmPinEdit->text();

    if (newPin.isEmpty() || confirmPin.isEmpty()) {
        QMessageBox::warning(this, "Blad", "Wszystkie pola musza byc wypelnione.");
        return;
    }

    if (newPin != confirmPin) {
        QMessageBox::warning(this, "Blad", "Podane PIN-y nie sa zgodne.");
        return;
    }

    if (newPin.length() != 4 || newPin.toStdString().find_first_not_of("0123456789") != std::string::npos) {
        QMessageBox::warning(this, "Blad", "PIN musi sk³adac sie z 4 cyfr.");
        return;
    }

    // Pobranie danych u¿ytkownika z bazy
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT * FROM users WHERE id = :id");
    selectQuery.bindValue(":id", QString::fromStdString(accountNumber));

    if (!selectQuery.exec() || !selectQuery.next()) {
        QMessageBox::critical(this, "Blad", "Nie udalo sie znalezc uzytkownika.");
        return;
    }

    // Aktualizacja PIN-u w bazie danych
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE users SET pin = :pin WHERE id = :id");
    updateQuery.bindValue(":pin", newPin.toInt());
    updateQuery.bindValue(":id", QString::fromStdString(accountNumber));

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "Blad", "Nie udalo sie zaktualizowac PIN-u.");
        return;
    }

    // Utworzenie obiektu Osoba z aktualnymi danymi
    Osoba osoba(
        selectQuery.value("id").toString().toStdString(),
        newPin.toStdString(), // Ustawiamy nowy PIN
        selectQuery.value("password").toString().toStdString(),
        selectQuery.value("first_name").toString().toStdString(),
        selectQuery.value("last_name").toString().toStdString(),
        selectQuery.value("birth_date").toString().toStdString(),
        selectQuery.value("email").toString().toStdString(),
        selectQuery.value("city").toString().toStdString(),
        selectQuery.value("postal_code").toString().toStdString(),
        selectQuery.value("street").toString().toStdString(),
        selectQuery.value("house_number").toString().toStdString(),
        selectQuery.value("balance").toDouble()
    );

    osoba.setPin(newPin.toStdString());

    QMessageBox::information(this, "Sukces", "PIN zostal pomyslnie zmieniony.");

    ui.NewPinEdit->clear();
    ui.ConfirmPinEdit->clear();
}

void PinChangeWindow::setAccountNumber(std::string accNum)
{
    accountNumber = accNum;
}
