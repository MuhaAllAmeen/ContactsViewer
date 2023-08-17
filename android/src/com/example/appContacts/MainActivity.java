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
import android.os.Looper;
import android.provider.ContactsContract;
import android.util.Log;
import android.widget.Toast;
import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import com.google.gson.Gson;
import java.util.ArrayList;


public class MainActivity extends QtActivity {

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("","hello there");
        checkPermission();
    }

    public String checkPermission() {
        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.READ_CONTACTS)
        != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(MainActivity.this
            ,new String[]{Manifest.permission.READ_CONTACTS},100);
            return "Permission Denied";
        } else {
            return "Permission Granted";
//            loadContacts();
        }
    }

    public String loadContacts(){
        ArrayList<ContactsModel> arrayList = new ArrayList<ContactsModel>();
        Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI,null,null,null,ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME);
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
//        return nameList;
//        myMethod2(json);
//        thread();
        return json;
    }

    public int sumOfNumbers(int a, int b){
        return a+b;
    }
    public static String myMethod(String names){
        Log.d("Java","called");
        return names;
    }
    public static void checkMyMethod(String names){
        myMethod2(myMethod(names));
    }
        public static native void myMethod2(String names);

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
