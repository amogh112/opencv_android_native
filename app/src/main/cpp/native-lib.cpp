#include <jni.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>


extern "C"

JNIEXPORT void JNICALL Java_com_example_amogh_opencvtry1_EdgeDetection_detectEdges(JNIEnv*, jobject, jlong gray){
    cv::Mat& edges= *(cv::Mat *) gray;
    cv::Canny(edges,edges,50,250);
}
extern "C"

JNIEXPORT jstring JNICALL Java_com_example_amogh_opencvtry1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
