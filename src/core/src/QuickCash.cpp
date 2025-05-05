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
void QuickCashWindow::processWithdrawal(double amount)
{
    if (amount <= 0) {
        QMessageBox::warning(this, "B³¹d", "Kwota musi byæ wiêksza od zera!");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "B³¹d", "Baza danych nie jest otwarta!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", QString::fromStdString(accountNumber));

    double balanceBefore = 0.0;
    double balanceAfter = 0.0;

    if (query.exec() && query.next()) {
        balanceBefore = query.value("balance").toDouble();

        if (balanceBefore < amount) {
            QMessageBox::warning(this, "B³¹d", "Niewystarczaj¹ce œrodki na koncie!");
            return;
        }

        balanceAfter = balanceBefore - amount;

        // Aktualizacja stanu konta
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE users SET balance = :balance WHERE id = :id");
        updateQuery.bindValue(":balance", balanceAfter);
        updateQuery.bindValue(":id", QString::fromStdString(accountNumber));
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "B³¹d", "Nie uda³o siê zaktualizowaæ stanu konta!");
            return;
        }

        // Stwórz obiekt Osoba
        Osoba osoba(
            query.value("id").toString().toStdString(),
            std::to_string(query.value("pin").toInt()),
            query.value("password").toString().toStdString(),
            query.value("first_name").toString().toStdString(),
            query.value("last_name").toString().toStdString(),
            query.value("birth_date").toString().toStdString(),
            query.value("email").toString().toStdString(),
            query.value("city").toString().toStdString(),
            query.value("postal_code").toString().toStdString(),
            query.value("street").toString().toStdString(),
            query.value("house_number").toString().toStdString(),
            balanceAfter // nowy stan konta
        );

        osoba.dodajTransakcje("Szybka wyp³ata", balanceBefore, balanceAfter);

        QMessageBox::information(this, "Sukces", "Wyp³ata zakoñczona sukcesem!");
    }
    else {
        QMessageBox::critical(this, "B³¹d", "Nie znaleziono u¿ytkownika!");
    }
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


