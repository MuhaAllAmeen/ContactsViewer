#include "contactslist.h"
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
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        this->clearItems();
        this->checkContacts();
    });
    timer->start(10000);
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

void  ContactsList::checkContacts()
{
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    QJniObject permissions = javaClass.callObjectMethod("checkPermission","()Ljava/lang/String;");
    if (permissions.toString()=="Permission Granted"){
        qDebug()<<"Permission Granted";
        QJniObject contacts = javaClass.callObjectMethod("loadContacts","()Ljava/lang/String;");
//                qDebug()<<"json"<<contacts.toString();
        QString jsonStr = contacts.toString();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
        QJsonArray jsonArray = jsonDoc.array();
        foreach (const QJsonValue & value, jsonArray) {
            QString name = value["name"].toString();
            QString number = value["number"].toString();
//                        qDebug()<<name<<number;
            Contact c;
            c.name=name;
            c.number=number;
            appendItem(c);
        }
    }
}

