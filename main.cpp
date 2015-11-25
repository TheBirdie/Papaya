#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton b;
    b.setText("Boulibah");
    b.show();

    return app.exec();
}
