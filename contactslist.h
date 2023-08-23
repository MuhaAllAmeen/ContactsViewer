#ifndef CONTACTSLIST_H
#define CONTACTSLIST_H
#include <QObject>
#include <QVector>

struct Contact{
    QString name;
    QString number;
    QString id;
};

class ContactsList : public QObject
{
    Q_OBJECT
public:
    explicit ContactsList(QObject *parent = nullptr);
    QVector<Contact> contacts() const;
    bool setItemAt(int index,const Contact &contact);
    static ContactsList* getInstance(/*QObject *parent = nullptr*/);
    static void setInstance(ContactsList *cl);
    static ContactsList* instance;
signals:
    void preItemAppended();
    void postItemAppended();
    void preItemRemoved(int index);
    void postItemRemoved();
    void contactChanged(int index, Contact c);
    void contactsUpdated(const Contact contact);
    void contactsDeleted(const QString &id);
public slots:
    void appendItem(Contact contact);
    void checkContacts();
    void deleteContact(QString id);
    void updateItem(Contact c);
private:
    QVector<Contact> mContact;
};

#endif // CONTACTSLIST_H
