#ifndef BLOQUELABEL_H
#define BLOQUELABEL_H

#include <QtCore/qglobal.h>

class BloqueLabel : public QLabel {
public:
    BloqueLabel(const QString& text, const QString& horario, QWidget* parent = nullptr);

    void setHorario(const QString& horario);

    void restoreText();

protected:
    void enterEvent(QEvent*) override;

    void leaveEvent(QEvent*) override;

private:
    QString horario;
    QString originalText;
};



#endif // BLOQUELABEL_H
