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


#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jstring JNICALL
Java_com_example_appContacts_MainActivity_myMethod2(JNIEnv *env, jobject obj, jstring
                                                                          jstr) {
    JavaVM *jvm;
    env->GetJavaVM(&jvm);
    qDebug() << "JNI method called";
    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
    QString qstr = QString::fromUtf8(cstr);
    env->ReleaseStringUTFChars(jstr, cstr);
    // Use the QString here
    qDebug()<<"ss"<<qstr;
    return env->NewStringUTF("Hello World! From native code!");
}
#ifdef __cplusplus
}
#endif

int main(int argc, char *argv[])
{
    qmlRegisterType<ContactsModel>("Contacts",1,0,"ContactsModel");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    ContactsList contactsList;
    engine.rootContext()->setContextProperty(QStringLiteral("contactsList"),&contactsList);
    qmlRegisterUncreatableType<ContactsList>("Contacts",1,0,"ContactsList",
                                             QStringLiteral("Contacts List Should not be created in Qml"));
    QJniEnvironment env;
//    QJniObject arrayList = javaClass.callObjectMethod("loadContacts","()Ljava/util/ArrayList;");
//    qDebug()<<"hh"<<arrayList.toString();
    const QUrl url(u"qrc:/Contacts/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

