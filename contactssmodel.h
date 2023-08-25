#ifndef CONTACTSSMODEL_H
#define CONTACTSSMODEL_H

#include <QAbstractListModel>
#include <QJniObject>

class ContactssModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QVector<QMap<QString,QVariant>> contacts READ getContacts WRITE setContacts)

public:
    explicit ContactssModel(QObject *parent = nullptr);
    enum {
        nameRole = Qt::UserRole,
        numberRole
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVector<QMap<QString,QVariant>> getContacts();
    void setContacts(QVector<QMap<QString,QVariant>> contacts);

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;
    /*QJsonArray*/QVariantList convertToJsonVar(JNIEnv *env,jstring jstr);
    void appendItem(QMap <QString,QVariant> contact);
    void checkContacts();
    void deleteContact(QVariantList delIDs);
    void updateItem(QMap<QString,QVariant> contact);
    int findIndexofId(QString id);
public slots:
    void deleteFromQml(int index);
private:
    QVector<QMap<QString,QVariant>> mContact;

};

#endif // CONTACTSSMODEL_H
