#include <iostream>
#include "descriptormatcher.h"
cv::Mat DescHandler::result;


std::vector<DescHandler*> createHandlers()
{
    std::vector<DescHandler*> handlers;
    handlers.push_back(DescHandler::factory("sift", "bf"));
    handlers.push_back(DescHandler::factory("surf", "flann"));
    handlers.push_back(DescHandler::factory("orb", "flann"));
    return handlers;
}


int main()
{
    std::cout << "Press 'f' to change reference frame," << std::endl
              << "'u' to increase match accept ratio," << std::endl
              << "'d' to decrease match accept ratio," << std::endl
              << "and 'q' to quit." << std::endl;
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;

    std::vector<DescHandler*> curDesc = createHandlers();
    std::vector<DescHandler*> refDesc = createHandlers();

    // init ref descriptors by first frame
    cv::Mat initframe;
    cap >> initframe;
    for(auto des: refDesc)
        des->detectAndCompute(initframe);

    float acceptRatio = 0.5f;

    while(1)
    {
        cv::Mat frame;
        cap >> frame;
        for(auto des: curDesc)
            des->detectAndCompute(frame);

        int key = cv::waitKey(10);
        if(key==int('f') || key==int('F'))
        {
            std::cout << "set fixed reference result" << std::endl;
            for(auto des: refDesc)
                des->detectAndCompute(frame);
        }
        else if(key==int('u') || key==int('U'))
            acceptRatio = std::min(acceptRatio + 0.1f, 1.f);
        else if(key==int('d') || key==int('D'))
            acceptRatio = std::max(acceptRatio - 0.1f, 0.f);
        else if(key==int('q') || key==int('Q'))
            break;

        for(int i=0; i<curDesc.size(); i++)
        {
            auto matches = curDesc[i]->match(refDesc[i]->getDescriptors(), acceptRatio);
            DescHandler::drawAndAppendResult(curDesc[i], refDesc[i], matches);
        }


        cv::Mat result = DescHandler::getResultingImg(1000);
        cv::imshow("matches", result);
    }

    return 0;
}
