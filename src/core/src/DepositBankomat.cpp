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
        QMessageBox::warning(this, "B³¹d", "Kwota musi byæ wiêksza od zera!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", QString::fromStdString(accountNumber));
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "B³¹d", "Nie znaleziono konta!");
        return;
    }

    double balanceBefore = query.value("balance").toDouble();
    double balanceAfter = balanceBefore + amount;

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
        balanceAfter
    );

    osoba.dodajTransakcje("Wp³ata", balanceBefore, balanceAfter);
    QMessageBox::information(this, "Sukces", "Wp³ata zakoñczona sukcesem!");

    if (main) {
        main->show();
        close();
    }
    else {
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
