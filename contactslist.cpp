#include "contactslist.h"
#include "qabstractitemmodel.h"
#include <QJniObject>
#include <jni.h>
#include <qnativeinterface.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>



ContactsList::ContactsList(QObject *parent)
    : QObject{parent}
{
    this->checkContacts();
}

QVector<Contact> ContactsList::contacts() const
{
    return mContact;
}

bool ContactsList::setItemAt(int index, const Contact &contact)
{
    if (index < 0 || index >= mContact.size()){
        return false;
    }
    const Contact &oldContact = mContact.at(index);
    if(contact.name == oldContact.name && contact.number==oldContact.number){
        return false;
    }
    mContact[index] = contact;
    return true;
}

void ContactsList::appendItem(Contact contact)
{

    emit preItemAppended();
    mContact.append(contact);
    emit postItemAppended();

}

void ContactsList::clearItems()
{
    emit preItemRemoved(0);
    mContact.clear();
    emit postItemRemoved();
}

void ContactsList::deleteContact(QString id)
{
    qDebug()<<"called delete";
    for (int index=0; index<mContact.size();index++){
        if (mContact.at(index).id==id){
            qDebug()<<"found";
            qDebug()<<mContact.size();
           emit preItemRemoved(index);
            mContact.removeAt(index);
           emit postItemRemoved();
        }
    }
}

void  ContactsList::checkContacts()
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    QJniObject permissions = javaClass.callObjectMethod("checkPermission","()Ljava/lang/String;");
    if (permissions.toString()!="Permission Denied"){
        qDebug()<<"Permission Granted";
        QJniObject contacts = javaClass.callObjectMethod("loadContacts","()Ljava/lang/String;");
        QString jsonStr = contacts.toString();
        if (jsonStr.size()>12){
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
            QJsonArray jsonArray = jsonDoc.array();
    //        this->clearItems();
            foreach (const QJsonValue & value, jsonArray) {
                QString id = value["id"].toString();
                QString name = value["name"].toString();
                QString number = value["number"].toString();
    //            qDebug()<<name<<number<<id;
                Contact c;
                c.name=name;
                c.number=number;
                c.id=id;
                appendItem(c);
            }
        }
        else{
            deleteContact(jsonStr);
        }
    }
}



