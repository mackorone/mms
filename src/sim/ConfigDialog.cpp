#include "ConfigDialog.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "Assert.h"

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
		appendTextField(field, gridLayout);
    }

    // Add OK and cancel buttons
    m_buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel
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

QString ConfigDialog::getLineEditValue(const QString& label) {
    if (!m_lineEdits.contains(label)) {
        return QString();
    }
    return m_lineEdits[label]->text();
}

void ConfigDialog::addLabel(QString text, QGridLayout* layout, int row) {
	QLabel* label = new QLabel(text);
	label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	layout->addWidget(label, row, 0);
}

void ConfigDialog::appendTextField(
	ConfigDialogField field,
	QGridLayout* layout
) {
    // Add the label
	int row = layout->rowCount();
	addLabel(field.label, layout, row);

    // Next, create the line edit
    m_lineEdits[field.label] =
        new QLineEdit(field.initialLineEditValue.toString());
    connect(
        m_lineEdits[field.label], &QLineEdit::textChanged,
        this, &ConfigDialog::validate
    );

    // Next, if necessary, create a browse button
    QPushButton* browseButton = nullptr;
    if (
        field.type == ConfigDialogFieldType::FILE || 
        field.type == ConfigDialogFieldType::DIRECTORY
    ) {
        browseButton = new QPushButton("Browse");
        connect(browseButton, &QPushButton::clicked, this, [=](){
            QString path;
            if (field.type == ConfigDialogFieldType::FILE) {
                path = QFileDialog::getOpenFileName(
                    this,
                    tr("Choose File")
                );
            }
            else {
                path = QFileDialog::getExistingDirectory(
                    this,
                    tr("Choose Directory"),
                    QString(),
                    QFileDialog::ShowDirsOnly
                );
            }
            if (!path.isNull()) {
                QString nativePath = QDir::toNativeSeparators(path);
                m_lineEdits[field.label]->setText(nativePath);
            }
        });
    }

    // Add the line edit to the layout
    layout->addWidget(m_lineEdits[field.label], row, 2);
    if (browseButton != nullptr) {
        layout->addWidget(browseButton, row, 3);
    }
}

void ConfigDialog::validate() {
    bool valid = true;
    for (const auto& field : m_fields) {
        if (
            !field.allowEmptyLineEditValue &&
            m_lineEdits[field.label]->text().isEmpty()
        ) {
            valid = false;
            break;
        }
    }
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

} // namespace mms
