package no.nordicsemi.android.nrftoolbox.template.savefile;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Environment;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;


public class SaveFileManager {

    private Context mContext;

    FileWriter mFileWriter;
    private long mTimeInitialMillis;
    private double mTimeSeconds;


    public SaveFileManager(Context context) {

        mContext = context;

        Log.v("SaveFileManager","HOLI");

        //Request user permission:
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!(ContextCompat.checkSelfPermission(mContext, android.Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED)) {
                ActivityCompat.requestPermissions((Activity) mContext, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
            }
        }

    }

    public void createFile(){
        if(mFileWriter == null) {
            try {

                // Get the base folder to save the file:
                File mBaseFolder;

                if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
                    mBaseFolder = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getAbsolutePath() + "/DNAamplifier");
                } else {
                    mBaseFolder = mContext.getFilesDir();
                }

                // Create the directory if necessary:
                if (!mBaseFolder.exists()) {
                    mBaseFolder.mkdirs();
                }

                // Create the mFileWriter object:
                String mFileName = (new SimpleDateFormat("yyyyMMdd_HHmmss", Locale.getDefault())).format(new Date()) + ".txt";
                mFileWriter = new FileWriter(mBaseFolder + "/" + mFileName);

                // Write the fist line (must be the name columns of the data to be written):
                mFileWriter.write("timeWrite,index,time,mosfetBefore,mosfetAfter,lightBefore,lightAfter\r\n");

                // Get the initial time
                mTimeInitialMillis = Calendar.getInstance().getTimeInMillis();
                mTimeSeconds = (Calendar.getInstance().getTimeInMillis() - mTimeInitialMillis) / 1000.0;
                Log.v("SaveFileManager", "mTimeInitialMillis: " + mTimeInitialMillis);
                Log.v("SaveFileManager", "mTimeSeconds: " + mTimeSeconds);

            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }
    }


    public void writeLine(int[] dataArray){
        if (mFileWriter != null) {
            try {
                mTimeSeconds = (Calendar.getInstance().getTimeInMillis() - mTimeInitialMillis) / 1000.0;
                mFileWriter.write(String.format(Locale.ENGLISH,"%.1f", mTimeSeconds));
                mFileWriter.write(",");
                for(int i = 0; i < (dataArray.length-1); i++){
                    mFileWriter.write(String.valueOf(dataArray[i]));
                    mFileWriter.write(",");
                }
                mFileWriter.write(String.valueOf(dataArray[dataArray.length-1]));
                mFileWriter.write("\r\n");
            } catch (IOException ioe){
                ioe.printStackTrace();
            }
        }
    }


    public void closeFile(){
        if(mFileWriter != null) {
            try {
                mFileWriter.flush();
                mFileWriter.close();
                mFileWriter = null;
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }
    }

}