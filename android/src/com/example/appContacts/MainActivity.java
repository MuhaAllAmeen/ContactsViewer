package com.example.appContacts;

import org.qtproject.qt.android.bindings.QtActivity;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.util.Log;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.util.ArrayList;

public class MainActivity extends QtActivity {
    ArrayList<ContactsModel> arrayList = new ArrayList<ContactsModel>();
    ArrayList<String> nameList = new ArrayList<String>();
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("","hello there");
        checkPermission();
    }

    public void checkPermission() {
        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.READ_CONTACTS)
        != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(MainActivity.this
            ,new String[]{Manifest.permission.READ_CONTACTS},100);
        } else {
            loadContacts();
        }
    }

    public ArrayList<ContactsModel> loadContacts(){
        Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI,null,null,null,ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME);
        if (cursor.getCount()>0){
            while(cursor.moveToNext()){
                @SuppressLint("Range") String id = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID));
                @SuppressLint("Range") String name = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
                Log.d("name",name);
                nameList.add(name);
                Uri uriPhone = ContactsContract.CommonDataKinds.Phone.CONTENT_URI;
                String selection = ContactsContract.CommonDataKinds.Phone.CONTACT_ID + "=?";
                Cursor phoneCursor = getContentResolver().query(uriPhone,null,selection,new String[]{id},null);
                if (phoneCursor.moveToNext()){
                    @SuppressLint("Range") String number = phoneCursor.getString(phoneCursor.getColumnIndex(
                            ContactsContract.CommonDataKinds.Phone.NUMBER
                    ));
                    Log.d("number",number);
                    ContactsModel model = new ContactsModel();
                    model.setName(name);
                    model.setNumber(number);
                    arrayList.add(model);
                }
                phoneCursor.close();
            }
        }
        for (int i=0; i<arrayList.size();i++){
            Log.d("arrayLits", String.valueOf(arrayList.get(i).name));
            Log.d("num",Integer.toString(i));
        }
//        Log.d("arrayList", String.valueOf(arrayList.size()));
        cursor.close();
        return arrayList;
//        return nameList;
    }

    public int sumOfNumbers(int a, int b){
        return a+b;
    }
    public String myMethod(String names){
        Log.d("Java","called");
        return names;
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
