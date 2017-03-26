#include "ConfigDialog.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "ContainerUtilities.h"

namespace mms {

ConfigDialog::ConfigDialog(
        const QString& action,
        const QString& object,
        const QVector<ConfigDialogField>& fields,
        bool includeRemoveButton) :
        m_removeButtonPressed(false) {

    // Set the main layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    // Add a groupbox
    QGroupBox* groupBox = new QGroupBox();
    QGridLayout* gridLayout = new QGridLayout();
    groupBox->setLayout(gridLayout);

    // Add rows for all required config
    int row = 0;
    for (const ConfigDialogField& field : fields) {

        // Add the QLabel and QLineEdit 
        QLabel* label = new QLabel(field.label);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        // TODO: MACK - shouldn't always be a string...
        m_lineEdits[field.key] = new QLineEdit(field.initialValue.toString());
        gridLayout->addWidget(label, row, 0);
        gridLayout->addWidget(m_lineEdits[field.key], row, 2);

        // Add a browse button if necessary
        if (field.fileBrowser) {
            QPushButton* browseButton = new QPushButton("Browse");
            gridLayout->addWidget(browseButton, row, 3);
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
                    m_lineEdits[field.key]->setText(nativePath);
                }
            });
        }

        // Don't allow submission unless all values are valid
        // TODO: MACK - I want to actually call this function on startup to ensure starting value isn't invalid
        // TODO: MACK - if the filters are inclusive, use a combo box
        connect(m_lineEdits[field.key], &QLineEdit::textChanged, this, [=](){
            QString text = m_lineEdits[field.key]->text();
            bool pass = (
                field.filterValues.contains(text) ==
                field.inclusiveFilter
            );
            m_buttons->button(QDialogButtonBox::Ok)->setEnabled(pass);
        });

        row += 1;
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
}

bool ConfigDialog::removeButtonPressed() {
    return m_removeButtonPressed;
}

QString ConfigDialog::getValue(const QString& key) {
    return m_lineEdits[key]->text();
}

} // namespace mms
