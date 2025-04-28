#include "../include/Transfer.h"
#include "../include/LoggedInWindow.h"
#include "../include/Osoba.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

TransferWindow::TransferWindow(QWidget* parent)
{
	ui.setupUi(this);
	connect(ui.BackButton, &QPushButton::clicked, this, &TransferWindow::on_BackButton_clicked);
	connect(ui.TransferButton, &QPushButton::clicked, this, &TransferWindow::on_TransferButton_clicked);
}

TransferWindow::~TransferWindow()
{
}

void TransferWindow::setLog(LoggedInWindow* mainWindow)
{
	main = mainWindow;
}

void TransferWindow::on_TransferButton_clicked()
{
    QString recipientAccount = ui.AccountNumberEdit->text();
    double amount = ui.TransferAmountEdit->text().toDouble();

    if (recipientAccount.isEmpty() || amount <= 0) {
        QMessageBox::warning(this, "Blad", "Wprowadz poprawny numer konta i kwote!");
        return;
    }

    QFile file("./src/login.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Blad", "Nie mozna otworzyc pliku!");
        return;
    }

    QStringList lines;
    QTextStream in(&file);

    QStringList senderData;
    QStringList recipientData;

    double senderBalanceBefore = 0, senderBalanceAfter = 0;
    double recipientBalanceBefore = 0, recipientBalanceAfter = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() >= 11) {
            if (parts[0].toStdString() == accountNumber) { // Nadawca przelewu
                senderData = parts;
                senderBalanceBefore = parts[10].toDouble();
                if (senderBalanceBefore < amount) {
                    QMessageBox::warning(this, "Blad", "Niewystarczajace srodki na koncie!");
                    file.close();
                    return;
                }
                senderBalanceAfter = senderBalanceBefore - amount;
                senderData[10] = QString::number(senderBalanceAfter, 'f', 2);
                line = senderData.join(",");
            }
            else if (parts[0] == recipientAccount) { // Odbiorca przelewu
                recipientData = parts;
                recipientBalanceBefore = parts[10].toDouble();
                recipientBalanceAfter = recipientBalanceBefore + amount;
                recipientData[10] = QString::number(recipientBalanceAfter, 'f', 2);
                line = recipientData.join(",");
            }
        }

        lines.append(line);
    }
    file.close();

    if (senderData.isEmpty()) {
        QMessageBox::critical(this, "Blad", "Twoje konto nie zostalo znalezione!");
        return;
    }

    if (recipientData.isEmpty()) {
        QMessageBox::critical(this, "Blad", "Konto odbiorcy nie istnieje!");
        return;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::critical(this, "Blad", "Nie mozna zapisac do pliku!");
        return;
    }

    QTextStream out(&file);
    for (const QString& line : lines) {
        out << line << "\n";
    }
    file.close();

    // Konwersja danych nadawcy na obiekt Osoba
    Osoba sender(senderData[0].toStdString(), senderData[1].toStdString(), senderData[2].toStdString(),
        senderData[3].toStdString(), senderData[4].toStdString(), senderData[5].toStdString(),
        senderData[6].toStdString(), senderData[7].toStdString(), senderData[8].toStdString(),
        senderData[9].toStdString(), senderData[10].toDouble());
    sender.dodajTransakcje("Przelew - wyslano", senderBalanceBefore, senderBalanceAfter);

    // Konwersja danych odbiorcy na obiekt Osoba
    Osoba recipient(recipientData[0].toStdString(), recipientData[1].toStdString(), recipientData[2].toStdString(),
        recipientData[3].toStdString(), recipientData[4].toStdString(), recipientData[5].toStdString(),
        recipientData[6].toStdString(), recipientData[7].toStdString(), recipientData[8].toStdString(),
        recipientData[9].toStdString(), recipientData[10].toDouble());
    recipient.dodajTransakcje("Przelew - otrzymano", recipientBalanceBefore, recipientBalanceAfter);

    QMessageBox::information(this, "Sukces", "Przelew wykonany pomyslnie!");

    // Aktualizacja salda w g³ównym oknie
    if (main) {
        main->refreshBalance();
    }
}


void TransferWindow::on_BackButton_clicked()
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

void TransferWindow::setAccountNumber(std::string accNum)
{
    accountNumber = accNum;
}
