//
// Created by AMOGH on 3/27/2018.
//

#include <jni.h>
#include "tryadd.h"


    int tryAdd::tryFunc()
    {
        int a=2;
        int b=3;
        return a+b;
    }
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_amogh_opencvtry1_EdgeDetection_tryAdd(JNIEnv *env, jobject instance) {

    tryAdd a;
    int b=a.tryFunc();
    return 1;
}