#include "../include/AccountBalance.h"
#include "../include/LoggedInWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>

AccountBalanceWindow::AccountBalanceWindow(QWidget* parent)
{
	ui.setupUi(this);
	connect(ui.BackButton, &QPushButton::clicked, this, &AccountBalanceWindow::on_BackButton_clicked);
}

AccountBalanceWindow::~AccountBalanceWindow()
{
}
void AccountBalanceWindow::setAccountNumber(std::string accNum)
{
    accountNumber = accNum;
    ui.AccountNameLabel->setText("Numer konta: " + QString::fromStdString(accountNumber));

    // Odczyt salda konta z login.txt
    QFile file("./src/login.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "B³¹d", "Nie mo¿na otworzyæ pliku!");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 11 && parts[0].toStdString() == accountNumber) {
            ui.BalanceLabel->setText("Stan konta: " + parts[10] + " PLN");
        }
    }
    file.close();
}
void AccountBalanceWindow::setBalanceText(QString balance)
{
    ui.BalanceLabel->setText("Stan konta: " + balance + " PLN");
}


void AccountBalanceWindow::setLog(LoggedInWindow* mainWindow)
{
	main = mainWindow;
}

void AccountBalanceWindow::on_BackButton_clicked()
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
