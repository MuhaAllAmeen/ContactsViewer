#ifndef CONTACTSLIST_H
#define CONTACTSLIST_H

#include <QObject>
#include <QVector>
struct Contact{
    QString name;
    QString number;
};

class ContactsList : public QObject
{
    Q_OBJECT

public:
    explicit ContactsList(QObject *parent = nullptr);
    QVector<Contact> contacts() const;
    bool setItemAt(int index,const Contact &contact);
signals:
    void preItemAppended();
    void postItemAppended();
    void preItemRemoved(int index);
    void postItemRemoved();
    void contactsUpdated();
public slots:
    void appendItem(Contact contact);
    void clearItems();
    void checkContacts();
private:
    QVector<Contact> mContact;
};

#endif // CONTACTSLIST_H
