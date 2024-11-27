#ifndef IMGHANDLER_H
#define IMGHANDLER_H
#include <iostream>
#include <QImage>
#include <opencv2/opencv.hpp>

class ImgHandler
{
public:
    static ImgHandler& getInstance();
    QImage cvMatToQImage(const cv::Mat &mat);
    QImage getResizedImage(const QString &imagePath);
    cv::Mat resizeImageTo300(const cv::Mat &inputImage);
    std::string matToBase64(const cv::Mat &image, const std::string &format);
    std::string matToHtml(const cv::Mat &image);
private:
    ImgHandler();
    ImgHandler(const ImgHandler& instance) = delete;
    ImgHandler& operator=(const ImgHandler&) = delete;
};

#endif // IMGHANDLER_H
