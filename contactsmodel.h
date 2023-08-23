#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QAbstractListModel>
class ContactsList;
class ContactsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ContactsList *list READ list WRITE setList)
public:
    explicit ContactsModel(QObject *parent = nullptr);
    enum {
        nameRole = Qt::UserRole,
        numberRole
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    ContactsList *list() const;
    void setList(ContactsList *list);
private:
    ContactsList *mList;
};

#endif // CONTACTSMODEL_H
