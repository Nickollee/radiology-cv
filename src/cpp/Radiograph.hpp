#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <string>

class Radiograph
{
    private:
        std::string _filename;
        int _subtlety;
        int _size;
        int _age;
        bool _isMale;
        int _x;
        int _y;
        bool _isMalignant;
        bool _hasNodule;
        cv::Mat _image;

    public:
        Radiograph(std::string filename, int subtlety, int size, int age, bool isMale, int x, int y, bool isMalignant, bool hasNodule);
        std::string getFilename() {return _filename;}
        int getSubtlety() {return _subtlety;}
        int getSize() {return _size;}
        int getAge() {return _age;}
        bool isMale() {return _isMale;}
        int getX() {return _x;}
        int getY() {return _y;}
        bool isMalignant() {return _isMalignant;}
        bool hasNodule() {return _hasNodule;}
        cv::Mat getImage() {return _image;}
        bool LoadImage();
        void DisplayImage();
        void CircleNodule();
};