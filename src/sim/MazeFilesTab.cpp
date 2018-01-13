#include "MazeFilesTab.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

#include "MazeFileType.h"
#include "SettingsMazeFiles.h"

namespace mms {

MazeFilesTab::MazeFilesTab() : m_table(new QTableWidget()) {

    // Set up the layout
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    layout->addLayout(buttonsLayout);

    // Create the import button
    QPushButton* importButton = new QPushButton("Import Maze File(s)");
    connect(importButton, &QPushButton::clicked, this, &MazeFilesTab::import);
    buttonsLayout->addWidget(importButton);

    // Create the remove button
    QPushButton* removeButton = new QPushButton("Remove Selected File");
    connect(removeButton, &QPushButton::clicked, this, &MazeFilesTab::remove);
    connect(m_table, &QTableWidget::itemSelectionChanged, this, [=](){
        // TODO: upforgrabs
        // We should disable the remove button for the builtin mazes
        removeButton->setEnabled(0 < m_table->selectedItems().size());
    });
    removeButton->setEnabled(false);
    buttonsLayout->addWidget(removeButton);

    // Initialize the table
    m_table->horizontalHeader()->setHighlightSections(false);
    m_table->setAutoScroll(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setSortingEnabled(true);
    connect(m_table, &QTableWidget::itemSelectionChanged, this, [=](){
        const auto& selected = m_table->selectedItems();
        if (0 < selected.size()) {
            QString path = m_table->item(m_table->currentRow(), 1)->text();
            emit mazeFileChanged(path);
        }
    });
    layout->addWidget(m_table);

    // Adds the entries to the table
    refresh();
}

void MazeFilesTab::import() {
    QStringList suffixes;
    for (const QString& suffix : MAZE_FILE_TYPE_TO_SUFFIX().values()) {
        suffixes.append(QString("*.") + suffix);
    }
    QStringList paths = QFileDialog::getOpenFileNames(
        this,
        "Import Maze File(s)",
        "", // Don't specify directory
        QString("(") + suffixes.join(" ") + ")"
    );
    // TODO: upforgrabs
    // We should detect duplicate paths and ask the user
    // whether or not they'd like to skip adding them
    for (const QString& path : paths) {
        SettingsMazeFiles::addMazeFile(path);
    }
    refresh();
}

void MazeFilesTab::remove() {
    const auto& selected = m_table->selectedItems();
    ASSERT_LT(0, selected.size());
    QString path = m_table->item(m_table->currentRow(), 1)->text();
    SettingsMazeFiles::removeMazeFile(path);
    refresh();
}

void MazeFilesTab::refresh() {
    m_table->clear();
    m_table->setColumnCount(2);
    m_table->setHorizontalHeaderLabels({"File Name", "File Path"});
    QStringList mazeFiles;
    // TODO: upforgrabs
    // Verify that the path separators works on windows
    QString mazeFilesPath = ":/resources/mazes/";
    for (const auto& file : QDir(mazeFilesPath).entryList()) {
        mazeFiles.append(mazeFilesPath + file);
    }
    for (const auto& path : SettingsMazeFiles::getSettingsMazeFiles()) {
        mazeFiles.append(path);
    }
    m_table->setRowCount(mazeFiles.size());
    for (int i = 0; i < mazeFiles.size(); i += 1) {
        QString path = mazeFiles.at(i);
        QString name = QFileInfo(path).fileName();
        m_table->setItem(i, 0, new QTableWidgetItem(name));
        m_table->setItem(i, 1, new QTableWidgetItem(path));
    }
    m_table->resizeColumnsToContents();
}

} // namespace mms
