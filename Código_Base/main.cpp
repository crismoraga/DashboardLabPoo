#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QMessageBox>
#include <QPalette>
#include <QGridLayout>
#include <QComboBox>
#include <QIcon>



int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    DashboardApp dashboard;
    dashboard.setWindowTitle("Sistema de gestión de salas");
    dashboard.setMinimumSize(1100, 700);
    dashboard.show();

    return a.exec();
}
