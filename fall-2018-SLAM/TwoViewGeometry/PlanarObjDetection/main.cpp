#include <iostream>
#include <vector>
#include "planarobjdet.h"
cv::Mat DescHandler::result;

#define TRAINIMG_WIDTH  500


std::vector<PlanarObjDet*> createDetectors()
{
    cv::Mat trainimg1 = cv::imread(std::string(PRJ_PATH) + "/../data/stop.png");
    cv::Mat trainimg2 = cv::imread(std::string(PRJ_PATH) + "/../data/crosswalk.jpg");
    std::vector<cv::Mat> images = {trainimg1, trainimg2};
    std::vector<PlanarObjDet*> objdets;
    // widths of all the train images are set to TRAINIMG_WIDTH
    for(cv::Mat& image: images)
    {
        if(image.cols != TRAINIMG_WIDTH)
        {
            cv::Size neosize(TRAINIMG_WIDTH, int(image.rows*float(TRAINIMG_WIDTH)/float(image.cols)));
            cv::resize(image, image, neosize);
            std::cout << "resize: " << image.rows << " " << image.cols << std::endl;
        }
        objdets.push_back(new PlanarObjDet(image));
    }
    return objdets;
}


int main()
{
    std::cout << "Press 'a' to add train image," << std::endl
              << "'u' to increase inlier ratio threshold," << std::endl
              << "'d' to decrease inlier ratio threshold," << std::endl
              << "and 'q' to quit." << std::endl;
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;

    std::vector<PlanarObjDet*> objdets = createDetectors();
    DescHandler* queryHdl = DescHandler::factory("orb", "flann");
    float inlierRatio = 0.2;

    while(1)
    {
        cv::Mat frame;
        cap >> frame;
        queryHdl->detectAndCompute(frame);

        for(auto det: objdets)
            det->detect(queryHdl, inlierRatio);
        cv::Mat result = PlanarObjDet::resultingImg(1000);
        if(!result.empty())
            cv::imshow("detection result", result);

        int key = cv::waitKey(10);
        if(key==int('a') || key==int('A'))
        {
            std::cout << "add train image" << std::endl;
            if(frame.cols!=TRAINIMG_WIDTH)
            {
                cv::Size neosize(TRAINIMG_WIDTH, int(frame.rows*float(TRAINIMG_WIDTH)/float(frame.cols)));
                cv::resize(frame, frame, neosize);
            }
            objdets.push_back(new PlanarObjDet(frame));
        }
        else if(key==int('u') || key==int('U'))
            inlierRatio = std::min(inlierRatio + 0.1f, 0.9f);
        else if(key==int('d') || key==int('D'))
            inlierRatio = std::max(inlierRatio - 0.1f, 0.1f);
        else if(key==int('q') || key==int('Q'))
            break;
    }

    for(auto det: objdets)
        delete det;
    return 0;
}
