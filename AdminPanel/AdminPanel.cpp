#include "AdminPanel.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include "../BankApp-v2.0/src/core/include/BankApp.h"
#include <QDir>
AdminPanel::AdminPanel(QWidget* parent)
{
    ui.setupUi(this);
    loadAccounts(); // Wczytaj konta przy uruchomieniu panelu
    connect(ui.ChangePinAdminButton, &QPushButton::clicked, this, &AdminPanel::changePin);
    connect(ui.ChangePasswordAdminButton, &QPushButton::clicked, this, &AdminPanel::changePassword);
    connect(ui.LogOutAdminButton, &QPushButton::clicked, this, &AdminPanel::on_LogOutButton_clicked);
    connect(ui.AcceptAccAdminButton, &QPushButton::clicked, this, &AdminPanel::acceptAccount);

}

AdminPanel::~AdminPanel()
{
}

void AdminPanel::loadAccounts()
{
    ui.AdminWidget->setRowCount(0); // Wyczyœæ tabelê
    QFile file("./src/login.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "B³¹d", "Nie mo¿na otworzyæ pliku login.txt!");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() >= 6) { // Upewniamy siê, ¿e mamy wystarczaj¹ce dane
            int row = ui.AdminWidget->rowCount();
            ui.AdminWidget->insertRow(row);

            ui.AdminWidget->setItem(row, 0, new QTableWidgetItem(parts[0])); // Numer Konta
            ui.AdminWidget->setItem(row, 1, new QTableWidgetItem(parts[1])); // Numer Pin
            ui.AdminWidget->setItem(row, 2, new QTableWidgetItem(parts[2])); // Imiê
            ui.AdminWidget->setItem(row, 3, new QTableWidgetItem(parts[3])); // Nazwisko
            ui.AdminWidget->setItem(row, 4, new QTableWidgetItem(parts[4])); // Data Urodzenia
            ui.AdminWidget->setItem(row, 5, new QTableWidgetItem(parts[5])); // Adres Email
        }
    }
    file.close();
}

void AdminPanel::deleteAccount()
{
    int row = ui.AdminWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Blad", "Wybierz konto do usuniecia!");
        return;
    }

    QString accountNumber = ui.AdminWidget->item(row, 0)->text();

    QFile file("./src/login.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Blad", "Nie mozna otworzyc pliku login.txt!");
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.startsWith(accountNumber + ",")) {
            lines.append(line);
        }
    }
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::critical(this, "Blad", "Nie mozna zapisac do pliku login.txt!");
        return;
    }

    QTextStream out(&file);
    for (const QString& line : lines) {
        out << line << "\n";
    }
    file.close();

    ui.AdminWidget->removeRow(row);
    QMessageBox::information(this, "Sukces", "Konto zostalo usuniete!");
}

void AdminPanel::changePin()
{
    int row = ui.AdminWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Blad", "Wybierz konto do zmiany PIN-u!");
        return;
    }

    QString accountNumber = ui.AdminWidget->item(row, 0)->text();
    QString newPin = QInputDialog::getText(this, "Zmiana PIN", "Wprowadz nowy PIN:");

    if (newPin.isEmpty()) {
        QMessageBox::warning(this, "Blad", "PIN nie moze byæ pusty!");
        return;
    }
    QRegularExpression pinRegex("^[0-9]{4}$");
    if (!pinRegex.match(newPin).hasMatch()) {
        QMessageBox::warning(this, "B³¹d", "PIN musi sk³adaæ siê dok³adnie z 4 cyfr!");
        return;
    }

    QFile file("./src/login.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Blad", "Nie mozna otworzyc pliku login.txt!");
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 2 && parts[0] == accountNumber) {
            parts[1] = newPin; // Aktualizacja PIN-u
            line = parts.join(",");
        }
        lines.append(line);
    }
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::critical(this, "Blad", "Nie mozna zapisac do pliku login.txt!");
        return;
    }

    QTextStream out(&file);
    for (const QString& line : lines) {
        out << line << "\n";
    }
    file.close();

    ui.AdminWidget->item(row, 1)->setText(newPin);
    QMessageBox::information(this, "Sukces", "PIN zostal zmieniony!");
}

void AdminPanel::setLog(BankApp* mainApp)
{
    main = mainApp;
}

void AdminPanel::on_LogOutButton_clicked()
{
    // Uruchom z powrotem BankApp.exe
    QString bankAppExePath = QDir(QCoreApplication::applicationDirPath()).filePath("BankApp.exe");

    if (QFile::exists(bankAppExePath)) {
        QProcess::startDetached(bankAppExePath);
    }
    else {
        QMessageBox::warning(this, "B³¹d", "Nie znaleziono BankApp.exe!");
    }

    close(); // Zamknij AdminPanel
}

void AdminPanel::changePassword()
{
    int row = ui.AdminWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "B³¹d", "Wybierz konto do zmiany has³a!");
        return;
    }

    QString accountNumber = ui.AdminWidget->item(row, 0)->text();
    QString newPassword = QInputDialog::getText(this, "Zmiana has³a", "WprowadŸ nowe has³o:");

    if (newPassword.isEmpty()) {
        QMessageBox::warning(this, "B³¹d", "Has³o nie mo¿e byæ puste!");
        return;
    }

    QFile file("./src/login.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "B³¹d", "Nie mo¿na otworzyæ pliku login.txt!");
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 3 && parts[0] == accountNumber) {
            parts[6] = newPassword; // Za³ó¿my, ¿e has³o jest w kolumnie 6
            line = parts.join(",");
        }
        lines.append(line);
    }
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::critical(this, "B³¹d", "Nie mo¿na zapisaæ do pliku login.txt!");
        return;
    }

    QTextStream out(&file);
    for (const QString& line : lines)
        out << line << "\n";
    file.close();

    QMessageBox::information(this, "Sukces", "Has³o zosta³o zmienione!");
}
void AdminPanel::acceptAccount()
{
    int row = ui.AdminWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "B³¹d", "Wybierz konto do zatwierdzenia!");
        return;
    }

    QString accountNumber = ui.AdminWidget->item(row, 0)->text();

    QFile file("./src/login.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "B³¹d", "Nie mo¿na otworzyæ pliku login.txt!");
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 7 && parts[0] == accountNumber) {
            parts[7] = "true";  // Pole np. "zatwierdzony" na pozycji 7
            line = parts.join(",");
        }
        lines.append(line);
    }
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::critical(this, "B³¹d", "Nie mo¿na zapisaæ do pliku login.txt!");
        return;
    }

    QTextStream out(&file);
    for (const QString& line : lines)
        out << line << "\n";
    file.close();

    QMessageBox::information(this, "Zatwierdzono", "Konto zosta³o zatwierdzone.");
}
