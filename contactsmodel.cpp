#include "contactsmodel.h"
#include "contactslist.h"
ContactsModel::ContactsModel(QObject *parent)
    : QAbstractListModel(parent), mList(nullptr)
{

}

int ContactsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !mList)
        return 0;

    return mList->contacts().size();
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const Contact contact = mList->contacts().at(index.row());
    switch(role){
    case nameRole:
        return QVariant(contact.name);
    case numberRole:
        return QVariant(contact.number);
    }


    return QVariant();
}

bool ContactsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!mList){
        return false;
    }
    Contact contact = mList->contacts().at(index.row());
    switch(role){
    case nameRole:
        contact.name = value.toString();
        break;
    case numberRole:
        contact.number = value.toString();
        break;
    }
    if (mList->setItemAt(index.row(),contact)) {
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags ContactsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ContactsModel::roleNames() const
{
    QHash<int,QByteArray> names;
    names[nameRole] = "name";
    names[numberRole]= "number";
    return names;
}

ContactsList *ContactsModel::list() const
{
    return mList;
}

void ContactsModel::setList(ContactsList *list)
{
    beginResetModel() ;
    if (mList)
        mList->disconnect(this);
    mList = list;
    if (mList) {
        connect (mList, &ContactsList::preItemAppended, this, [=]() {
            const int index = mList->contacts().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect (mList, &ContactsList::postItemAppended, this, [=]() {
            endInsertRows();
        });
        connect(mList, &ContactsList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &ContactsList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
    endResetModel();
    }
}
