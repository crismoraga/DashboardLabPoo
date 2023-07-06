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
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <iostream>
#include <QMenu>
#include <QEvent>
#include <QMouseEvent>
#include <QMenuBar>
#include <QAction>

using namespace std;

enum DiasSemana { Lunes, Martes, Miercoles, Jueves, Viernes, Sabado };

class BloqueLabel : public QLabel
{
public:
    BloqueLabel(const QString &text, const QString &horario, QWidget *parent = nullptr)
        : QLabel(text, parent), horario(horario)
    {}

    void setHorario(const QString &horario) { this->horario = horario; }

    void restoreText() { setText(originalText); }

protected:
    void enterEvent(QEvent *) override
    {
        originalText = text();
        setText(horario);
    }

    void leaveEvent(QEvent *) override { setText(originalText); }

private:
    QString horario;
    QString originalText;
};

class Asignatura
{
public:
    QString profesor;
    int cantidadAlumnos;
    QString departamento;
    QString sigla;
};

class DashboardApp : public QWidget
{
public:
    QComboBox* diaComboBox = new QComboBox(this);
    void actualizarVista()
    {
        diaComboBox->setCurrentIndex(static_cast<int>(diaSeleccionado));
        // Borrar todas las asignaturas cargadas previamente
        QGridLayout *gridLayout = qobject_cast<QGridLayout *>(layout());
        QLayoutItem *item;
        while ((item = gridLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }
    void crearWidgets() {
        QGridLayout* gridLayout = new QGridLayout(this);
        setLayout(gridLayout);

        // Crear botones de días de la semana
        QStringList diasSemanaLabels = {
                                        "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"
        };

        for (int i = 0; i < diasSemanaLabels.size(); ++i) {
            QPushButton* diaButton = new QPushButton(diasSemanaLabels[i], this);
            diaButton->setCheckable(true);
            diaButton->setChecked(i == 0); // Seleccionar el primer día por defecto
            connect(diaButton, &QPushButton::clicked, [this, i]() {
                diaSeleccionado = static_cast<DiasSemana>(i);
                actualizarVista();
            });
            gridLayout->addWidget(diaButton, 0, i + 3);
        }

        // Agregar etiquetas de encabezado
        QLabel* salaEncabezado = new QLabel("Sala", this);
        QLabel* bloqueEncabezado = new QLabel("Bloque", this);
        QLabel* horarioEncabezado = new QLabel("Horario", this);

        gridLayout->addWidget(salaEncabezado, 0, 0);
        gridLayout->addWidget(bloqueEncabezado, 0, 1);
        gridLayout->addWidget(horarioEncabezado, 0, 2);

        diaComboBox->addItems({ "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado" });
        diaComboBox->setCurrentIndex(0); // Seleccionar el primer día por defecto

        connect(diaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DashboardApp::cambiarDiaSeleccionado);

        connect(diaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
            diaSeleccionado = static_cast<DiasSemana>(index);
            actualizarVista();
        });

        gridLayout->addWidget(diaComboBox, 0, 2); // Agregar el combobox al layout

        // Cargar las asignaturas para el día seleccionado
        actualizarVista();
    }

    DashboardApp(QWidget *parent = nullptr) : QWidget(parent)
    {
        QGridLayout *gridLayout = new QGridLayout(this);
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

        QStringList salaLabels = {
            "Sala 1", "Sala 2", "Sala 3", "Sala 4", "Sala 5", "Sala 6",
            "Sala 7", "Sala 8", "Sala 9", "Sala 10", "Sala 11", "Sala 12"
        };

        for (int i = 0; i < salaLabels.size(); ++i) {
            QLabel* salaLabel = new QLabel(salaLabels[i], this);
            salaLabel->setStyleSheet("background-color: rgba(50, 150, 250, 0.2);");
            gridLayout->addWidget(salaLabel, 0, i + 1);
        }

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

        // Botón para crear nueva petición
        QPushButton* nuevaPeticionBtn = new QPushButton("Crear nueva petición", this);
        nuevaPeticionBtn->setStyleSheet(buttonStyle);
        connect(nuevaPeticionBtn, &QPushButton::clicked, this, &DashboardApp::crearNuevaPeticion);
        gridLayout->addWidget(nuevaPeticionBtn, bloqueLabels.size() + 1, 0, 1, salaLabels.size() + 1);

        // Cargar peticiones guardadas
        //cargarPeticiones();
        mostrarAsignaturasCargadas();

        QIcon icon("usmlogo.ico");
        setWindowIcon(icon);
    }

    ~DashboardApp() {
        // Guardar peticiones al cerrar la aplicación
        guardarPeticiones();
    }

private:
    QMap<QString, Asignatura> asignaturasProgramadas;
    QMap<QString, QString> asignaturasDia; // Sala y bloque ocupados
    DiasSemana diaSeleccionado;
private:
    void guardarPeticiones() {
        QFile file("peticiones.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            for (const QString& salaBloque : asignaturasDia.keys()) {
                QString asignatura = asignaturasDia.value(salaBloque);
                stream << salaBloque << ":" << asignatura << "\n";
            }
            file.close();
        }
    }

    void cargarPeticiones() {
        QFile file("peticiones.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            while (!stream.atEnd()) {
                QString line = stream.readLine();
                QStringList parts = line.split(":");
                if (parts.size() == 2) {
                    QString salaBloque = parts[0];
                    QString asignatura = parts[1];
                    asignaturasDia[salaBloque] = asignatura;
                }
            }
            file.close();
        }
    }

    void mostrarAsignaturasCargadas() {
        QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());
        int columnCount = gridLayout->columnCount();
        for (const QString& salaBloque : asignaturasDia.keys()) {
            QString asignatura = asignaturasDia.value(salaBloque);
            QStringList partes = salaBloque.split("-");
            if (partes.size() == 2) {
                int fila = partes[0].toInt();
                int columna = partes[1].toInt();
                QLabel* asignaturaLabel = new QLabel(asignatura, this);
                asignaturaLabel->setStyleSheet("background-color: rgba(50, 150, 250, 0.2);");
                if (columna == diaSeleccionado + 1) { // Mostrar solo para el día seleccionado
                    gridLayout->addWidget(asignaturaLabel, fila, columna);
                }
            }
        }
    }

    void crearNuevaPeticion() {
        QDialog dialog(this);
        QFormLayout formLayout(&dialog);

        QComboBox* bloqueComboBox = new QComboBox(&dialog);
        bloqueComboBox->addItems({
            "1-2", "3-4", "5-6", "7-8","Almuerzo (bloqueado)", "9-10", "11-12", "13-14", "15-16", "17-18"
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

                // Agregar la asignatura a la grilla
                QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());
                int fila = bloqueComboBox->currentIndex() + 1;
                int columna = salaComboBox->currentIndex() + 1;

                QLabel* asignaturaLabel = new QLabel(asignatura, this);
                asignaturaLabel->setStyleSheet("background-color: rgba(50, 150, 250, 0.2);");
                gridLayout->addWidget(asignaturaLabel, fila, columna);

                asignaturasProgramadas[asignatura] = nuevaAsignatura;
                asignaturasDia[sala + bloque] = asignatura;

                guardarPeticiones();

                QMessageBox::information(this, "Petición creada", "Petición agregada correctamente");
            } else {
                QMessageBox::warning(this, "Error", "La sala no está disponible en ese bloque");
            }

            dialog.accept();
        });

        formLayout.addRow(aceptarBtn);

        dialog.exec();
    }

    QStringList getAsignaturasDisponibles() {
        QStringList asignaturas;
        // Agregar asignaturas disponibles aquí
        asignaturas.append("Ayudantia MEC141");
        asignaturas.append("IQA371");
        asignaturas.append("Ayudantia IWI131");
        asignaturas.append("Ayudantia INF130");
        asignaturas.append("ELO329");
        asignaturas.append("MAT021");
        asignaturas.append("MAT270");
        asignaturas.append("ICN337");
        asignaturas.append("ICS024");
        asignaturas.append("MAT267");
        asignaturas.append("INF358");
        asignaturas.append("MEC270");
        asignaturas.append("IWI131");
        asignaturas.append("TEL312");
        asignaturas.append("ILI281");
        asignaturas.append("MAT071");
        asignaturas.append("CIV390");
        asignaturas.append("Vespertino comercial");
        asignaturas.append("ELO320");
        asignaturas.append("TEL101");
        return asignaturas;
    }

    bool esHorarioDisponible(const QString& bloque, const QString& sala) {
        QString key = sala + bloque;
        return !asignaturasDia.contains(key); // Verificar si el bloque y sala están ocupados
    }
public slots:
    void cambiarDiaSeleccionado(int index) {
        diaSeleccionado = static_cast<DiasSemana>(index);
        actualizarVista();
    }
};

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    DashboardApp dashboard;
    dashboard.setWindowTitle("Sistema de gestión de salas");
    dashboard.setMinimumSize(1100, 700);
    dashboard.show();

    return a.exec();
}
