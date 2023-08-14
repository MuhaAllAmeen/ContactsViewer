#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QDebug>
#include <jni.h>
#include <QJniObject>
#include <jni.h>
#include <QDebug>

//#ifdef __cplusplus
//extern "C" {
//#endif
//JNIEXPORT void JNICALL
//Java_com_example_appContacts_MainActivity_myMethod(JNIEnv *env, jobject, jstring
//                                                                             jstr) {
//    const char *cstr = env->GetStringUTFChars(jstr, nullptr);
//    QString qstr = QString::fromUtf8(cstr);
//    env->ReleaseStringUTFChars(jstr, cstr);
//    // Use the QString here
//    qDebug()<<"ss"<<qstr;
//}
//#ifdef __cplusplus
//}
//#endif
class ContactsModel
{
public:
    ContactsModel();
};

#endif // CONTACTSMODEL_H
