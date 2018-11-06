#ifndef IMAGEMANIPULATOR_H
#define IMAGEMANIPULATOR_H

#include <imageprocess.h>

void onMouse(int evt, int x, int y, int flags, void* param);


class ImageManipulator
{
public:
    ImageManipulator();
    ImageManipulator(const QString& fileName);
    void convertColor(COLOR color);
    void setEffect(EFFECT effect, int param);
    void handleEvent(int evt, int x, int y, int flags);

private:
    static int imID_count;
    int imID;
    cv::Mat image;
    QString imName;
    IPParams params;
    ImageProcess improc;

    void putEffectAndShow(cv::Point pt);
    void concatAndShow(std::vector<cv::Mat>& images, QString suffix="_channels");
};

#endif // IMAGEMANIPULATOR_H
