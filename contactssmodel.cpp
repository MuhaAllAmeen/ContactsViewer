#include "contactssmodel.h"
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
        foreach (const QJsonValue &value, contactsModel->convertToJsonArr(env,jstr)) {
            QString id = value["id"].toString();
            QString name = value["name"].toString();
            QString number = value["number"].toString();
            QStringList qcontact = {name,number};
            QMap<QString,QStringList> contactsMap;
            contactsMap.insert(id,qcontact);
            if(firstPass){
                contactsModel->appendItem(contactsMap);
            }else{
                contactsModel->updateItem(contactsMap);
            }
        }
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_example_appContacts_MainActivity_sendDeletedIDs(JNIEnv *env, jobject obj,jstring jstr, jlong ptr) {

    ContactssModel* contactsModel = reinterpret_cast<ContactssModel*>(ptr);
    contactsModel->deleteContact(contactsModel->convertToJsonArr(env,jstr));

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

QJsonArray ContactssModel::convertToJsonArr(JNIEnv *env, jstring jstr)
{
    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
    QString contacts = QString::fromUtf8(cstr);
    env->ReleaseStringUTFChars(jstr, cstr);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(contacts.toUtf8());
    QJsonArray jsonArray = jsonDoc.array();
    return jsonArray;
}

int ContactssModel::rowCount(const QModelIndex &parent) const
{

    if (parent.isValid())
        return 0;

    return mContact.size();
}


void ContactssModel::appendItem(QMap<QString,QStringList> contact){
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

    const QMap<QString,QStringList> contact = mContact.at(index.row());
//    qDebug()<<"i"<<contact.keys().at(0);
    switch(role){
    case nameRole:
        return QVariant(contact.values().at(0).at(0));
    case numberRole:
        return QVariant(contact.values().at(0).at(1));
    }

    return QVariant();
}

QVector<QMap<QString,QStringList>> ContactssModel::getContacts()
{
    return mContact;
}

void ContactssModel::setContacts(QVector<QMap<QString,QStringList>> contacts)
{
    beginResetModel();
    checkContacts();
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

void ContactssModel::checkContacts()
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    QJniObject permissions = javaClass.callObjectMethod("checkPermission","()Ljava/lang/String;");
    if (permissions.toString()!="Permission Denied"){
        qDebug()<<"Permission Granted";
        QJniObject contacts = javaClass.callObjectMethod("loadContacts","()Ljava/lang/String;");
        QString jsonStr = contacts.toString();
        //        qDebug()<<jsonStr;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
        QJsonArray jsonArray = jsonDoc.array();
        foreach (const QJsonValue & value, jsonArray) {
            QString id = value["id"].toString();
            QString name = value["name"].toString();
            QString number = value["number"].toString();
            QStringList qcontact = {name,number};
            QMap<QString,QStringList> contactsMap;
            contactsMap.insert(id,qcontact);
            //            qDebug()<<name<<number<<id;
            this->appendItem(contactsMap);
        }
    }
}
void ContactssModel::deleteContact(QJsonArray delIDJson)
{
    int size = delIDJson.size();
    int i=0;
    while(i<size && size!=0){
        int index = findIndexofId(delIDJson[i].toString());
        if (index!=-1){
            qDebug()<<"json"<<delIDJson[i].toString();
            qDebug()<<size<<i;
            beginRemoveRows(QModelIndex(), index, index);
            mContact.removeAt(index);
            endRemoveRows();
            i++;
        }
        else{
            i++;
            qDebug()<<"s"<<size<<i;
        }
    }
}

void ContactssModel::updateItem(QMap<QString,QStringList> contact)
{
    int index = findIndexofId(contact.keys().at(0));
    if(index!=-1){
        mContact[index]=contact;
        setData(createIndex(index,0),contact.values().at(0).at(0),nameRole);
        setData(createIndex(index,0),contact.values().at(0).at(1),numberRole);
        emit dataChanged(createIndex(index,0),createIndex(index,0),{nameRole,numberRole});
    }
    else{
         this->appendItem(contact);
    }

}

int ContactssModel::findIndexofId(QString id)
{
    for (int index=0; index<mContact.size();index++){
        if(id==mContact.at(index).keys().at(0)){
            return index;
        }
    }
    return -1;
}

void ContactssModel::deleteFromQml(int index)
{

    beginRemoveRows(QModelIndex(), index, index);
    int id = mContact.at(index).keys().at(0).toInt();
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    javaClass.callMethod<void>("deleteContactbyID","(I)V",id);
    mContact.removeAt(index);
    endRemoveRows();

}
