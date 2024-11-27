#include "imghandler.h"
#include <qdebug.h>

// 将OpenCV的Mat图像转换为QImage
ImgHandler &ImgHandler::getInstance()
{
    static ImgHandler instance;
    return instance;
}

QImage ImgHandler::cvMatToQImage(const cv::Mat& mat)
{
    if (mat.type() == CV_8UC1)
    {
        // 灰度图像
        QImage image(mat.cols, mat.rows, QImage::Format_Grayscale8);
        uchar *dst = image.bits();
        const uchar *src = mat.data;

        for (int y = 0; y < mat.rows; ++y)
        {
            memcpy(dst, src, mat.cols);
            src += mat.step;
            dst += image.bytesPerLine();
        }

        return image;
    }
    else if (mat.type() == CV_8UC3)
    {
        // 彩色图像
        QImage image(mat.cols, mat.rows, QImage::Format_RGB888);
        const uchar *src = mat.data;
        uchar *dst = image.bits();

        for (int y = 0; y < mat.rows; ++y)
        {
            for (int x = 0; x < mat.cols; ++x)
            {
                dst[(y * mat.cols + x) * 3] = src[(y * mat.step + x) * 3 + 2];
                dst[(y * mat.cols + x) * 3 + 1] = src[(y * mat.step + x) * 3 + 1];
                dst[(y * mat.cols + x) * 3 + 2] = src[(y * mat.step + x) * 3];
            }

            src += mat.step;
        }

        return image;
    }
    else
    {
        std::cerr << "不支持的图像类型" << std::endl;
        return QImage();
    }
}

// 获取图片长宽并等比例缩小图片
QImage ImgHandler::getResizedImage(const QString& imagePath)
{
    cv::Mat img = cv::imread(imagePath.toStdString());

    if (img.empty())
    {
        std::cerr << "无法读取图像" << std::endl;
        return QImage();
    }

    int height = img.rows;
    int width = img.cols;

    double ratio;
    int newWidth, newHeight;

    if (width > height)
    {
        ratio = static_cast<double>(240) / width;
        newWidth = 240;
        newHeight = static_cast<int>(height * ratio);
    }
    else
    {
        ratio = static_cast<double>(240) / height;
        newHeight = 240;
        newWidth = static_cast<int>(width * ratio);
    }

    if (newWidth < 180)
    {
        if (width > height)
        {
            ratio = static_cast<double>(180) / width;
            newWidth = 180;
            newHeight = static_cast<int>(height * ratio);
        }
        else
        {
            ratio = static_cast<double>(180) / height;
            newHeight = 180;
            newWidth = static_cast<int>(width * ratio);
        }
    }

    cv::Mat resizedImg;
    cv::resize(img, resizedImg, cv::Size(newWidth, newHeight));

    return cvMatToQImage(resizedImg);
}

cv::Mat ImgHandler::resizeImageTo300(const cv::Mat& inputImage)
{
    if (inputImage.empty())
    {
        qDebug() << "输入图像为空，无法处理！";
        return cv::Mat(); // 返回空图像
    }

    // 获取原始宽度和高度
    int originalWidth = inputImage.cols;
    int originalHeight = inputImage.rows;

    // 计算缩放比例
    double scaleFactor = 1.0;

    if (originalWidth > originalHeight)
    {
        scaleFactor = 300.0 / originalWidth; // 宽度为基准
    }
    else
    {
        scaleFactor = 300.0 / originalHeight; // 高度为基准
    }

    // 计算新的尺寸
    int newWidth = static_cast<int>(originalWidth * scaleFactor);
    int newHeight = static_cast<int>(originalHeight * scaleFactor);

    // 调整图像大小
    cv::Mat resizedImage;
    cv::resize(inputImage, resizedImage, cv::Size(newWidth, newHeight));

    return resizedImage;
}

// 工具函数：将 cv::Mat 转换为 Base64 字符串
std::string ImgHandler::matToBase64(const cv::Mat& image, const std::string& format )
{
    if (image.empty())
    {
        qDebug() << "图像为空，无法转换！";
        return "";
    }

    // 将 Mat 编码为指定格式（JPEG/PNG）
    std::vector<uchar> buffer;
    std::vector<int> params;

    if (format == "jpeg")
    {
        params = {cv::IMWRITE_JPEG_QUALITY, 90}; // JPEG 压缩质量
    }
    else if (format == "png")
    {
        params = {cv::IMWRITE_PNG_COMPRESSION, 3}; // PNG 压缩级别
    }
    else
    {
        qDebug() << "不支持的图像格式：" << QString::fromStdString(format);
        return "";
    }

    cv::imencode("." + format, image, buffer, params);

    // 将二进制数据编码为 Base64
    const uchar* data = reinterpret_cast<const uchar*>(buffer.data());
    std::string base64 = QByteArray::fromRawData(reinterpret_cast<const char*>(data), static_cast<int>(buffer.size()))
                         .toBase64()
                         .toStdString();

    return base64;
}

// 主函数：将 cv::Mat 转换为 HTML 片段
std::string ImgHandler::matToHtml(const cv::Mat& image)
{
    // 转换为 Base64 格式
    std::string base64 = matToBase64(image, "jpeg");

    // 构造 HTML 片段
    std::ostringstream html;
    html << "<img src=\"data:image/jpeg;base64," << base64 << "\" alt=\"Image\" />";

    return html.str();
}

ImgHandler::ImgHandler()
{

}
