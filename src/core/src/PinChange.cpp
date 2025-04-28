#include "../include/PinChange.h"
#include "../include/LoggedInWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
PinChangeWindow::PinChangeWindow(QWidget* parent)
{
	ui.setupUi(this);
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
    // Pobierz wartoœci z pól NewPinEdit i ConfirmPinEdit
    QString newPin = ui.NewPinEdit->text();
    QString confirmPin = ui.ConfirmPinEdit->text();

    // SprawdŸ, czy pola nie s¹ puste
    if (newPin.isEmpty() || confirmPin.isEmpty()) {
        QMessageBox::warning(this, "Blad", "Wszystkie pola musza byc wypelnione.");
        return;
    }

    // SprawdŸ, czy nowe PIN-y s¹ zgodne
    if (newPin != confirmPin) {
        QMessageBox::warning(this, "Blad", "Podane PIN-y nie sa zgodne.");
        return;
    }

    // SprawdŸ d³ugoœæ PIN-u (np. musi mieæ 4 cyfry)
    if (newPin.length() != 4 || newPin.toStdString().find_first_not_of("0123456789") != std::string::npos) {
        QMessageBox::warning(this, "Blad", "PIN musi sk³adaæ siê z 4 cyfr.");
        return;
    }

    QFile file("./src/login.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Blad", "Nie mozna otworzyc pliku!");
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    QStringList userData;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 11 && parts[0].toStdString() == accountNumber) {
            parts[1] = newPin; // Aktualizacja PIN-u
            line = parts.join(",");
            userData = parts;
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

    if (userData.isEmpty()) {
        QMessageBox::critical(this, "Blad", "Nie udalo sie znalezc danych uzytkownika!");
        return;
    }

    // Konwersja QString na std::string i utworzenie obiektu Osoba
    Osoba osoba(userData[0].toStdString(), userData[1].toStdString(), userData[2].toStdString(),
        userData[3].toStdString(), userData[4].toStdString(), userData[5].toStdString(),
        userData[6].toStdString(), userData[7].toStdString(), userData[8].toStdString(),
        userData[9].toStdString(), userData[10].toDouble());

    // Zapisanie zmienionego PIN-u
    osoba.setPin(newPin.toStdString());

    // Wyœwietl komunikat o sukcesie
    QMessageBox::information(this, "Sukces", "PIN zostal pomyslnie zmieniony.");

    // Wyczyœæ pola tekstowe
    ui.NewPinEdit->clear();
    ui.ConfirmPinEdit->clear();
}
void PinChangeWindow::setAccountNumber(std::string accNum)
{
    accountNumber = accNum;
}
