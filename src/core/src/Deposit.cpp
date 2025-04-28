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
		QMessageBox::warning(this, "Blad", "Kwota musi byc wieksza od zera!");
		return;
	}
    QFile file("./src/login.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Blad", "Nie mozna otworzyc pliku!");
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    double balanceBefore = 0;
    double balanceAfter = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 11 && parts[0].toStdString() == accountNumber) {
            balanceBefore = parts[10].toDouble();
            balanceAfter = balanceBefore + amount;
            parts[10] = QString::number(balanceAfter, 'f', 2);
            line = parts.join(",");
        }
        lines.append(line);
    }
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::critical(this, "Blad", "Nie mozna zapisac do pliku!");
        return;
    }

    QTextStream out(&file);
    for (const QString& line : lines) {
        out << line << "\n";
    }
    file.close();
    QStringList userData;
    for (const QString& line : lines) {
        QStringList parts = line.split(",");
        if (parts.size() >= 11 && parts[0].toStdString() == accountNumber) {
            userData = parts;
            break;
        }
    }

    if (userData.isEmpty()) {
        QMessageBox::critical(this, "Blad", "Nie udalo sie znalezc danych uzytkownika!");
        return;
    }

    // Konwersja QString na std::string
    Osoba osoba(userData[0].toStdString(), userData[1].toStdString(), userData[2].toStdString(),
        userData[3].toStdString(), userData[4].toStdString(), userData[5].toStdString(),
        userData[6].toStdString(), userData[7].toStdString(), userData[8].toStdString(),
        userData[9].toStdString(), userData[10].toDouble());

    osoba.dodajTransakcje("Wplata", balanceBefore, balanceAfter);
    QMessageBox::information(this, "Sukces", "Wplata zakonczona sukcesem!");
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
