#include "Settings.h"

#include <QCoreApplication>
#include <QSettings>

#include "AssertMacros.h"

namespace mms {

Settings *Settings::INSTANCE = nullptr;

void Settings::init() {
  ASSERT_TR(INSTANCE == nullptr);
  INSTANCE = new Settings();
}

Settings *Settings::get() {
  ASSERT_FA(INSTANCE == nullptr);
  return INSTANCE;
}

QString Settings::value(QString group, QString key) {
  QSettings settings;
  settings.beginGroup(group);
  return settings.value(key).toString();
}

void Settings::update(QString group, QString key, QString value) {
  QSettings settings;
  settings.beginGroup(group);
  settings.setValue(key, value);
}

QStringList Settings::values(QString group, QString key) {
  QStringList values;
  for (const auto &entry : getGroup(group)) {
    values << entry.value(key);
  }
  values.sort(Qt::CaseInsensitive);
  return values;
}

void Settings::add(QString group, QMap<QString, QString> entry) {
  // Group and entry must be nonempty
  ASSERT_FA(group.isEmpty());
  ASSERT_FA(entry.isEmpty());

  // Get the current size
  QSettings settings;
  int size = settings.beginReadArray(group);
  settings.endArray();

  // Append the element at the current size
  settings.beginWriteArray(group);
  settings.setArrayIndex(size);
  QMap<QString, QString>::const_iterator it;
  for (it = entry.constBegin(); it != entry.constEnd(); it += 1) {
    settings.setValue(it.key(), it.value());
  }
  settings.endArray();
}

void Settings::remove(QString group, QString key, QString value) {
  // Group and key must be nonempty
  ASSERT_FA(group.isEmpty());
  ASSERT_FA(key.isEmpty());

  // Get all existing entries
  QVector<QMap<QString, QString>> entries = getGroup(group);

  // Remove all existing entries
  QSettings settings;
  settings.remove(group);

  // Write back entries that don't have value given
  settings.beginWriteArray(group);
  int index = 0;
  for (const auto &entry : entries) {
    if (entry.value(key) != value) {
      settings.setArrayIndex(index);
      QMap<QString, QString>::const_iterator it;
      for (it = entry.constBegin(); it != entry.constEnd(); it += 1) {
        settings.setValue(it.key(), it.value());
      }
      index += 1;
    }
  }
  settings.endArray();
}

QVector<QMap<QString, QString>> Settings::find(QString group, QString key,
                                               QString value) {
  // Group and key must be nonempty
  ASSERT_FA(group.isEmpty());
  ASSERT_FA(key.isEmpty());

  // Find entries that match the criteria
  QVector<QMap<QString, QString>> entries;
  for (const auto &entry : getGroup(group)) {
    if (entry.value(key) == value) {
      entries.append(entry);
    }
  }
  return entries;
}

void Settings::update(QString group, QString key, QString value,
                      QMap<QString, QString> changes) {
  // Group and key must be nonempty
  ASSERT_FA(group.isEmpty());
  ASSERT_FA(key.isEmpty());

  // Find the entries to update
  QVector<QMap<QString, QString>> entries = find(group, key, value);

  // Remove those entries
  remove(group, key, value);

  // Re-add the entries, but with updates
  for (QMap<QString, QString> entry : entries) {
    QMap<QString, QString>::const_iterator it;
    for (it = changes.constBegin(); it != changes.constEnd(); it += 1) {
      entry[it.key()] = it.value();
    }
    add(group, entry);
  }
}

Settings::Settings() {
  QCoreApplication::setOrganizationName("mackorone");
  QCoreApplication::setOrganizationDomain("www.github.com/mackorone");
  QCoreApplication::setApplicationName("mms");
}

QVector<QMap<QString, QString>> Settings::getGroup(QString group) {
  // Group must be nonempty
  ASSERT_FA(group.isEmpty());

  // Get all entries for a given group
  QVector<QMap<QString, QString>> entries;
  QSettings settings;
  int size = settings.beginReadArray(group);
  for (int i = 0; i < size; i += 1) {
    settings.setArrayIndex(i);
    QMap<QString, QString> map;
    for (QString key : settings.allKeys()) {
      map[key] = settings.value(key).toString();
    }
    entries.append(map);
  }
  settings.endArray();
  return entries;
}

}  // namespace mms
