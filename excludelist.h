#ifndef EXCLUDELIST_H
#define EXCLUDELIST_H

#include <QString>

class ExcludeList {
  public:
    ExcludeList();
    ExcludeList(QString name);
    ExcludeList(QString list_name, bool activated);
    bool getIs_activated() const;
    void setIs_activated(bool newIs_activated);

    QVector<QString>* getList() const;
    void              setList(QVector<QString>* newList);

    void add_item(QString item);

    QString getName() const;
    void    setName(const QString& newName);

  private:
    QVector<QString>* list;
    bool              is_activated;
    QString           name;
};

#endif // EXCLUDELIST_H
