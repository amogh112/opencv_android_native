#include <jni.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/video.hpp>
#include <stdlib.h>
#include <android/log.h>
#include<sstream>
using namespace std;
using namespace cv;
extern "C"
JNIEXPORT void JNICALL Java_com_example_amogh_opencvtry1_EdgeDetection_detectEdges(JNIEnv*, jobject, jlong gray){
    cv::Mat& edges= *(cv::Mat *) gray;
    cv::Canny(edges,edges,50,250);
//    cv::ml::SVM::create();

}

//}
extern "C"
JNIEXPORT jfloatArray JNICALL Java_com_example_amogh_opencvtry1_EdgeDetection_hogOutput(JNIEnv* env, jobject, jlong gray){
//    cv::Mat& edges= *(cv::Mat *) gray;
    Mat& src=*(cv::Mat *) gray; //this is not assigning the address, it means that src is an alias of the cv::Mat to which gray points (*gray.)
//    cv::cvtColor(src,src,CV_RGBA2RGB);
    Size size=src.size();
//    cv::Size smallSize;
//    smallSize.width=100;
//    smallSize.height=size.height/4;
//    cv::Mat smallImg=cv::Mat(smallSize, CV_8UC3);
    HOGDescriptor hog(size,cv::Size(8,8),cv::Size(4,4),cv::Size(8,8),9,1,-1,0,0.2,0,64,1);
    vector<float> descriptors;
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

extern "C"
JNIEXPORT jstring JNICALL Java_com_example_amogh_opencvtry1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
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
void similarityTransform(std::vector<cv::Point2f>& inPoints, std::vector<cv::Point2f>& outPoints, cv::Mat &tform)
{

    double s60 = sin(60 * M_PI / 180.0);
    double c60 = cos(60 * M_PI / 180.0);

    vector <cv::Point2f> inPts = inPoints;
    vector <cv::Point2f> outPts = outPoints;

    inPts.push_back(cv::Point2f(0,0));
    outPts.push_back(cv::Point2f(0,0));


    inPts[2].x =  c60 * (inPts[0].x - inPts[1].x) - s60 * (inPts[0].y - inPts[1].y) + inPts[1].x;
    inPts[2].y =  s60 * (inPts[0].x - inPts[1].x) + c60 * (inPts[0].y - inPts[1].y) + inPts[1].y;

    outPts[2].x =  c60 * (outPts[0].x - outPts[1].x) - s60 * (outPts[0].y - outPts[1].y) + outPts[1].x;
    outPts[2].y =  s60 * (outPts[0].x - outPts[1].x) + c60 * (outPts[0].y - outPts[1].y) + outPts[1].y;


    tform = cv::estimateRigidTransform(inPts, outPts, false);
}

// Calculate Delaunay triangles for set of points
// Returns the vector of indices of 3 points for each triangle
static void calculateDelaunayTriangles(Rect rect, vector<Point2f> &points, vector< vector<int> > &delaunayTri){

    // Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);
    __android_log_write(ANDROID_LOG_ERROR, "checkdelaunay", "Inside calcDelaunayTriangle, subdiv2d initiated");

    // Insert points into subdiv
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++){
        subdiv.insert(*it);
        __android_log_print(ANDROID_LOG_INFO, "checkdelaunay", "the points are %f %f", (*it).x,
                            (*it).y);}
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point2f> pt(3);
    vector<int> ind(3);
    __android_log_write(ANDROID_LOG_ERROR, "checkdelaunay", "going inside the loop");

    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
        pt[0] = Point2f(t[0], t[1]);
        pt[1] = Point2f(t[2], t[3]);
        pt[2] = Point2f(t[4], t[5 ]);
        __android_log_print(ANDROID_LOG_INFO, "checkdelaunay", "there are %d triangles the points of triangle are: %f %f %f %f %f %f",triangleList.size(),t[0],t[1],t[2],t[3],t[4],t[5]);


        if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])){
            for(int j = 0; j < 3; j++)
                for(size_t k = 0; k < points.size(); k++)
                    if(abs(pt[j].x - points[k].x) < 1.0 && abs(pt[j].y - points[k].y) < 1)
                        ind[j] = k; //basically the index of nearest landmark
//            __android_log_print(ANDROID_LOG_INFO, "newnew", "found truet %d", i);

            __android_log_print(ANDROID_LOG_INFO, "checkdelaunay", "landmarks forming triangle: %d %d %d",ind[0],ind[1],ind[1]);

            delaunayTri.push_back(ind);// delaunayTri(vector<vector<int>>)
        }

    }
}

// Apply affine transform calculated using srcTri and dstTri to src
void applyAffineTransform(Mat &warpImage, Mat &src, vector<Point2f> &srcTri, vector<Point2f> &dstTri)
{
    // Given a pair of triangles, find the affine transform.
    Mat warpMat = getAffineTransform( srcTri, dstTri );

    // Apply the Affine Transform just found to the src image
    warpAffine( src, warpImage, warpMat, warpImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}


// Warps and alpha blends triangular regions from img1 and img2 to img
void warpTriangle(Mat &img1, Mat &img2, vector<Point2f> t1, vector<Point2f> t2)
{
    // Find bounding rectangle for each triangle
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "beginning checkwarp");

    Rect r1 = boundingRect(t1);
    Rect r2 = boundingRect(t2);
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "t1 has: %f, %f,%f, %f,%f, %f", t1[0].x,t1[0].y,t1[1].x,t1[1].y,t1[2].x,t1[2].y);
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "t2 has: %f, %f,%f, %f,%f, %f", t2[0].x,t2[0].y,t2[1].x,t2[1].y,t2[2].x,t2[2].y);

    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "rect initialised r1 top left is: %f, %f", r1.tl().x,r1.tl().y);
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "rect initialised r1 bottom right is: %f, %f", r1.br().x,r1.br().y);
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "rect initialised r2 top left is: %f, %f", r2.tl().x,r2.tl().y);
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "rect initialised r2 bottom right is: %f, %f", r2.br().x,r2.br().y);

    // Offset points by left top corner of the respective rectangles
    vector<Point2f> t1Rect, t2Rect;
    vector<Point> t2RectInt;
    for(int i = 0; i < 3; i++)
    {
        //tRect.push_back( Point2f( t[i].x - r.x, t[i].y -  r.y) );
        t2RectInt.push_back( Point((int)(t2[i].x - r2.x), (int)(t2[i].y - r2.y)) ); // for fillConvexPoly
        __android_log_print(ANDROID_LOG_INFO, "checkwarp", "1. pushing in t2RectInt: %d, %d", (int)(t2[i].x - r2.x), (int)(t2[i].y - r2.y));
        t1Rect.push_back( Point2f( t1[i].x - r1.x, t1[i].y -  r1.y) );
        __android_log_print(ANDROID_LOG_INFO, "checkwarp", "2. pushing in t1Rect: %f, %f", t1[i].x - r1.x, t1[i].y -  r1.y);
        t2Rect.push_back( Point2f( t2[i].x - r2.x, t2[i].y - r2.y) );
        __android_log_print(ANDROID_LOG_INFO, "checkwarp", "3. pushing in t2Rect: %f, %f", t2[i].x - r2.x, t2[i].y - r2.y);

    }
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of pin and pout is %f %f",t1Rect[0].x,t2Rect[0].x);
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of pin and pout is %f %f",t1Rect[1].x,t2Rect[1].x);
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of pin and pout is %f %f",t1Rect[2].x,t2Rect[2].x);
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of pin and pout is %f %f",t1Rect[0].y,t2Rect[0].y);
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of pin and pout is %f %f",t1Rect[1].y,t2Rect[1].y);
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of pin and pout is %f %f",t1Rect[2].y,t2Rect[2].y);
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of pin and pout is %d %d",t2RectInt[0].x,t2Rect[0].y);
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of pin and pout is %d %d",t2RectInt[1].x,t2Rect[1].y);
//    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of pin and pout is %d %d",t2RectInt[2].x,t2Rect[2].y);

    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "new rect ready");

    // Get mask by filling triangle
    Mat mask = Mat::zeros(r2.height, r2.width, CV_32FC3);
    fillConvexPoly(mask, t2RectInt, Scalar(1.0, 1.0, 1.0), 16, 0);//mask is an image, and the polygon made by t2RectInt si going to be filled.
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "convexPoly filled");

    // Apply warpImage to small rectangular patches
    Mat img1Rect, img2Rect;
    img1(r1).copyTo(img1Rect);

    Mat warpImage = Mat::zeros(r2.height, r2.width, img1Rect.type());

    applyAffineTransform(warpImage, img1Rect, t1Rect, t2Rect);
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "rect affine transform applied");
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of warpImage is %d %d %d",warpImage.size[0],warpImage.size[1],warpImage.size[2]);
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "the size of warpImage is %d %d %d",mask.size[0],mask.size[1],mask.size[2]);

    // Copy triangular region of the rectangular patch to the output image
    multiply(warpImage,mask, warpImage);
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "mult1 done");

    multiply(img2(r2), Scalar(1.0,1.0,1.0) - mask, img2(r2));
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "mult2 done");

    img2(r2) = img2(r2) + warpImage;
    __android_log_print(ANDROID_LOG_INFO, "checkwarp", "finalimg calculated");

}

// Constrains points to be inside boundary
void constrainPoint(Point2f &p, Size sz)
{
    p.x = min(max( (double)p.x, 0.0), (double)(sz.width - 1));
    p.y = min(max( (double)p.y, 0.0), (double)(sz.height - 1));

}

//old function
// vector<Point2f> vectoriseLandmarks(jfloat landmarks[]){
//    vector<Point2f> lm;
//    for (int i=0;i<=134;i+=2) {
//        lm.push_back(Point2f(landmarks[i],landmarks[i+1]));
//    }
//    return lm;
//}

/* //old function, we are trying out the source code as it is.
void process(cv::Mat img, vector<Point2f> landmarksVec){
    int w=400;
    int h=400;
    img.convertTo(img,CV_32FC3,1/255.0);
    //defining the points for both the eyes.
    //amogh - not all things need to be defined again, only the ones in loop, change code later
    vector<Point2f> eyecornerDst, eyecornerSrc;
//    eyecornerDst.push_back(Point2f(36.f, h/3));
//    eyecornerDst.push_back(Point2f(76.f, h/3));
    eyecornerDst.push_back(Point2f( 0.3*w, h/3));
    eyecornerDst.push_back(Point2f( 0.7*w, h/3));

    eyecornerSrc.push_back(Point2f(0,0));
    eyecornerSrc.push_back(Point2f(0,0));
    // Space for normalized images and points.
    __android_log_write(ANDROID_LOG_ERROR, "newtry", "eye corners pushed");


    vector <Mat> imagesNorm; //amogh this should be outside the loop, stores things for all frames
    vector < vector <Point2f> > pointsNorm;//amogh this should be outside the loop, stores things for all frames
    // Space for average landmark points
    vector <Point2f> pointsAvg(landmarksVec.size());
    // 8 Boundary points for Delaunay Triangulation
    vector <Point2f> boundaryPts;
    boundaryPts.push_back(Point2f(0,0));
    boundaryPts.push_back(Point2f(w/2, 0));
    boundaryPts.push_back(Point2f(w-1,0));
    boundaryPts.push_back(Point2f(w-1, h/2));
    boundaryPts.push_back(Point2f(w-1, h-1));
    boundaryPts.push_back(Point2f(w/2, h-1));
    boundaryPts.push_back(Point2f(0, h-1));
    boundaryPts.push_back(Point2f(0, h/2));
    __android_log_write(ANDROID_LOG_ERROR, "newtry", "Boundary points pushed");

    // Warp images and trasnform landmarks to output coordinate system,
    // and find average of transformed landmarks.

    // The corners of the eyes are the landmarks number 36 and 45
        eyecornerSrc[0] = landmarksVec[36];
        eyecornerSrc[1] = landmarksVec[45];

        // Calculate similarity transform
        Mat tform;
        similarityTransform(eyecornerSrc, eyecornerDst, tform);
    __android_log_write(ANDROID_LOG_ERROR, "newtry", "Similarity transform obtained");

        // Apply similarity transform to input image and landmarks
        Mat newimg = Mat::zeros(h, w, CV_32FC3);
        warpAffine(img, newimg, tform, newimg.size());//this is to transform the image such that the eyes are aligned and in a smaller frame
    __android_log_write(ANDROID_LOG_ERROR, "newtry", "Warp affine done");

    transform( landmarksVec, landmarksVec, tform); //these are the transformed points when the eyes are aligned. and the size is smaller
    __android_log_write(ANDROID_LOG_ERROR, "newtry", "transformed!");

    //amogh add average points locations
         //example code
        // for ( size_t j = 0; j < points.size(); j++)
//    {
//        pointsAvg[j] += points[j] * ( 1.0 / numImages);
//    }
    pointsAvg=landmarksVec;
    // Append boundary points. Will be used in Delaunay Triangulation
    for ( size_t j = 0; j < boundaryPts.size(); j++)
    {
        pointsAvg.push_back(boundaryPts[j]); //amogh change
    }
    for ( size_t j = 0; j < boundaryPts.size(); j++)
    {
        landmarksVec.push_back(boundaryPts[j]); //amogh change
    }

    pointsNorm.push_back(landmarksVec);
    imagesNorm.push_back(newimg);
    //amogh add the boundary point to the average image landmarks also
    // Append boundary points to average points.
//    for ( size_t j = 0; j < boundaryPts.size(); j++)
//    {
//        pointsAvg.push_back(boundaryPts[j]);
//    }
    // Calculate Delaunay triangles
    Rect rect(0, 0, w, h);
    vector< vector<int> > dt;//contains the number id of the landmarks which form triangles
    calculateDelaunayTriangles(rect, pointsAvg, dt);//Delaunay triangles are calculated and stored in dt //amogh change
    __android_log_write(ANDROID_LOG_ERROR, "newtry", "Delaunay triangles calculated");

    // Space for output image
    Mat output = Mat::zeros(h, w, CV_32FC3);
    Size size(w,h);

    Mat newerimg = Mat::zeros(h, w, CV_32FC3);
    // Transform triangles one by one
    for(size_t j = 0; j < dt.size(); j++)
    {
        // Input and output points corresponding to jth triangle
        vector<Point2f> tin, tout;
        for(int k = 0; k < 3; k++)
        {
            Point2f pIn = pointsNorm[0][dt[j][k]]; //change
            constrainPoint(pIn, size);

            Point2f pOut = pointsAvg[dt[j][k]];
            constrainPoint(pOut,size);

            tin.push_back(pIn);
            tout.push_back(pOut);
        }
//        __android_log_print(ANDROID_LOG_INFO, "newtry", "the size of pin and pout is %f %f",tout[0].x,tout[0].y);
//        __android_log_print(ANDROID_LOG_INFO, "newtry", "the size of pin and pout is %f %f",tout[1].x,tout[1].y);
        __android_log_print(ANDROID_LOG_INFO, "newtry", "starting to try warp on triangle %d and the sizes are: %d,%d,%d,%d",j,imagesNorm[0].size[0],newerimg.size[0],tin.size(),tout.size());
        warpTriangle(imagesNorm[0], newerimg, tin, tout); //change will be warped for each triangle.
        __android_log_print(ANDROID_LOG_INFO, "newtry", "warp done on this triangle");

    }
    // Add image intensities for averaging
    output = output + newerimg;

}
*/

extern "C"
JNIEXPORT jfloatArray JNICALL Java_com_example_amogh_opencvtry1_OpenCVCamera_transferPointsToNative(JNIEnv* env, jobject self, jfloatArray input, jlong im){

    jfloat* landmarkPoint=env->GetFloatArrayElements(input,0);
    cv::Mat& src=*(cv::Mat *) im;
//    __android_log_print(ANDROID_LOG_INFO, "newnew", "the points are %f %f",landmarkPoint[0],landmarkPoint[1]);

    vector<Point2f> landmarksVec;
    for (int i=0;i<=134;i+=2) {
        landmarksVec.push_back(Point2f(landmarkPoint[i],landmarkPoint[i+1]));
    }
//    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the points are %f %f",landmarksVec[0].x,landmarksVec[1].y);
    __android_log_write(ANDROID_LOG_ERROR, "checkvec", "____Landmarks vectorised_____");
//        __android_log_print(ANDROID_LOG_INFO, "checkvec", "the points are %f %f",landmarksVec[0].x,landmarksVec[0].y );

    //source code begins
    int w = 112;
    int h = 112;
    vector<string> imageNames, ptsNames;
    vector<vector<Point2f> > allPoints;
    allPoints.push_back(landmarksVec);
    //debug point
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the points are %f %f",allPoints[0][0].x,allPoints[0][0].y);
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the points are %f %f",allPoints[0][45].x,allPoints[0][45].y);
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the points are %f %f",allPoints[0][67].x,allPoints[0][67].y);

    int n = allPoints[0].size();

    // Read images
    vector<Mat> images;
    src.convertTo(src,CV_32FC3,1/255.0);

    images.push_back(src);
    if(images.empty())
    {
        cout << "No images found " << endl;
        exit(EXIT_FAILURE);
    }

    int numImages = images.size();


    // Eye corners
    vector<Point2f> eyecornerDst, eyecornerSrc;
    eyecornerDst.push_back(Point2f( 0.3*w, h/3));
    eyecornerDst.push_back(Point2f( 0.7*w, h/3));

    eyecornerSrc.push_back(Point2f(0,0));
    eyecornerSrc.push_back(Point2f(0,0));

    // Space for normalized images and points.
    vector <Mat> imagesNorm;
    vector < vector <Point2f> > pointsNorm;

    // Space for average landmark points
    vector <Point2f> pointsAvg(allPoints[0].size());

    // 8 Boundary points for Delaunay Triangulation
    vector <Point2f> boundaryPts;
    boundaryPts.push_back(Point2f(0,0));
    boundaryPts.push_back(Point2f(w/2, 0));
    boundaryPts.push_back(Point2f(w-1,0));
    boundaryPts.push_back(Point2f(w-1, h/2));
    boundaryPts.push_back(Point2f(w-1, h-1));
    boundaryPts.push_back(Point2f(w/2, h-1));
    boundaryPts.push_back(Point2f(0, h-1));
    boundaryPts.push_back(Point2f(0, h/2));

    // Warp images and trasnform landmarks to output coordinate system,
    // and find average of transformed landmarks.

    for(size_t i = 0; i < images.size(); i++)
    {

        vector <Point2f> points = allPoints[i];

        // The corners of the eyes are the landmarks number 36 and 45
        eyecornerSrc[0] = allPoints[i][36];
        eyecornerSrc[1] = allPoints[i][45];

        // Calculate similarity transform
        Mat tform;
        similarityTransform(eyecornerSrc, eyecornerDst, tform);

        // Apply similarity transform to input image and landmarks
        Mat img = Mat::zeros(h, w, CV_32FC3);
        warpAffine(images[i], img, tform, img.size());
        transform( points, points, tform);

        // Calculate average landmark locations
        for ( size_t j = 0; j < points.size(); j++)
        {
            pointsAvg[j] += points[j] * ( 1.0 / numImages);
        }

        // Append boundary points. Will be used in Delaunay Triangulation
        for ( size_t j = 0; j < boundaryPts.size(); j++)
        {
            points.push_back(boundaryPts[j]);
        }

        pointsNorm.push_back(points);
        imagesNorm.push_back(img);


    }
    //debug point
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the normalised points are %f %f",pointsNorm[0][0].x,pointsNorm[0][0].y);
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the normalised points are %f %f",pointsNorm[0][45].x,pointsNorm[0][45].y);
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the normalised points are %f %f",pointsNorm[0][67].x,pointsNorm[0][67].y);


    // Append boundary points to average points.
    for ( size_t j = 0; j < boundaryPts.size(); j++)
    {
        pointsAvg.push_back(boundaryPts[j]);
    }
    //pushing average points
    float avgLandmarkPoint[]={19.982,37.762,20.220,46.945,21.262,55.963,22.500,65.110,25.268,74.055,30.317,82.012,36.878,88.921,44.220,94.616,52.994,96.524,61.951,95.024,69.689,89.811,76.976,83.457,83.207,75.774,87.061,66.884,89.354,57.433,91.146,48.091,92.409,38.512,25.555,33.061,29.470,28.037,35.933,25.860,42.750,26.445,49.116,28.787,61.915,28.659,68.494,26.433,75.299,25.805,81.854,27.957,85.896,32.774,54.982,35.415,54.567,41.585,54.232,47.902,53.848,54.476,47.335,58.390,50.518,59.274,53.872,60.280,57.518,59.372,60.927,58.470,33.000,37.000,37.061,35.146,41.720,35.037,46.079,37.518,41.622,38.207,37.030,38.360,64.805,37.268,68.970,34.854,73.762,34.988,78.000,37.000,73.841,38.195,69.244,38.146,38.841,68.500,44.152,66.884,49.756,65.732,53.591,66.701,57.530,65.909,63.500,67.238,69.494,68.750,63.537,74.738,57.616,77.537,53.421,78.073,49.195,77.488,43.848,74.634,40.878,69.104,49.591,69.067,53.524,69.591,57.506,69.232,67.274,69.317,57.567,72.274,53.494,72.848,49.518,72.183,0.000,0.000,56.000,0.000,111.000,0.000,111.000,56.000,111.000,111.000,56.000,111.000,0.000,111.000,0.000,56.000};
    pointsAvg.clear();
    for(int i=0; i<=150 ; i+=2) {
        pointsAvg.push_back(Point2f(avgLandmarkPoint[i],avgLandmarkPoint[i+1]));
    }
    // Calculate Delaunay triangles
    Rect rect(0, 0, w, h);
    vector< vector<int> > dt;
    calculateDelaunayTriangles(rect, pointsAvg, dt);

    //debug point
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the triangle points are %d %d",(dt[0][0]),(dt[0][0]));
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the triangle points are %d %d",(dt[0][1]),(dt[0][1]));
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the triangle points are %d %d",(dt[0][2]),(dt[0][2]));
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the triangle points are %d %d",(dt[45][0]),(dt[45][0]));
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the triangle points are %d %d",(dt[45][1]),(dt[45][1]));
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the triangle points are %d %d",(dt[45][2]),(dt[45][2]));
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the triangle points are %d %d",(dt[67][0]),(dt[67][0]));
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the triangle points are %d %d",(dt[67][1]),(dt[67][1]));
    __android_log_print(ANDROID_LOG_INFO, "checkfinal", "the triangle points are %d %d",(dt[67][2]),(dt[67][2]));


    // Space for output image
    Mat output = Mat::zeros(h, w, CV_32FC3);
    Size size(w,h);

    // Warp input images to average image landmarks

    for(size_t i = 0; i < numImages; i++)
    {
        Mat img = Mat::zeros(h, w, CV_32FC3);
        // Transform triangles one by one
        for(size_t j = 0; j < dt.size(); j++)
        {
            // Input and output points corresponding to jth triangle
            vector<Point2f> tin, tout;
            for(int k = 0; k < 3; k++)
            {
                Point2f pIn = pointsNorm[i][dt[j][k]];
                constrainPoint(pIn, size);

                Point2f pOut = pointsAvg[dt[j][k]];
                constrainPoint(pOut,size);

                tin.push_back(pIn);
                tout.push_back(pOut);
            }
            warpTriangle(imagesNorm[i], img, tin, tout);
        }

        // Add image intensities for averaging
        output = output + img;
    }

    //masking code below
    vector<Point> landmarks_for_mask;
    for (int j=0;j<=16;j++)
    {
        // cout<<endl<<"pushing ___"<<pointsNorm[0][j].x;
        // cout<<endl<<"pushing ___"<<pointsNorm[0][j].y;
        landmarks_for_mask.push_back(Point((int)pointsNorm[0][j].x,(int)pointsNorm[0][j].y));
    }
    for (int j=26;j>=17;j--)
    {
        // cout<<endl<<"pushing ___"<<pointsNorm[0][j].x;
        // cout<<endl<<"pushing ___"<<pointsNorm[0][j].y;
        // landmarks_for_mask.push_back(pointsNorm[0][j]);
        landmarks_for_mask.push_back(Point((int)pointsNorm[0][j].x,(int)pointsNorm[0][j].y));

    }
    Mat mask = Mat::zeros(output.size().height, output.size().width, CV_32FC3);
    fillConvexPoly(mask, landmarks_for_mask, Scalar(1.0, 1.0, 1.0), 16, 0);
    multiply(output,mask,output);

    __android_log_write(ANDROID_LOG_ERROR, "checkfinal", "before defining");
    Mat hogOutput;
    output.convertTo(hogOutput, CV_8UC3, 255);
    Size size2=output.size();
    HOGDescriptor hog(size2,cv::Size(16,16),cv::Size(8,8),cv::Size(8,8),9,1,-1,0,0.2,0,64,1);
    vector<float> descriptors;
    hog.compute(hogOutput,descriptors);
    __android_log_write(ANDROID_LOG_ERROR, "checkfinal", "Going to compute");

    src=hogOutput;
//saving the HOG descriptor
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
    __android_log_write(ANDROID_LOG_ERROR, "checkfinal", "Array written");

//    jfloat array1[3];
//    array1[0]=(float)desc_size;
//    array1[1]=3.0f;
//    array1[2]=4.0f;
//    env->SetFloatArrayRegion(result,0,3,array1);
    return result;


    // Divide by numImages to get average
//    output = output / (double)numImages;
//    Size size2=output.size();
//    HOGDescriptor hog(size2,Size(8,8),Size(4,4),Size(2,2),9,1,-1,0,0.2,0,64,1);
//    vector<float> descriptors;
//    hog.compute(src,descriptors);
    __android_log_write(ANDROID_LOG_ERROR, "newtry", "Complete Processing Done");

}