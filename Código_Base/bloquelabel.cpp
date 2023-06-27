#include "bloquelabel.h"

BloqueLabel::    BloqueLabel(const QString& text, const QString& horario, QWidget* parent = nullptr)
    : QLabel(text, parent), horario(horario) {}

void BloqueLabel::setHorario(const QString& horario) {
    this->horario = horario;
}

void BloqueLabel::restoreText() {
    setText(originalText);
}

void BloqueLabel::enterEvent(QEvent*) override {
    originalText = text();
    setText(horario);
}

void BloqueLabel::leaveEvent(QEvent*) override {
    setText(originalText);
}
