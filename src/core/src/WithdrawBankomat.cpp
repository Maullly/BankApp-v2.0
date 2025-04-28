#include "../include/WithdrawBankomat.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "../include/Osoba.h"
#include "../include/BankomatLoggedIn.h"
WithdrawBankomatWindow::WithdrawBankomatWindow(QWidget* parent) {
    ui.setupUi(this);
    // Po³¹cz przyciski numeryczne z funkcj¹ dodawania cyfr
    connect(ui.ZeroButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);
    connect(ui.OneButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);
    connect(ui.TwoButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);
    connect(ui.ThreeButon, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);
    connect(ui.FourButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);
    connect(ui.FiveButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);
    connect(ui.SixButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);
    connect(ui.SevenButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);
    connect(ui.EightButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);
    connect(ui.NineButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onNumberClicked);

    // Po³¹cz kasowanie z odpowiedni¹ funkcj¹
    connect(ui.ClearButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onClearClicked);
    connect(ui.DeleteButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onBackspaceClicked);

    // Po³¹cz przycisk depozytu
    connect(ui.OkButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::onWithdrawClicked);

    connect(ui.BackButton, &QPushButton::clicked, this, &WithdrawBankomatWindow::on_BackButton_clicked);
}

WithdrawBankomatWindow::~WithdrawBankomatWindow()
{
}

void WithdrawBankomatWindow::setLog(BankomatLoggedIn* mainApp)
{
    main = mainApp;
}

void WithdrawBankomatWindow::setAccountNumber(std::string accNum)
{
    accountNumber = accNum;
}

void WithdrawBankomatWindow::onNumberClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString digit = button->text();

    // Ograniczenie do 6 cyfr (maksymalna kwota 999999)
    if (ui.WithdrawEdit->text().length() < 6) {
        ui.WithdrawEdit->setText(ui.WithdrawEdit->text() + digit);
    }
}

void WithdrawBankomatWindow::onBackspaceClicked()
{
    if (!ui.WithdrawEdit->text().isEmpty()) {
        ui.WithdrawEdit->setText(ui.WithdrawEdit->text().left(ui.WithdrawEdit->text().length() - 1));
    }
}

void WithdrawBankomatWindow::onClearClicked()
{
    ui.WithdrawEdit->clear();
}

void WithdrawBankomatWindow::onWithdrawClicked()
{
    double amount = ui.WithdrawEdit->text().toDouble();
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
    bool found = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 11 && parts[0] == accountNumber) {
            balanceBefore = parts[10].toDouble();
            if (balanceBefore < amount) {
                QMessageBox::warning(this, "Blad", "Niewystarczajace srodki na koncie!");
                file.close();
                return;
            }
            balanceAfter = balanceBefore - amount;
            parts[10] = QString::number(balanceAfter, 'f', 2);
            line = parts.join(",");
            found = true;
        }
        lines.append(line);
    }
    file.close();

    if (!found) {
        QMessageBox::critical(this, "Blad", "Nie udalo sie znalezc konta!");
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

    osoba.dodajTransakcje("Wyplata", balanceBefore, balanceAfter);
    QMessageBox::information(this, "Sukces", "Wyplata zakoñczona sukcesem!");
}

void WithdrawBankomatWindow::on_BackButton_clicked()
{
    if (main) {
        main->show();
        this->hide();
    }
    else {
        QMessageBox::critical(this, "Error", "Main window not set");
    }
}
