#include "ConfigDialog.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include "Assert.h"
#include "ContainerUtilities.h"

namespace mms {

ConfigDialog::ConfigDialog(
    const QString& action,
    const QString& object,
    const QVector<ConfigDialogField>& fields,
    bool includeRemoveButton) :
    m_fields(fields),
    m_removeButtonPressed(false) {

    // Ensure that all labels are unique
    for (int i = 0; i < fields.size(); i += 1) {
        for (int j = i + 1; j < fields.size(); j += 1) {
            ASSERT_FA(fields.at(i).label == fields.at(j).label);
        }
    }

    // Set the layout for the dialog
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    // Add a groupbox
    QGroupBox* groupBox = new QGroupBox();
    QGridLayout* gridLayout = new QGridLayout();
    groupBox->setLayout(gridLayout);

    // Add a row for each field
    for (int row = 0; row < fields.size(); row += 1) {
        ConfigDialogField field = fields.at(row);

        // First, add the QLabel
        QLabel* label = new QLabel(field.label);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        gridLayout->addWidget(label, row, 0);

        // Next, create the line edit
        m_lineEdits[field.label] =
            new QLineEdit(field.initialLineEditValue.toString());
        connect(
            m_lineEdits[field.label], &QLineEdit::textChanged,
            this, &ConfigDialog::validate
        );

        // Next, if necessary, create a browse button
        QPushButton* browseButton = nullptr;
        if (field.fileBrowser) {
            browseButton = new QPushButton("Browse");
            connect(browseButton, &QPushButton::clicked, this, [=](){
                QString path;
                if (field.onlyDirectories) {
                    path = QFileDialog::getExistingDirectory(
                        this,
                        tr("Choose Directory")
                    );
                }
                else {
                    path = QFileDialog::getOpenFileName(
                        this,
                        tr("Choose File")
                    );
                }
                if (!path.isNull()) {
                    QString nativePath = QDir::toNativeSeparators(path);
                    m_lineEdits[field.label]->setText(nativePath);
                }
            });
        }

        // Handle the case of both combo box and line edit
        if (0 < field.comboBoxValues.size()) {

            // Initialize the group box
            QGroupBox* choicesBox = new QGroupBox();
            choicesBox->setStyleSheet("QGroupBox{border:0px;}");
            gridLayout->addWidget(choicesBox, row, 2, 1, 2);
            QGridLayout* choicesLayout = new QGridLayout();
            choicesLayout->setContentsMargins(0, 0, 0, 0);
            choicesBox->setLayout(choicesLayout);

            // Insert the combo box
            QRadioButton* comboBoxRadioButton = new QRadioButton("Built-in");
            m_comboBoxes[field.label] = new QComboBox();
            for (const auto& value : field.comboBoxValues) {
                m_comboBoxes[field.label]->addItem(value.toString());
            }
            int index = m_comboBoxes[field.label]->findText(
                field.initialComboBoxValue.toString()
            );
            if (index != -1) {
                m_comboBoxes[field.label]->setCurrentIndex(index);
            }
            choicesLayout->addWidget(comboBoxRadioButton, 0, 0);
            choicesLayout->addWidget(m_comboBoxes[field.label], 0, 1);

            // Insert the line edit
            QRadioButton* lineEditRadioButton = new QRadioButton("Custom");
            choicesLayout->addWidget(lineEditRadioButton, 1, 0);
            choicesLayout->addWidget(m_lineEdits[field.label], 1, 1);
            if (browseButton != nullptr) {
                choicesLayout->addWidget(browseButton, 1, 2);
            }

            // Make sure only one choice is enabled at a time
            connect(
                comboBoxRadioButton, &QRadioButton::toggled,
                this, [=](){
                    bool isChecked = comboBoxRadioButton->isChecked();
                    m_comboBoxes[field.label]->setEnabled(isChecked);
                    m_lineEdits[field.label]->setEnabled(!isChecked);
                    if (browseButton != nullptr) {
                        browseButton->setEnabled(!isChecked);
                    }
                }
            );

            // Enable the most recent choice, guaranteeing a toggle
            comboBoxRadioButton->setChecked(true);
            if (!field.comboBoxSelected) {
                lineEditRadioButton->setChecked(true);
            }
        }

        // Handle the case of just a line edit
        else {
            gridLayout->addWidget(m_lineEdits[field.label], row, 2);
            if (browseButton != nullptr) {
                gridLayout->addWidget(browseButton, row, 3);
            }
        }
    }

    // Add OK and cancel buttons
    m_buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    );
    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Potentially add a remove button
    if (includeRemoveButton) {
        QPushButton* removeButton = m_buttons->addButton(
            "&Remove",
            QDialogButtonBox::DestructiveRole
        );
        connect(removeButton, &QPushButton::clicked, this, [=](){
            QMessageBox::StandardButton response = QMessageBox::question(
                this, 
                "Remove " + object,
                "Are you sure?"
            );
            if (response == QMessageBox::StandardButton::Yes) {
                m_removeButtonPressed = true;
                accept();
            }
        });
    }

    // Add group box and buttons to dialog
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(m_buttons);

    // Misc. initialization
    resize(800, 0);
    setWindowTitle(action + " " + object);
    validate();
}

bool ConfigDialog::removeButtonPressed() {
    return m_removeButtonPressed;
}

QString ConfigDialog::getComboBoxValue(const QString& label) {
    if (!m_comboBoxes.contains(label)) {
        return QString();
    }
    return m_comboBoxes[label]->currentText();
}

QString ConfigDialog::getLineEditValue(const QString& label) {
    if (!m_lineEdits.contains(label)) {
        return QString();
    }
    return m_lineEdits[label]->text();
}

bool ConfigDialog::getComboBoxSelected(const QString& label) {
    if (!m_comboBoxes.contains(label)) {
        return false;
    }
    return m_comboBoxes[label]->isEnabled();
}

void ConfigDialog::validate() {
    bool valid = true;
    for (const auto& field : m_fields) {
        if (!field.allowEmptyLineEditValue) {
            if (
                !field.allowEmptyLineEditValue &&
                m_lineEdits[field.label]->text().isEmpty()
            ) {
                valid = false;
                break;
            }
        }
    }
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

} // namespace mms
