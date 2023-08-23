#include "contactslist.h"
#include <QJniObject>
#include <jni.h>
#include <qnativeinterface.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
//extern "C" JNIEXPORT void JNICALL Java_com_example_appContacts_MainActivity_sendUpdatedContacts(JNIEnv *env, jobject obj,jstring jstr, jlong ptr) {
//    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
//    QString contacts = QString::fromUtf8(cstr);
//    env->ReleaseStringUTFChars(jstr, cstr);
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(contacts.toUtf8());
//    QJsonArray jsonArray = jsonDoc.array();
//    qDebug()<<"called"<<ptr;
//    if(ptr!=0){
//        qDebug()<<"called"<<ptr;

//        ContactsList *cl = reinterpret_cast<ContactsList *>(ptr);
//        foreach (const QJsonValue &value, jsonArray) {
//            QString id = value["id"].toString();
//            QString name = value["name"].toString();
//            QString number = value["number"].toString();
//            Contact c;
//            qDebug()<<"ss"<<id<<name<<number;
//            c.name=name; c.id=id; c.number=number;
//            cl->appendItem({"ss","ss","ss"});
//            qDebug()<<"in"<<ContactsList::getInstance();
//            ContactsList::instance->appendItem(c);
//            ContactsList::getInstance()->appendItem(c);
//            ContactsList::getInstance()->appendItem({"ss","ss","ss"});
//            cl->appendItem(c);

//    //        QMetaObject::invokeMethod(globalContactsList, "contactsUpdated",Q_ARG(Contact,c));
//        }
//    }

//}
//extern "C" JNIEXPORT void JNICALL Java_com_example_appContacts_MainActivity_sendDeletedIDs(JNIEnv *env, jobject obj,jstring jstr, jlong ptr) {
//    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
//    QString contacts = QString::fromUtf8(cstr);
//    env->ReleaseStringUTFChars(jstr, cstr);
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(contacts.toUtf8());
//    QJsonArray jsonArray = jsonDoc.array();
//    foreach (const QJsonValue &value, jsonArray) {
//        QMetaObject::invokeMethod(globalContactsList, "contactsDeleted",Q_ARG(QString,value.toString()));
//    }
//}

ContactsList* ContactsList::instance = nullptr;
ContactsList::ContactsList(QObject *parent)
    : QObject{parent}
{
    this->checkContacts();
    instance = this;

}
ContactsList* ContactsList::getInstance(/*QObject *parent*/)
{
//    if (!instance)
    qDebug()<<"instance"<<instance;
//        instance = new ContactsList(/*parent*/);
    return instance;
}

void ContactsList::setInstance(ContactsList *cl)
{
    instance=cl;
    qDebug()<<instance;
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



void ContactsList::deleteContact(QString id)
{
    for (int index=0; index<mContact.size();index++){
        if (mContact.at(index).id==id){
           emit preItemRemoved(index);
            mContact.removeAt(index);
           emit postItemRemoved();
        }
    }
}

void ContactsList::updateItem(Contact c)
{
    jboolean found = false;
    for (int index=0; index<mContact.size();index++){
        if (mContact.at(index).id==c.id){
           emit contactChanged(index,c);
           found=true;
        }
    }
    if(!found){
        this->appendItem(c);
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
//        qDebug()<<jsonStr;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());
            QJsonArray jsonArray = jsonDoc.array();
            foreach (const QJsonValue & value, jsonArray) {
                QString id = value["id"].toString();
                QString name = value["name"].toString();
                QString number = value["number"].toString();
    //            qDebug()<<name<<number<<id;
                Contact c;
                c.name=name;
                c.number=number;
                c.id=id;
                this->appendItem(c);
            }
    }
}



