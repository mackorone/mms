#include "ColorDialog.h"

#include <QColor>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

#include "Color.h"

namespace mms {

ColorDialog::ColorDialog(QChar tileBaseColor, QChar tileWallColor,
                         QChar mouseBodyColor, QChar mouseWheelColor,
                         QChar tileWallIsSetColor,
                         unsigned char tileWallNotSetAlpha)
    : m_tileBaseColor(new QComboBox()),
      m_tileWallColor(new QComboBox()),
      m_mouseBodyColor(new QComboBox()),
      m_mouseWheelColor(new QComboBox()),
      m_tileWallIsSetColor(new QComboBox()),
      m_tileWallNotSetAlpha(new QSpinBox()) {
  // Configure the inputs
  auto map = CHAR_TO_COLOR();
  for (QComboBox *comboBox :
       {m_tileBaseColor, m_tileWallColor, m_mouseBodyColor, m_mouseWheelColor,
        m_tileWallIsSetColor}) {
    for (auto it = map.begin(); it != map.end(); it++) {
      RGB rgb = COLOR_TO_RGB().value(it.value());
      QPixmap pixmap(32, 32);
      pixmap.fill(QColor(rgb.r, rgb.g, rgb.b));
      comboBox->addItem(QIcon(pixmap), it.key());
    }
  }
  m_tileWallNotSetAlpha->setMaximum(255);

  // Set the current values
  m_tileBaseColor->setCurrentText(tileBaseColor);
  m_tileWallColor->setCurrentText(tileWallColor);
  m_mouseBodyColor->setCurrentText(mouseBodyColor);
  m_mouseWheelColor->setCurrentText(mouseWheelColor);
  m_tileWallIsSetColor->setCurrentText(tileWallIsSetColor);
  m_tileWallNotSetAlpha->setValue(tileWallNotSetAlpha);

  // Set the layout for the dialog
  QVBoxLayout *mainLayout = new QVBoxLayout();
  setLayout(mainLayout);

  // Add a groupbox
  QGroupBox *groupBox = new QGroupBox();
  QGridLayout *gridLayout = new QGridLayout();
  groupBox->setLayout(gridLayout);

  // Add a row for each field
  appendRow(gridLayout, "Maze Floor Color", m_tileBaseColor);
  appendRow(gridLayout, "Maze Wall Color", m_tileWallColor);
  appendRow(gridLayout, "Mouse Body Color", m_mouseBodyColor);
  appendRow(gridLayout, "Mouse Wheel Color", m_mouseWheelColor);
  appendRow(gridLayout, "setWall() Color", m_tileWallIsSetColor);
  appendRow(gridLayout, "clearWall() Alpha", m_tileWallNotSetAlpha);

  // Add OK and cancel buttons
  m_buttons =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

  // Add the group box and buttons to dialog
  mainLayout->addWidget(groupBox);
  mainLayout->addWidget(m_buttons);

  // Misc initialization
  setWindowTitle("Edit Colors");
}

QChar ColorDialog::getTileBaseColor() {
  return m_tileBaseColor->currentText().at(0);
}

QChar ColorDialog::getTileWallColor() {
  return m_tileWallColor->currentText().at(0);
}

QChar ColorDialog::getMouseBodyColor() {
  return m_mouseBodyColor->currentText().at(0);
}

QChar ColorDialog::getMouseWheelColor() {
  return m_mouseWheelColor->currentText().at(0);
}

QChar ColorDialog::getTileWallIsSetColor() {
  return m_tileWallIsSetColor->currentText().at(0);
}

unsigned char ColorDialog::getTileWallNotSetAlpha() {
  return m_tileWallNotSetAlpha->value();
}

void ColorDialog::appendRow(QGridLayout *layout, QString label,
                            QWidget *widget) {
  int row = layout->rowCount();
  QLabel *rowLabel = new QLabel(label);
  rowLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(rowLabel, row, 0);
  layout->addWidget(widget, row, 1);
}

}  // namespace mms
