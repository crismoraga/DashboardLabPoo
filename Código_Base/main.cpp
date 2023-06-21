#include "dashboard.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QTime>
#include <QFormLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


class Asignatura {
public:
    QString profesor;
    int cantidadAlumnos;
    QString departamento;
    QString sigla;
};

class DashboardApp : public QWidget {
public:
    DashboardApp(QWidget *parent = nullptr) : QWidget(parent) {
        QGridLayout *gridLayout = new QGridLayout(this);
        for (int i = 0; i < 12; ++i) {
            QString titulo = "Sala " + QString::number(i + 1);
            gridLayout->addWidget(new QLabel(titulo, this), 0, i + 1);
        }

        for (int i = 0; i < 28; ++i) {
            QString hora = QTime(8, 15).addSecs(i * 60 * 10).toString("hh:mm");
            gridLayout->addWidget(new QLabel(hora, this), i + 1, 0);
        }

        //podriamos cambiar las horas por bloques

        // Botón para crear nueva petición
        QPushButton *nuevaPeticionBtn = new QPushButton("Crear nueva petición", this);
        connect(nuevaPeticionBtn, &QPushButton::clicked, this, &DashboardApp::crearNuevaPeticion);
        gridLayout->addWidget(nuevaPeticionBtn, 30, 0);
    }
private:
     QMap<QString, Asignatura> asignaturasProgramadas;
    QMap<QMap<QString, Asignatura>, QString> asignaturasDia;

private slots:
    void crearNuevaPeticion() {
        QDialog dialog(this);
        QFormLayout formLayout(&dialog);

        QLineEdit *horarioLineEdit = new QLineEdit(&dialog);
        QLineEdit *asignaturaLineEdit = new QLineEdit(&dialog);
        QLineEdit *salaLineEdit = new QLineEdit(&dialog);

        formLayout.addRow("Horario:", horarioLineEdit);
        formLayout.addRow("Asignatura:", asignaturaLineEdit);
        formLayout.addRow("Número de sala:", salaLineEdit);

        QPushButton *aceptarBtn = new QPushButton("Aceptar", &dialog);
        connect(aceptarBtn, &QPushButton::clicked, [&]() {
            QString horario = horarioLineEdit->text();
            QString asignatura = asignaturaLineEdit->text();
            int numeroSala = salaLineEdit->text().toInt();

            // Verificar disponibilidad de la sala y horario
            if (esHorarioDisponible(horario, numeroSala)) {
                Asignatura nuevaAsignatura;
                nuevaAsignatura.profesor = "Nombre del profesor";
                nuevaAsignatura.cantidadAlumnos = 50;
                nuevaAsignatura.departamento = "Departamento";
                nuevaAsignatura.sigla = asignatura;

                // Agregar la asignatura a la grilla
                QMessageBox::information(this, "Petición creada", "Petición agregada correctamente");
            } else {
                QMessageBox::warning(this, "Error", "La sala no está disponible en ese horario");
            }

            dialog.accept();
        });

        formLayout.addRow(aceptarBtn);

        dialog.exec();
    }

    bool esHorarioDisponible(const QString& horario, int numeroSala) {

        QString clave = QString::number(numeroSala) + horario;
        if (asignaturasProgramadas.contains(clave)) {
            return false;  // Sala ocupada en el horario especificado
        }

        return true;  // Sala disponible en el horario especificado
    }
};

DashboardApp dashboard;
dashboard.show();

    return a.exec();
}
