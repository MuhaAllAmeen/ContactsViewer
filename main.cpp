#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJniObject>
#include <jni.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QQmlContext>
#include "contactsmodel.h"
#include "contactslist.h"
#define JNIIMPORT
#define JNIEXPORT  __attribute__ ((visibility ("default")))
#define JNICALL
static ContactsList *globalContactsList = nullptr;
extern "C" JNIEXPORT void JNICALL Java_com_example_appContacts_MainActivity_sendUpdatedContacts(JNIEnv *env, jobject obj, jstring jstr) {
    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
    QString qstr = QString::fromUtf8(cstr);
    //    qDebug()<<"ssss"<<qstr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(qstr.toUtf8());
    QJsonArray jsonArray = jsonDoc.array();
    QMetaObject::invokeMethod(globalContactsList, "contactsDeleteBeforeUpdate");
//    foreach (const QJsonValue & value, jsonArray) {
//        QString name = value["name"].toString();
//        QString number = value["number"].toString();
//        qDebug()<<name<<number;
        Contact c;
//        c.name=name;
//        c.number=number;
        QMetaObject::invokeMethod(globalContactsList, "contactsUpdated", Q_ARG(Contact, c));
//    }
}
int main(int argc, char *argv[])
{
    qmlRegisterType<ContactsModel>("Contacts",1,0,"ContactsModel");
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    ContactsList *contactsList=new ContactsList(&app);
    QObject::connect(contactsList, &ContactsList::contactsDeleteBeforeUpdate, [contactsList]() {
        contactsList->clearItems();
    });
    QObject::connect(contactsList, &ContactsList::contactsUpdated, [contactsList](const Contact &contacts) {
//        contactsList->appendItem(contacts);
        contactsList->checkContacts();
    });
    globalContactsList = contactsList;
    engine.rootContext()->setContextProperty(QStringLiteral("contactsList"),contactsList);
    qmlRegisterUncreatableType<ContactsList>("Contacts",1,0,"ContactsList",
                                             QStringLiteral("Contacts List Should not be created in Qml"));
    const QUrl url(u"qrc:/Contacts/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}

