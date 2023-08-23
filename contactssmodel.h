#ifndef CONTACTSSMODEL_H
#define CONTACTSSMODEL_H

#include <QAbstractListModel>
struct Contacts{
    QString name;
    QString number;
    QString id;
};
class ContactssModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QVector<Contacts> contacts READ getContacts WRITE setContacts)

public:
    explicit ContactssModel(QObject *parent = nullptr);
    enum {
        nameRole = Qt::UserRole,
        numberRole
    };
    bool setItemAt(int index,const Contacts &contact);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVector<Contacts> getContacts();
    void setContacts(QVector<Contacts> contacts);
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:

    virtual QHash<int, QByteArray> roleNames() const override;
    // Remove data:

    void appendItem(Contacts contact);
    void checkContacts();
    void deleteContact(QString id);
    void updateItem(Contacts c);
private:
    QVector<Contacts> mContact;
//    QList<QVector<Contacts>> mcontacts;

};

#endif // CONTACTSSMODEL_H
