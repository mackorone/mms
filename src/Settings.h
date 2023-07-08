#pragma once

#include <QMap>
#include <QString>
#include <QStringList>
#include <QVector>

namespace mms {

class Settings {
 public:
  static void init();
  static Settings *get();

  // --- Non-array Functions --- //

  QString value(QString group, QString key);
  void update(QString group, QString key, QString value);

  // --- Array-group Functions --- //

  // Returns all values for the given group and key
  QStringList values(QString group, QString key);

  // Adds the given entry to the given group
  void add(QString group, QMap<QString, QString> entry);

  // Removes all entries from group with value for key
  void remove(QString group, QString key, QString value);

  // Returns all entries in group with value for key
  QVector<QMap<QString, QString>> find(QString group, QString key,
                                       QString value);

  // Updates all entries in group with value for key
  void update(QString group, QString key, QString value,
              QMap<QString, QString> changes);

 private:
  Settings();
  static Settings *INSTANCE;

  // Returns all entries for a given group
  QVector<QMap<QString, QString>> getGroup(QString group);
};

}  // namespace mms
