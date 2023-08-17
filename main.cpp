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

int main(int argc, char *argv[])
{
    qmlRegisterType<ContactsModel>("Contacts",1,0,"ContactsModel");
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    ContactsList *contactsList=new ContactsList(&app);
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

