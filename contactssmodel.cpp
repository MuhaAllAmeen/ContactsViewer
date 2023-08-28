#include "contactssmodel.h"
#include "qforeach.h"
#include <QJniObject>
#include <jni.h>
#include <qnativeinterface.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVectorIterator>
extern "C" JNIEXPORT void JNICALL Java_com_example_appContacts_MainActivity_sendUpdatedContacts(JNIEnv *env, jobject obj,jstring jstr, jlong ptr, jboolean firstPass) {
    if(ptr!=0){
        ContactssModel* contactsModel = reinterpret_cast<ContactssModel*>(ptr);
        foreach (const QVariant value, contactsModel->convertToJsonVar(env,jstr)) {
            if(firstPass){

                contactsModel->appendItem(value.value<QVariantMap>());
            }else{
                contactsModel->updateItem(value.value<QVariantMap>());
            }
        }
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_example_appContacts_MainActivity_sendDeletedIDs(JNIEnv *env, jobject obj,jstring jstr, jlong ptr) {

    ContactssModel* contactsModel = reinterpret_cast<ContactssModel*>(ptr);
    contactsModel->deleteContact(contactsModel->convertToJsonVar(env,jstr));

}

ContactssModel::ContactssModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    jlong ptr = javaClass.callMethod<long>("setPointer","(J)J",(long)(ContactssModel *)this);
    QJniObject permissions = javaClass.callObjectMethod("checkPermission","()Ljava/lang/String;");
    if (permissions.toString()!="Permission Denied"){
        qDebug()<<"Permission Granted";
        javaClass.callMethod<void>("loadContacts","()V");
    }
}

QVariantList ContactssModel::convertToJsonVar(JNIEnv *env, jstring jstr)
{
    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
    QString contacts = QString::fromUtf8(cstr);
    env->ReleaseStringUTFChars(jstr, cstr);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(contacts.toUtf8());
    QVariantList jsonVariant = jsonDoc.toVariant().value<QVariantList>();
    return jsonVariant;
}

int ContactssModel::rowCount(const QModelIndex &parent) const
{

    if (parent.isValid())
        return 0;

    return mContact.size();
}


void ContactssModel::appendItem(QMap<QString,QVariant> contact){
    const int index = mContact.size();
    beginInsertRows(QModelIndex(), index, index);

    mContact.append(contact);
    endInsertRows();
    qDebug()<<"size"<<mContact.size();

}

QVariant ContactssModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const QMap<QString,QVariant> contact = mContact.at(index.row());
    switch(role){
    case nameRole:
        return contact.values().at(1);
    case numberRole:
        return contact.values().at(2);
    }

    return QVariant();
}

QVector<QMap<QString,QVariant>> ContactssModel::getContacts()
{
    return mContact;
}

void ContactssModel::setContacts(QVector<QMap<QString,/*QStringList*/QVariant>> contacts)
{
    beginResetModel();
    endResetModel();
}



bool ContactssModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags ContactssModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ContactssModel::roleNames() const
{
    QHash<int,QByteArray> names;
    names[nameRole] = "name";
    names[numberRole]= "number";
    return names;
}

void ContactssModel::deleteContact(QVariantList delIDs)
{
    int size = delIDs.size();
//    int i=0;
//    while(i<size && size!=0){
        for (int i=0; i<size; i++){
            qDebug()<<delIDs[i];
            int index = findIndexofId(delIDs[i].toString());
            if (index!=-1 && size!=0){
                qDebug()<<"json"<<delIDs[i].toString();
                qDebug()<<size<<i;
                beginRemoveRows(QModelIndex(), index, index);
                mContact.removeAt(index);
                endRemoveRows();
//                i++;
            }
            else{
//                i++;
                qDebug()<<"s"<<size<<i;
            }
        }
//    }
}

void ContactssModel::updateItem(QMap<QString,QVariant> contact)
{
    int index = findIndexofId(contact.values().at(0).toString());
    if(index!=-1){
        mContact[index]=contact;
        setData(createIndex(index,0),contact.values().at(1),nameRole);
        setData(createIndex(index,0),contact.values().at(2),numberRole);
        emit dataChanged(createIndex(index,0),createIndex(index,0),{nameRole,numberRole});
    }
    else{
         this->appendItem(contact);
    }

}

int ContactssModel::findIndexofId(QString id)
{
    for (int index=0; index<mContact.size();index++){
        if(id==mContact.at(index).values().at(0)){
            return index;
        }
    }
    return -1;
}

void ContactssModel::deleteFromQml(int index)
{
    int id = mContact.at(index).values().at(0).toInt();
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    jboolean found = javaClass.callMethod<jboolean>("deleteContactbyID","(I)Z",id);
    if (found){
        beginRemoveRows(QModelIndex(), index, index);
        mContact.removeAt(index);
        endRemoveRows();
    }
    else{
        javaClass.callMethod<void>("contactNotfound","()V");
    }

}
