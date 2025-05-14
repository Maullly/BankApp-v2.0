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
        QMessageBox::warning(this, "B³¹d", "Kwota musi byæ wiêksza od zera!");
        return;
    }

    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT * FROM users WHERE id = :id");
    selectQuery.bindValue(":id", QString::fromStdString(accountNumber));

    if (!selectQuery.exec() || !selectQuery.next()) {
        QMessageBox::critical(this, "B³¹d", "Nie znaleziono konta!");
        return;
    }

    double balanceBefore = selectQuery.value("balance").toDouble();
    if (balanceBefore < amount) {
        QMessageBox::warning(this, "B³¹d", "Niewystarczaj¹ce œrodki na koncie!");
        return;
    }

    double balanceAfter = balanceBefore - amount;

    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE users SET balance = :balance WHERE id = :id");
    updateQuery.bindValue(":balance", balanceAfter);
    updateQuery.bindValue(":id", QString::fromStdString(accountNumber));
    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "B³¹d", "Nie uda³o siê zaktualizowaæ salda!");
        return;
    }

    // Utwórz obiekt Osoba na podstawie danych z bazy
    Osoba osoba(
        selectQuery.value("id").toString().toStdString(),
        std::to_string(selectQuery.value("pin").toInt()),
        selectQuery.value("password").toString().toStdString(),
        selectQuery.value("first_name").toString().toStdString(),
        selectQuery.value("last_name").toString().toStdString(),
        selectQuery.value("birth_date").toString().toStdString(),
        selectQuery.value("email").toString().toStdString(),
        selectQuery.value("city").toString().toStdString(),
        selectQuery.value("postal_code").toString().toStdString(),
        selectQuery.value("street").toString().toStdString(),
        selectQuery.value("house_number").toString().toStdString(),
        balanceAfter
    );

    osoba.dodajTransakcje("Wyp³ata", balanceBefore, balanceAfter);
    QMessageBox::information(this, "Sukces", "Wyp³ata zakoñczona sukcesem!");
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
