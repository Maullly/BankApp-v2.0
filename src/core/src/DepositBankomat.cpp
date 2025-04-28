#include "../include/DepositBankomat.h"
#include "../include/BankomatLoggedIn.h"
#include <QMessageBox>
#include "../include/Osoba.h"
#include <QFile>
#include <QTextStream>
DepositBankomat::DepositBankomat(QWidget* parent)
{
	ui.setupUi(this);
    // Po³¹cz przyciski numeryczne z funkcj¹ dodawania cyfr
    connect(ui.ZeroButton, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);
    connect(ui.OneButton, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);
    connect(ui.TwoButton, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);
    connect(ui.ThreeButon, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);
    connect(ui.FourButton, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);
    connect(ui.FiveButton, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);
    connect(ui.SixButton, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);
    connect(ui.SevenButton, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);
    connect(ui.EightButton, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);
    connect(ui.NineButton, &QPushButton::clicked, this, &DepositBankomat::onNumberClicked);

    // Po³¹cz kasowanie z odpowiedni¹ funkcj¹
    connect(ui.ClearButton, &QPushButton::clicked, this, &DepositBankomat::onClearClicked);
    connect(ui.DeleteButton, &QPushButton::clicked, this, &DepositBankomat::onBackspaceClicked);

    // Po³¹cz przycisk depozytu
    connect(ui.OkButton, &QPushButton::clicked, this, &DepositBankomat::onDepositClicked);

    connect(ui.BackButton, &QPushButton::clicked, this, &DepositBankomat::on_BackButtonClicked);
}

DepositBankomat::~DepositBankomat()
{
}

void DepositBankomat::setLog(BankomatLoggedIn* mainApp)
{
	main = mainApp;
}

void DepositBankomat::setAccountNumber(std::string accNum)
{
    accountNumber = accNum;
}

void DepositBankomat::onNumberClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString digit = button->text();

    // Nie pozwalamy na wpisanie wiêcej ni¿ 6 cyfr (maks. 999999)
    if (ui.DepositEdit->text().length() < 6) {
        ui.DepositEdit->setText(ui.DepositEdit->text() + digit);
    }
}

void DepositBankomat::onBackspaceClicked()
{
    if (!ui.DepositEdit->text().isEmpty()) {
        ui.DepositEdit->setText(ui.DepositEdit->text().left(ui.DepositEdit->text().length() - 1));
    }
}

void DepositBankomat::onClearClicked()
{
    ui.DepositEdit->clear();
}

void DepositBankomat::onDepositClicked()
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
        if (parts.size() >= 11 && parts[0] == accountNumber) {
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
        if (parts.size() >= 11 && parts[0] == accountNumber) {
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
        main->show();
        close();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Main window not set");
    }

    ui.DepositEdit->clear();
}
void DepositBankomat::on_BackButtonClicked()
{
    if (main) {
        main->show();
        this->hide();
    }
    else {
        QMessageBox::critical(this, "Error", "Main window not set");
    }
}
