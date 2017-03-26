#pragma once

#include <QTableWidget>
#include <QWidget>

namespace mms {

class MazeFilesTab : public QWidget {

    Q_OBJECT

public:

    MazeFilesTab();

signals:

    void mazeFileChanged(const QString& path);

private:

    QTableWidget* m_table;
    void import();
    void remove();
    void refresh();

};

} // namespace mms
