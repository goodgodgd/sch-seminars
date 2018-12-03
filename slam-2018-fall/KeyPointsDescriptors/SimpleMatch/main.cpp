#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>


int main()
{
    std::cout << "Press 'f' to show fixed snapshot, and 'q' to quit." << std::endl;
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;

    cv::Ptr<cv::Feature2D> orb = cv::ORB::create();
    cv::Ptr<cv::FlannBasedMatcher> matcher = new cv::FlannBasedMatcher(new cv::flann::LshIndexParams(12, 20, 2));;
    std::vector<cv::KeyPoint> refKeypoints, curKeypoints;
    cv::Mat refDescriptors, curDescriptors;
    std::vector<cv::DMatch> matches;
    const cv::String name = "ORB";
    const float acceptRatio = 0.5f;

    cv::Mat refimg, frame;
    cap >> refimg;
    orb->detectAndCompute(refimg, cv::Mat(), refKeypoints, refDescriptors);


    while(1)
    {
        cap >> frame;

        // detect key points and descriptors as the key points
        orb->detectAndCompute(frame, cv::Mat(), curKeypoints, curDescriptors);
        // find closest descriptor pairs by flann
        matcher->match(refDescriptors, curDescriptors, matches);

        // sort matches by score and remove not so good matches
        std::sort(matches.begin(), matches.end());
        const int numGoodMatches = matches.size() * acceptRatio;
        matches.erase(matches.begin()+numGoodMatches, matches.end());

        // draw matches
        cv::Mat matchimg;
        cv::drawMatches(refimg, refKeypoints,
                        frame, curKeypoints,
                        matches, matchimg);
        cv::putText(matchimg, name, cv::Point(10,30),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar::all(0), 2);


        int key = cv::waitKey(10);
        if(key==int('f') || key==int('F'))
        {
            std::cout << "set fixed reference result" << std::endl;
            refimg = frame.clone();
            orb->detectAndCompute(refimg, cv::Mat(), refKeypoints, refDescriptors);
        }
        else if(key==int('q') || key==int('Q'))
            break;

        cv::imshow("simple match", matchimg);
    }

    return 0;
}
