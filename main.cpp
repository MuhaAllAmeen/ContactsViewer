#include "contactsmodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJniObject>
#include <jni.h>
#define JNIIMPORT
#define JNIEXPORT  __attribute__ ((visibility ("default")))
#define JNICALL

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL
Java_com_example_appContacts_MainActivity_myMethod(JNIEnv *env, jobject obj, jstring
                                                                          jstr) {
    qDebug() << "JNI method called";
    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
    QString qstr = QString::fromUtf8(cstr);
    env->ReleaseStringUTFChars(jstr, cstr);
    // Use the QString here
    qDebug()<<"ss"<<qstr;
}
#ifdef __cplusplus
}
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QJniObject javaClass = QNativeInterface::QAndroidApplication::context();
    jint sumOfNums = javaClass.callMethod<jint>("sumOfNumbers","(II)I",5, 5);

    JNIEXPORT void JNICALL
    Java_com_example_appContacts_MainActivity_myMethod(JNIEnv *env, jobject obj, jstring
                                                                                      jstr);
    QJniObject arrayList = javaClass.callObjectMethod("loadContacts","()Ljava/util/ArrayList;");
    qDebug()<<"hh"<<arrayList.toString();
    for (int i=0; i<4;i++){

    }
    qDebug()<<sumOfNums;
//    JNIEXPORT void JNICALL
//    Java_com_example_appContacts_MainActivity_myMethod(JNIEnv *env, jobject, jstring
//                                                                            jstr);
    const QUrl url(u"qrc:/Contacts/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

