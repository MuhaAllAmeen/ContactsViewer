package com.example.appContacts;

import org.qtproject.qt.android.bindings.QtActivity;
import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ContentProviderOperation;
import android.content.ContentProviderResult;
import android.content.ContentResolver;
import android.content.pm.PackageManager;
import android.database.ContentObserver;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.ContactsContract;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import com.google.gson.Gson;
import java.util.ArrayList;
import java.util.concurrent.Callable;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class MyObserver extends ContentObserver {
    public MyObserver(Handler handler,MainActivity main) {
        super(handler);
        this.main = main;
    }
    private MainActivity main;
    @Override
    public  void onChange(boolean selfChange, Uri uri) {
        super.onChange(selfChange,uri);

                Log.d("change","change");
                main.updateContacts();
    }
}

public class MainActivity extends QtActivity {
    ArrayList<ContactsModel> arrayList = new ArrayList<ContactsModel>();
    String[] contactPermission;
    MyObserver observer;
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("","hello there");
        checkPermission();
        contactPermission = new String[]{Manifest.permission.WRITE_CONTACTS};
        Handler handler = new Handler(getApplicationContext().getMainLooper());
        observer = new MyObserver(handler,this);
        getContentResolver().registerContentObserver(ContactsContract.Contacts.CONTENT_URI, true, observer);
//        if (isWriteContactPermissionEnabled()){
//            for (int i=0;i<100;i++) {
//                saveContacts();
//            }
//        }
//        else{
//            requestWriteContactPermission();
//        }
    }
    public void onDestroy() {
        super.onDestroy();
        getContentResolver().unregisterContentObserver(observer);
    }

    public String checkPermission() {
        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.READ_CONTACTS)
        != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(MainActivity.this
            ,new String[]{Manifest.permission.READ_CONTACTS},100);
            return "Permission Denied";
        } else {
            return "Permission Granted";
//            return loadContacts();
//            loadContacts();
        }

    }

    public String loadContacts() throws InterruptedException {
        arrayList.clear();
        Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI,null,null,null,ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME);
        Uri mainUri= ContactsContract.Contacts.CONTENT_URI;
        if (cursor.getCount()>0){
            while(cursor.moveToNext()){
                @SuppressLint("Range") String id = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID));
                @SuppressLint("Range") String name = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
//                Log.d("name",name);
                Thread t = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        Uri uriPhone = ContactsContract.CommonDataKinds.Phone.CONTENT_URI;
                        String selection = ContactsContract.CommonDataKinds.Phone.CONTACT_ID + "=?";
                        Cursor phoneCursor = getContentResolver().query(uriPhone,null,selection,new String[]{id},null);

                        if (phoneCursor.moveToNext()){
                            @SuppressLint("Range") String number = phoneCursor.getString(phoneCursor.getColumnIndex(
                                    ContactsContract.CommonDataKinds.Phone.NUMBER
                            ));
//                    Log.d("number",number);
                            ContactsModel model = new ContactsModel();
                            model.setName(name);
                            model.setNumber(number);
                            arrayList.add(model);

                        }
                        phoneCursor.close();
                    }
                });
                t.start();
                t.join();
            }
        }
        cursor.close();
        Gson gson = new Gson();
        String json = gson.toJson(arrayList);
//        Log.d("json",json);
        return json;
    }
    public native void sendUpdatedContacts();

    public void updateContacts(){
        sendUpdatedContacts();
        Toast.makeText(MainActivity.this, "Contacts Updated", Toast.LENGTH_SHORT).show();
    }

    private void saveContacts() {
        Log.d("save","called");
        ArrayList<ContentProviderOperation> cpo = new ArrayList<>();
        int rawContactId = cpo.size();

        cpo.add(ContentProviderOperation.newInsert(
                        ContactsContract.RawContacts.CONTENT_URI)
                .withValue(ContactsContract.RawContacts.ACCOUNT_TYPE, null)
                .withValue(ContactsContract.RawContacts.ACCOUNT_NAME, null)
                .build());

        cpo.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
                .withValueBackReference(ContactsContract.RawContacts.Data.RAW_CONTACT_ID, 0)
                .withValue(ContactsContract.RawContacts.Data.MIMETYPE, ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE)
                .withValue(ContactsContract.CommonDataKinds.StructuredName.DISPLAY_NAME, "firsttName")
                .build());

        cpo.add(ContentProviderOperation
                .newInsert(ContactsContract.Data.CONTENT_URI)
                        .withValueBackReference(ContactsContract.Data.RAW_CONTACT_ID,0)
                .withValue(ContactsContract.Data.MIMETYPE,
                ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE)
                        .withValue(ContactsContract.CommonDataKinds.Phone.NUMBER, "+971")
                .withValue(ContactsContract.CommonDataKinds.Phone.TYPE,
                        ContactsContract.CommonDataKinds.Phone.TYPE_MOBILE)
                .build());
        cpo.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
                        .withValueBackReference(ContactsContract.RawContacts.Data.RAW_CONTACT_ID, rawContactId)
                        .withValue(ContactsContract.RawContacts.Data.MIMETYPE,ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE)
                        .withValue(ContactsContract.CommonDataKinds.Phone.NUMBER,"+06")
                        .withValue(ContactsContract. CommonDataKinds.Phone.TYPE, ContactsContract.CommonDataKinds.Phone.TYPE_HOME)
                        .build());
        try {
            ContentProviderResult[] results = getContentResolver().applyBatch(ContactsContract.AUTHORITY, cpo);

            Log.d("saved", "saveContact: Saved..."+results+cpo);
        } catch (Exception e) {
            e.printStackTrace();
            Log.d("notsaved:", "saveContact: " + e.getMessage());
            Toast.makeText(this, "" + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }
    private boolean isWriteContactPermissionEnabled () {
        boolean result = ContextCompat.checkSelfPermission
                (this, Manifest.permission.WRITE_CONTACTS) ==(PackageManager.PERMISSION_GRANTED);
        return result;
    }
        private void requestWriteContactPermission(){
            ActivityCompat.requestPermissions ( this,contactPermission, 100);
        }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 100 && grantResults.length>0 && grantResults[0]==PackageManager.PERMISSION_GRANTED){
            try {
                loadContacts();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
//        if (grantResults. length > 0) {
//            if (requestCode == 100){
//                boolean haveWriteContactPermission = grantResults[0] == PackageManager.PERMISSION_GRANTED;
//                if (haveWriteContactPermission) {
//                    saveContacts();
//                }
//                else{
//                    Toast.makeText (this,"Permission denied...", Toast. LENGTH_SHORT).show();
//                }
//            }
//        }
        else{
            Toast.makeText(MainActivity.this, "Permission Denied", Toast.LENGTH_SHORT).show();
            checkPermission();
        }
    }

}
