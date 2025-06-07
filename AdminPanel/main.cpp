#include <QApplication>
#include "AdminPanel.h" !

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    AdminPanel panel;
    panel.show();
    return app.exec();
}
