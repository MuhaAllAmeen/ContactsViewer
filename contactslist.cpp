#include "contactslist.h"
#include <QJniObject>
#include <jni.h>
#include <qnativeinterface.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


ContactsList::ContactsList(QObject *parent)
    : QObject{parent}
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    QJniObject permissions = javaClass.callObjectMethod("checkPermission","()Ljava/lang/String;");
    if (permissions.toString()=="Permission Granted"){
        qDebug()<<"Permission Granted";
        QJniObject contacts = javaClass.callObjectMethod("loadContacts","()Ljava/lang/String;");
        //        qDebug()<<"json"<<contacts.toString();
        QString jsonStr = contacts.toString();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
        jobject contactObj = contacts.object<jobject>();
        QJsonArray jsonArray = jsonDoc.array();
        foreach (const QJsonValue & value, jsonArray) {
            QString name = value["name"].toString();
            QString number = value["number"].toString();
            qDebug()<<name<<number;
            mContact.append({name,number});
        }
    }
    mContact.append({QStringLiteral("john"),QStringLiteral("+987654321")});
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

void ContactsList::appendItem()
{
    emit preItemAppended();
    Contact contact;
    mContact.append(contact);
    emit postItemAppended();
}
