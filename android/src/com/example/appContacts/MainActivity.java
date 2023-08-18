package com.example.appContacts;

import org.qtproject.qt.android.bindings.QtActivity;
import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.pm.PackageManager;
import android.database.ContentObserver;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.provider.ContactsContract;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import com.google.gson.Gson;
import java.util.ArrayList;

class MyObserver extends ContentObserver {
    public MyObserver(Handler handler,MainActivity main) {

        super(handler);
        this.main = main;
    }
    private MainActivity main;
//    @Override
//    public void onChange(boolean selfChange) {
//
//        Log.d("change","change");
//        super.onChange(selfChange);
//        main.updateContacts();
//    }

    @Override
    public  void onChange(boolean selfChange, Uri uri) {
        super.onChange(selfChange,uri);
        // do s.th.
        // depending on the handler you might be on the UI
        // thread, so be cautious!
        Log.d("change","change");
        main.updateContacts();
    }
}

public class MainActivity extends QtActivity {
    ArrayList<ContactsModel> arrayList = new ArrayList<ContactsModel>();

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("","hello there");
        checkPermission();
        Handler handler = new Handler(getApplicationContext().getMainLooper());
        MyObserver observer = new MyObserver(handler,this);
        getContentResolver().registerContentObserver(ContactsContract.Contacts.CONTENT_URI, true, observer);
    }

    public String checkPermission() {
        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.READ_CONTACTS)
        != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(MainActivity.this
            ,new String[]{Manifest.permission.READ_CONTACTS},100);
            return "Permission Denied";
        } else {
            return loadContacts();
//            loadContacts();
        }
    }

    public String loadContacts(){
        arrayList.clear();
        Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI,null,null,null,ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME);
        Uri mainUri= ContactsContract.Contacts.CONTENT_URI;
        if (cursor.getCount()>0){
            while(cursor.moveToNext()){
                @SuppressLint("Range") String id = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID));
                @SuppressLint("Range") String name = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
//                Log.d("name",name);
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
        }
        cursor.close();
        Gson gson = new Gson();
        String json = gson.toJson(arrayList);
//        Log.d("json",json);
//        return arrayList;
        return json;
    }
    public native void sendUpdatedContacts(String Updatedcontacts);

    public void updateContacts(){
        Toast.makeText(MainActivity.this, "Contacts Updated", Toast.LENGTH_SHORT).show();
        sendUpdatedContacts(loadContacts());
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 100 && grantResults.length>0 && grantResults[0]==PackageManager.PERMISSION_GRANTED){
            loadContacts();
        }
        else{
            Toast.makeText(MainActivity.this, "Permission Denied", Toast.LENGTH_SHORT).show();
            checkPermission();
        }
    }

}
