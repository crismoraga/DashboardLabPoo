#ifndef DASHBOARDAPP_H
#define DASHBOARDAPP_H

#include <QtCore/qglobal.h>

class DashboardApp : public QWidget {
public:
    DashboardApp(QWidget* parent = nullptr);

private:
    QMap<QString, Asignatura> asignaturasProgramadas;
    QMap<QString, QString> asignaturasDia; // Sala y bloque ocupados

    void crearNuevaPeticion();

    QStringList getAsignaturasDisponibles();

    bool esHorarioDisponible(const QString& bloque, const QString& sala);
};
#endif // DASHBOARDAPP_H
