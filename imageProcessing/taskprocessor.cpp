#include "taskprocessor.h"
#include "CImg.h"
using namespace cimg_library;

CImg<unsigned char> getPartialImage(const CImg<unsigned char>& img, unsigned int neededPart)
{
    int width = img.width();
    int height = img.height();
    int spectrum = img.spectrum();
    if(neededPart >= spectrum)
        return CImg<unsigned char>();

    CImg<unsigned char> partialImage(width, height, img.depth(), 1, 0);

    for(int x = 0; x < width; ++x)
    for(int y = 0; y < height; ++y)
    {
        partialImage(x,y,0) = img(x,y,neededPart);
    }

    return partialImage;
}

void TaskProcessor::report(const string& reason)
{
    reporter_->measure(to_string(task_.Gettask_id())+":"+to_string(reportId_++));
    //reporter_->measure(reason);
}

void TaskProcessor::run()
{
    report("Start");

    CImg<unsigned char> partImage;
    {
        CImg<unsigned char> image(task_.GetfilePath().c_str());
        report("Open Image");

        partImage = image.get_crop(task_.Getpos_x(), task_.Getpos_y(),
                                           task_.Getpos_x() + task_.Getsize_x(), task_.Getpos_y() + task_.Getsize_y());

        image.deriche(1, 1, 'x');
        report("DERICHED X FULL");
        image.deriche(1, 1, 'y');
        report("DERICHED Y FULL");
    }
    report("Cropped Image");

    //partImage.display("PART");

    for(int i=0;i<3; ++i) // for each component of RGB
    {
        report("Next Component");
        CImg<unsigned char> image = getPartialImage(partImage, i);
        report("Component Created");

        CImg<unsigned char> euclidianDistanceMap = image.get_distance(0, 2);
        report("Distance Map Created");

        for(double alpha = 0.1; alpha <= 2;alpha+=0.5)
        {
            report("Next Alpha");
            CImg<unsigned char> deriche = image.get_deriche(alpha, 1, 'x');
            report("DERICHE X Created");

            CImg<unsigned char> watershed = deriche.get_watershed(euclidianDistanceMap);
            report("WATERSHED X Created");

            deriche = image.get_deriche(alpha, 1, 'y');
            report("DERICHE Y Created");

            watershed = deriche.get_watershed(euclidianDistanceMap);
            report("WATERSHED Y Created");
        }
    }

    report("LOOKS LIKE WE'RE DONE");
}

