#include <jni.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/ml.hpp>
#include "tryadd.h"

extern "C"
JNIEXPORT void JNICALL Java_com_example_amogh_opencvtry1_EdgeDetection_detectEdges(JNIEnv*, jobject, jlong gray){
    cv::Mat& edges= *(cv::Mat *) gray;
    cv::Canny(edges,edges,50,250);
//    cv::ml::SVM::create();

}

//extern "C"
//JNIEXPORT void JNICALL Java_com_example_amogh_opencvtry1_EdgeDetection_cartoonify(
//        JNIEnv*, jobject /* this */,
//        jlong gray, jlong rgb) {
//    const int MEDIAN_BLUR_FILTER_SIZE = 7;
//    const int LAPLACIAN_FILTER_SIZE = 5;
//    const int EDGES_THRESHOLD = 30;
//    int repetitions = 5;
//    int kSize = 9;
//    double sigmaColor = 9;
//    double sigmaSpace = 7;
//
//    cv::Mat& edges = *(cv::Mat *) gray;
//    cv::medianBlur(edges, edges, MEDIAN_BLUR_FILTER_SIZE);
//    cv::Laplacian(edges, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
//    cv::Mat mask; cv::threshold(edges, mask, EDGES_THRESHOLD, 255, CV_THRESH_BINARY_INV);
//
//    cv::Mat& src = *(cv::Mat *) rgb;
//    cv::cvtColor(src,src,CV_RGBA2RGB);
//    cv::Size size = src.size();
//    cv::Size smallSize;
//    smallSize.width = size.width/4;
//    smallSize.height = size.height/4;
//    cv::Mat smallImg = cv::Mat(smallSize, CV_8UC3);
//    resize(src, smallImg, smallSize, 0, 0, CV_INTER_LINEAR);
//
//    cv::Mat tmp = cv::Mat(smallSize, CV_8UC3);
//
//    for(int i=0; i<repetitions;i++){
//        bilateralFilter(smallImg, tmp, kSize, sigmaColor, sigmaSpace);
//        bilateralFilter(tmp, smallImg, kSize, sigmaColor, sigmaSpace);
//    }
//
//    cv::Mat bigImg;
//    resize(smallImg, bigImg, size, 0, 0, CV_INTER_LINEAR);
//    cv::Mat dst; bigImg.copyTo(dst,mask);
//    cv::medianBlur(dst, src, MEDIAN_BLUR_FILTER_SIZE-4);
//}
extern "C"
JNIEXPORT jfloatArray JNICALL Java_com_example_amogh_opencvtry1_EdgeDetection_hogOutput(JNIEnv* env, jobject, jlong gray){
//    cv::Mat& edges= *(cv::Mat *) gray;
    cv::Mat& src=*(cv::Mat *) gray;
//    cv::cvtColor(src,src,CV_RGBA2RGB);
    cv::Size size=src.size();
//    cv::Size smallSize;
//    smallSize.width=100;
//    smallSize.height=size.height/4;
//    cv::Mat smallImg=cv::Mat(smallSize, CV_8UC3);
    cv::HOGDescriptor hog(size,cv::Size(8,8),cv::Size(4,4),cv::Size(8,8),9,1,-1,0,0.2,0,64,1);
    std::vector<float> descriptors;
    hog.compute(src,descriptors);
    int desc_size=descriptors.size();
    jfloatArray result=env->NewFloatArray(desc_size);
    if(result==NULL){
        return NULL; //out of memory thrown
    }
    jfloat array2[desc_size];
    for(int i=0;i<2000;i++){
        array2[i]=descriptors[i];
    }

    env->SetFloatArrayRegion(result,0,2000,array2);
//    jfloat array1[3];
//    array1[0]=(float)desc_size;
//    array1[1]=3.0f;
//    array1[2]=4.0f;
//    env->SetFloatArrayRegion(result,0,3,array1);
    return result;
}

/* trial function to test passing of array to Java, logs first two elements of an array
extern "C"
JNIEXPORT jfloatArray JNICALL Java_com_example_amogh_opencvtry1_EdgeDetection_arrayOutput(JNIEnv* env, jobject){
//    float arr[5]={1.,4.,5.,6.,7.};
    jfloatArray result=env->NewFloatArray(3);
    if(result==NULL){
        return NULL; //out of memory thrown
    }
    jfloat array1[3];
    array1[0]=2.0f;
    array1[1]=3.0f;
    array1[2]=4.0f;
    env->SetFloatArrayRegion(result,0,3,array1);
    return result;
}
*/

extern "C"
JNIEXPORT jstring JNICALL Java_com_example_amogh_opencvtry1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_amogh_opencvtry1_EdgeDetection_tryAdd(JNIEnv *env, jobject instance) {

    tryAdd a;
//    int b=a.tryFunc();
    return 1;
}