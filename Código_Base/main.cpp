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
#include <QDebug>

class BloqueLabel : public QLabel {
public:
    BloqueLabel(const QString& text, const QString& horario, QWidget* parent = nullptr)
        : QLabel(text, parent), horario(horario) {}

    void setHorario(const QString& horario) {
        this->horario = horario;
    }

    void restoreText() {
        setText(originalText);
    }

protected:
    void enterEvent(QEvent*) override {
        originalText = text();
        setText(horario);
    }

    void leaveEvent(QEvent*) override {
        setText(originalText);
    }

private:
    QString horario;
    QString originalText;
};

class Asignatura {
public:
    QString profesor;
    int cantidadAlumnos;
    QString departamento;
    QString sigla;
};

class DashboardApp : public QWidget {
public:
    DashboardApp(QWidget* parent = nullptr) : QWidget(parent) {
        QGridLayout* gridLayout = new QGridLayout(this);
        QPalette palette;

        // Configuración de colores
        palette.setColor(QPalette::Window, Qt::white);
        palette.setColor(QPalette::Base, QColor(200, 200, 200));
        palette.setColor(QPalette::AlternateBase, QColor(220, 220, 220));
        palette.setColor(QPalette::Button, QColor(50, 150, 250));
        palette.setColor(QPalette::ButtonText, Qt::white);
        setPalette(palette);

        // Configuración de estilo para el botón
        QString buttonStyle = "background-color: rgb(50, 150, 250); color: white; padding: 5px 10px;";

        QStringList bloqueLabels = {
            "1-2", "3-4", "5-6", "7-8", "Almuerzo", "9-10", "11-12", "13-14", "15-16", "17-18"
        };

        QStringList horarioLabels = {
            "8:15 - 9:25", "9:35 - 10:45", "10:55 - 12:05", "12:15 - 13:25", "13:25 - 14:30",
            "14:30 - 15:40", "15:50 - 17:00", "17:10 - 18:20", "18:30 - 19:40"
        };

        for (int i = 0; i < bloqueLabels.size() && i < horarioLabels.size(); ++i) {
            BloqueLabel* bloqueLabel = new BloqueLabel(bloqueLabels[i], horarioLabels[i], this);
            bloqueLabel->setStyleSheet("background-color: rgba(50, 150, 250, 0.2);");
            gridLayout->addWidget(bloqueLabel, i + 1, 0);
        }

        QStringList salaLabels = {
            "Sala 1", "Sala 2", "Sala 3", "Sala 4", "Sala 5", "Sala 6",
            "Sala 7", "Sala 8", "Sala 9", "Sala 10", "Sala 11", "Sala 12"
        };

        for (int i = 0; i < salaLabels.size(); ++i) {
            QLabel* salaLabel = new QLabel(salaLabels[i], this);
            salaLabel->setStyleSheet("background-color: rgba(50, 150, 250, 0.2);");
            gridLayout->addWidget(salaLabel, 0, i + 1);
        }

        // Botón para crear nueva petición
        QPushButton* nuevaPeticionBtn = new QPushButton("Crear nueva petición", this);
                                        nuevaPeticionBtn->setStyleSheet(buttonStyle);
        connect(nuevaPeticionBtn, &QPushButton::clicked, this, &DashboardApp::crearNuevaPeticion);
        gridLayout->addWidget(nuevaPeticionBtn, bloqueLabels.size() + 1, 0, 1, salaLabels.size() + 1);
    }

private:
    QMap<QString, Asignatura> asignaturasProgramadas;
    QMap<QMap<QString, Asignatura>, QString> asignaturasDia;

    void crearNuevaPeticion() {
        QDialog dialog(this);
        QFormLayout formLayout(&dialog);

        QComboBox* bloqueComboBox = new QComboBox(&dialog);
        bloqueComboBox->addItems({
            "1-2", "3-4", "5-6", "7-8", "9-10", "11-12", "13-14", "15-16", "17-18"
        });

        QComboBox* asignaturaComboBox = new QComboBox(&dialog);
        asignaturaComboBox->addItems(getAsignaturasDisponibles());

        QComboBox* salaComboBox = new QComboBox(&dialog);
        salaComboBox->addItems({
            "Sala 1", "Sala 2", "Sala 3", "Sala 4", "Sala 5", "Sala 6",
            "Sala 7", "Sala 8", "Sala 9", "Sala 10", "Sala 11", "Sala 12"
        });

        formLayout.addRow("Bloque:", bloqueComboBox);
        formLayout.addRow("Asignatura:", asignaturaComboBox);
        formLayout.addRow("Número de sala:", salaComboBox);

            QPushButton* aceptarBtn = new QPushButton("Aceptar", &dialog);
        aceptarBtn->setStyleSheet("background-color: rgb(50, 150, 250); color: white; padding: 5px 10px;");
        connect(aceptarBtn, &QPushButton::clicked, [&]() {
            QString bloque = bloqueComboBox->currentText();
            QString asignatura = asignaturaComboBox->currentText();
            QString sala = salaComboBox->currentText();

            // Verificar disponibilidad de la sala y horario
            if (esHorarioDisponible(bloque, sala)) {
                Asignatura nuevaAsignatura;
                nuevaAsignatura.profesor = "Nombre del profesor";
                nuevaAsignatura.cantidadAlumnos = 50;
                nuevaAsignatura.departamento = "Departamento";
                nuevaAsignatura.sigla = asignatura;

                // Verificar si el horario ya está ocupado
                if (asignaturasDia.contains(sala)) {
                    QMap<QString, Asignatura> asignaturasEnDia = asignaturasDia.value(sala);
                    if (asignaturasEnDia.contains(bloque)) {
                        QMessageBox::warning(this, "Error", "El horario ya está ocupado en esa sala");
                            return;
                    }
                }

                asignaturasProgramadas.insert(asignatura, nuevaAsignatura);
                asignaturasDia[sala].insert(bloque, nuevaAsignatura);

                BloqueLabel* bloqueLabel = findChild<BloqueLabel*>(bloque);
                if (bloqueLabel) {
                    bloqueLabel->setStyleSheet("background-color: rgba(50, 150, 250, 0.6);");
                    bloqueLabel->setHorario(asignatura);
                    bloqueLabel->setToolTip(asignatura);
                }

                QMessageBox::information(this, "Éxito", "Asignatura programada exitosamente");
                    dialog.accept();
            } else {
                QMessageBox::warning(this, "Error", "La sala ya está ocupada en ese horario");
            }
        });
        formLayout.addRow(aceptarBtn);

        dialog.exec();
    }

    QStringList getAsignaturasDisponibles() const {
        // Simulación de obtención de asignaturas disponibles desde una fuente de datos
        return {"Matemáticas", "Física", "Historia", "Química", "Biología"};
    }

    bool esHorarioDisponible(const QString& bloque, const QString& sala) const {
        if (asignaturasDia.contains(sala)) {
            QMap<QString, Asignatura> asignaturasEnDia = asignaturasDia.value(sala);
            return !asignaturasEnDia.contains(bloque);
        }
        return true;
    }
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    DashboardApp dashboard;
    dashboard.show();

    return app.exec();
}
