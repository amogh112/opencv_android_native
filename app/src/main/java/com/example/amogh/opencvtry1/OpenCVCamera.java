package com.example.amogh.opencvtry1;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.graphics.Rect;
import android.graphics.Bitmap;
import android.widget.TextView;
import android.util.JsonWriter;

import org.json.JSONException;
import org.json.JSONObject;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.CvException;
import org.opencv.core.Mat;
import org.opencv.android.Utils;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.imgproc.Imgproc;
import com.tzutalin.dlib.Constants;  //amogh added for dlib
import com.tzutalin.dlib.FaceDet; //amogh added for dlib
import com.tzutalin.dlib.VisionDetRet; //amogh added for dlib

import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.List;


public class OpenCVCamera extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2 {

    private static final String TAG = "OpenCVCamera";
    private CameraBridgeViewBase cameraBridgeViewBase;
    private Context mContext;
    private FaceDet mFaceDet;


    private BaseLoaderCallback baseLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                    cameraBridgeViewBase.enableView();
                    break;
                default:
                    super.onManagerConnected(status);
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_open_cvcamera);
        cameraBridgeViewBase = (CameraBridgeViewBase) findViewById(R.id.camera_view);
        cameraBridgeViewBase.setVisibility(SurfaceView.VISIBLE);
        cameraBridgeViewBase.setCvCameraViewListener(this);//need to see
        Log.d("landmarks_d", "status of file _____"+String.valueOf(new File(Constants.getFaceShapeModelPath()).exists())+"_____"+(Constants.getFaceShapeModelPath()));
        mFaceDet = new FaceDet(Constants.getFaceShapeModelPath());
        mContext = getApplicationContext();
    }

    @Override
    public void onResume() {
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_1_0, this, baseLoaderCallback);
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            baseLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }

    @Override
    public void onCameraViewStarted(int width, int height) {

    }

    @Override
    public void onCameraViewStopped() {

    }

//    public void writeJSON(){ //writing a json
//        JSONObject object=new JSONObject();
//        try{
//            object.put();
//
//        } catch(JSONException e){
//            e.printStackTrace();
//        }
//    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {

        Mat tmp = inputFrame.rgba();
        //Drawing a Circle(not necessary, for debugging only)
//        Imgproc.circle (
//                tmp,                 //Matrix obj of the image
//                new Point(10, 10),    //Center of the circle
//                10,                    //Radius
//                new Scalar(0, 0, 255),  //Scalar object for color
//                10                      //Thickness of the circle
//        );
        Bitmap bmp = null;
        try {
            bmp = Bitmap.createBitmap(tmp.cols(), tmp.rows(), Bitmap.Config.ARGB_8888);
            Utils.matToBitmap(tmp, bmp);
        } catch (CvException e) {
            Log.d("Exception", e.getMessage());
        }
//        Log.d("Exception", "status of file _____"+String.valueOf(new File(Constants.getFaceShapeModelPath()).exists())+"_____"+(Constants.getFaceShapeModelPath())); //checking if the landmark file exists
        if (!new File(Constants.getFaceShapeModelPath()).exists()) {
            FileUtils.copyFileFromRawToOthers(mContext, R.raw.shape_predictor_68_face_landmarks, Constants.getFaceShapeModelPath());
        }

        List<VisionDetRet> results;
        synchronized (OpenCVCamera.this) {
            results = mFaceDet.detect(bmp);
        }
        if (results != null) {
//            Log.d("Exception", "_______face found____number____"+results.size()+"____"+String.valueOf((endTime - startTime) / 1000f));
            for (final VisionDetRet ret : results) {
                float resizeRatio = 1.0f;
                Log.d("Exception", "________rectangle found________");
                Rect bounds = new Rect();
                bounds.left = (int) (ret.getLeft() * resizeRatio);
                bounds.top = (int) (ret.getTop() * resizeRatio);
                bounds.right = (int) (ret.getRight() * resizeRatio);
                bounds.bottom = (int) (ret.getBottom() * resizeRatio);
                Imgproc.rectangle(tmp,new Point((ret.getLeft() * resizeRatio),(ret.getBottom() * resizeRatio)),new Point((ret.getRight() * resizeRatio),(ret.getTop() * resizeRatio)),new Scalar(0,0,255));
                ArrayList<android.graphics.Point> landmarks = ret.getFaceLandmarks();
//                Log.d("Exception","landmarks are ____"+landmarks.size());
                for (android.graphics.Point point : landmarks) {
//                    Log.d("Exception","Point found ");
                    float pointX = (point.x * resizeRatio);
                    float pointY = (point.y * resizeRatio);
                    Imgproc.circle(tmp,new Point(pointX,pointY),2,new Scalar(0,0,255),4);
                }
//                Log.d("Exception", "_______face found_____left___"+String.valueOf(bounds.left)+"_____right_____"+String.valueOf(bounds.right));
            }
        }
        return tmp;
    }
}
