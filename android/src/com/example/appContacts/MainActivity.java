package com.example.appContacts;

import org.json.JSONArray;
import org.json.JSONObject;
import org.qtproject.qt.android.bindings.QtActivity;
import android.Manifest;
import android.annotation.SuppressLint;
import android.content.pm.PackageManager;
import android.database.ContentObserver;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.ContactsContract;
import android.util.Log;
import android.widget.Toast;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import java.util.ArrayList;
import java.util.HashMap;
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
        try {
            main.loadContacts();
        } catch (InterruptedException e) {throw new RuntimeException(e);
        }
    }
}

public class MainActivity extends QtActivity {
    long ptr;
    long lastUpdatedTime=0;
    long lastDeletedTime = 0;
    boolean firstPass=true;
    JSONArray jsonArray;
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
        }

    }

    public long setPointer(long pointer){
        ptr = pointer;
        Log.d("ptr", String.valueOf(ptr));
        return ptr;
    }
    @SuppressLint("Range")
    public void loadContacts() throws InterruptedException {
        jsonArray = new JSONArray();
        if(!firstPass) {
            Cursor deleteCursor = getContentResolver().query(ContactsContract.DeletedContacts.CONTENT_URI, new String[]{ContactsContract.DeletedContacts.CONTACT_ID}, ContactsContract.DeletedContacts.CONTACT_DELETED_TIMESTAMP + " > ?", new String[]{String.valueOf(lastDeletedTime)}, null);

            if (deleteCursor.getCount() > 0) {
                Log.d("delcount", String.valueOf(deleteCursor.getCount()));
                String id = "";
                ArrayList<String> deleteIDs = new ArrayList<>();
                while (deleteCursor.moveToNext()) {
                    id = deleteCursor.getString(deleteCursor.getColumnIndex(ContactsContract.DeletedContacts.CONTACT_ID));
                    Log.d("Deleted-id", id);
                    deleteIDs.add(id);
                }
                JSONArray delJsonArray = new JSONArray(deleteIDs);
                Log.d("deljson",delJsonArray.toString());
                sendDeletedIDs(delJsonArray.toString(), ptr);
            }
            deleteCursor.close();
        }
        lastDeletedTime= System.currentTimeMillis();

        Cursor cursor = getContentResolver().query(ContactsContract.Contacts.CONTENT_URI,null,ContactsContract.Contacts.CONTACT_LAST_UPDATED_TIMESTAMP + " > ?",new String[] {String.valueOf(lastUpdatedTime)},ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME);
        if (cursor.getCount()>0){
            Log.d("count", String.valueOf(cursor.getCount()));
            while(cursor.moveToNext()){
                @SuppressLint("Range") String id = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID));
                @SuppressLint("Range") String name = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
//                Log.d("name",name);
                Uri uriPhone = ContactsContract.CommonDataKinds.Phone.CONTENT_URI;
                String selection = ContactsContract.CommonDataKinds.Phone.CONTACT_ID + " = ?";
                Cursor phoneCursor = getContentResolver().query(uriPhone,null,selection,new String[]{id},null);

                if (phoneCursor.moveToNext()){
                    @SuppressLint("Range") String number = phoneCursor.getString(phoneCursor.getColumnIndex(
                            ContactsContract.CommonDataKinds.Phone.NUMBER
                    ));
//                    Log.d("number",number);
                    HashMap<String, String> contactsMap = new HashMap<>();
                    contactsMap.put("number",number);
                    contactsMap.put("name",name);
                    contactsMap.put("id",id);
                    JSONObject jsonObject = new JSONObject(contactsMap);
                    jsonArray.put(jsonObject);
                }
                phoneCursor.close();
            }
//                    Log.d("json",jsonArray.toString());
            sendUpdatedContacts(jsonArray.toString(), ptr, firstPass);
            firstPass= false;
            lastUpdatedTime = System.currentTimeMillis();
//                    Log.d("json",jsonArray.toString());
        }
        cursor.close();
    }
    @SuppressLint("Range")
    
    public native void sendUpdatedContacts(String contacts, long ptr, boolean firstPass);
    public native void sendDeletedIDs(String ids, long ptr);




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

        else{
            Toast.makeText(MainActivity.this, "Permission Denied", Toast.LENGTH_SHORT).show();
            checkPermission();
        }
    }

}
