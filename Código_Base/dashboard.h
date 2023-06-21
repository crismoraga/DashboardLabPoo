#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Dashboard; }
QT_END_NAMESPACE

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

private:
    Ui::Dashboard *ui;
};
#endif // DASHBOARD_H
