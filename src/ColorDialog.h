#pragma once

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QSpinBox>
#include <QString>

namespace mms {

class ColorDialog : public QDialog {
  Q_OBJECT

 public:
  ColorDialog(QChar tileBaseColor, QChar tileWallColor, QChar mouseBodyColor,
              QChar mouseWheelColor, QChar tileWallIsSetColor,
              unsigned char tileWallNotSetAlpha);

  QChar getTileBaseColor();
  QChar getTileWallColor();
  QChar getMouseBodyColor();
  QChar getMouseWheelColor();
  QChar getTileWallIsSetColor();
  unsigned char getTileWallNotSetAlpha();

 private:
  QComboBox *m_tileBaseColor;
  QComboBox *m_tileWallColor;
  QComboBox *m_mouseBodyColor;
  QComboBox *m_mouseWheelColor;
  QComboBox *m_tileWallIsSetColor;
  QSpinBox *m_tileWallNotSetAlpha;

  QDialogButtonBox *m_buttons;

  void appendRow(QGridLayout *layout, QString label, QWidget *widget);
};

}  // namespace mms
