package com.example.amogh.opencvtry1;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.widget.TextView;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

public class EdgeDetection extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2{

    private static final String TAG = "EdgeDetection";
    private CameraBridgeViewBase cameraBridgeViewBase;

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
        setContentView(R.layout.activity_edge_detection);
        cameraBridgeViewBase = (CameraBridgeViewBase) findViewById(R.id.camera_view2);
        cameraBridgeViewBase.setVisibility(SurfaceView.VISIBLE);
        cameraBridgeViewBase.setCvCameraViewListener(this);

//        setContentView(R.layout.activity_edge_detection);
    }

    @Override
    public void onPause() {
        super.onPause();
        if (cameraBridgeViewBase != null)
            cameraBridgeViewBase.disableView();
    }

    @Override
    public void onResume(){
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_1_0, this, baseLoaderCallback);
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            baseLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }

    public void onDestroy() {
        super.onDestroy();
        if (cameraBridgeViewBase != null)
            cameraBridgeViewBase.disableView();
    }

    @Override
    public void onCameraViewStarted(int width, int height) {

    }

    @Override
    public void onCameraViewStopped() {

    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        Mat edges = inputFrame.gray();
        Mat src_im=inputFrame.rgba();
//        cartoonify(edges.getNativeObjAddr(),src_im.getNativeObjAddr());
//        detectEdges(edges.getNativeObjAddr());
        float[] array3;
        //amogh maybe need to define the size
//        array3=arrayOutput();
        array3=hogOutput(edges.getNativeObjAddr());
        Log.d("tag",Float.toString(array3[0])+Float.toString(array3[1999]));
//        hogOutput(src_im.getNativeObjAddr());
        return edges;

    }
//    public native float[] arrayOutput();
    public native float[] hogOutput(long matGray);
    public native void detectEdges(long matGray);
//    public native void cartoonify(long matGray,long matSrc);
}
