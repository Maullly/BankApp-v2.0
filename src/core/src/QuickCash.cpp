#include "../include/QuickCash.h"
#include "../include/LoggedInWindow.h"
#include "../include/BankomatLoggedIn.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "../include/Osoba.h"
QuickCashWindow::QuickCashWindow(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.FirstQuickCashButton, &QPushButton::clicked, this, [this]() { processWithdrawal(50); });
	connect(ui.SecondQuickCashButton, &QPushButton::clicked, this, [this]() { processWithdrawal(100); });
	connect(ui.ThirdQuickCashButton, &QPushButton::clicked, this, [this]() { processWithdrawal(200); });
	connect(ui.FourthQuickCashButton, &QPushButton::clicked, this, [this]() { processWithdrawal(500); });
	connect(ui.FifthQuickCashButton, &QPushButton::clicked, this, [this]() { processWithdrawal(1000); });
	connect(ui.SixthQuickCashButton, &QPushButton::clicked, this, [this]() { processWithdrawal(2000); });
	connect(ui.BackButton, &QPushButton::clicked, this, &QuickCashWindow::on_BackButton_clicked);
}

QuickCashWindow::~QuickCashWindow()
{
}
void QuickCashWindow::setLog(BankomatLoggedIn* mainWindow)
{
	main = mainWindow;
}
void QuickCashWindow::setAccountNumber(std::string accNum)
{
	accountNumber = accNum;
}
void QuickCashWindow::processWithdrawal(double withdrawalamount)
{
    double amount = withdrawalamount;
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
    double balanceBefore = 0;
    double balanceAfter = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 11 && parts[0].toStdString() == accountNumber) {
            balanceBefore = parts[10].toDouble();
            if (balanceBefore < amount) {
                QMessageBox::warning(this, "Blad", "Niewystarczajace srodki na koncie!");
                file.close();
                return;
            }
            balanceAfter = balanceBefore - amount;
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

    osoba.dodajTransakcje("Szybka Wyplata", balanceBefore, balanceAfter);
    QMessageBox::information(this, "Sukces", "Wyplata zakonczona sukcesem!");
}

void QuickCashWindow::on_BackButton_clicked()
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


