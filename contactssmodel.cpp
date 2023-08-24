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
            Contacts contact;
            contact.name=name; contact.id=id; contact.number=number;
            if(firstPass){
                contactsModel->appendItem(contact);
            }else{
                contactsModel->updateItem(contact);
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


void ContactssModel::appendItem(Contacts c){
    const int index = mContact.size();
    beginInsertRows(QModelIndex(), index, index);
    mContact.append(c);
    endInsertRows();
}

QVariant ContactssModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Contacts contact = mContact.at(index.row());
    switch(role){
    case nameRole:
        return QVariant(contact.name);
    case numberRole:
        return QVariant(contact.number);
    }

    return QVariant();
}

QVector<Contacts> ContactssModel::getContacts()
{
    return mContact;
}

void ContactssModel::setContacts(QVector<Contacts> contacts)
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
            //            qDebug()<<name<<number<<id;
            Contacts c;
            c.name=name;
            c.number=number;
            c.id=id;
            this->appendItem(c);
        }
    }
}
void ContactssModel::deleteContact(QJsonArray delIDJson)
{

    beginResetModel();
    int size = delIDJson.size();
    int i=0;
    while(i<size && size!=0){
        jboolean found = false;
        for (int index=0; index<mContact.size();index++){
            if (mContact.at(index).id==delIDJson[i].toString()){
                qDebug()<<"json"<<delIDJson[i].toString();
                beginRemoveRows(QModelIndex(), index, index);
                mContact.removeAt(index);
                endRemoveRows();
                i++;
                found=true;
            }
            if(found){
                break;
            }
        }
        if(found==false){
            break;
        }
    }
    endResetModel();
}

void ContactssModel::updateItem(Contacts contact)
{
    beginResetModel();
    jboolean found = false;
    for (int index=0; index<mContact.size();index++){
        if (mContact.at(index).id==contact.id){
            mContact[index].name=contact.name;
            mContact[index].number=contact.number;
            setData(createIndex(index,0),contact.name,nameRole);
            setData(createIndex(index,0),contact.number,numberRole);
            found=true;
            break;
        }
    }
    if(!found){
        this->appendItem(contact);
    }
    endResetModel();

}
