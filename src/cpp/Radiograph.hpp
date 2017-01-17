#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <string>
#include <cmath>

const double PIXELS_PER_MM = 3.7795275590551;
const int PIXEL_EDGE_BUFFER = 20; 

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
        std::string getFilename() const {return _filename;}
        int getSubtlety() const {return _subtlety;}
        int getSize() const {return _size;}
        int getAge() const {return _age;}
        bool isMale() const {return _isMale;}
        int getX() const {return _x;}
        int getY() const {return _y;}
        bool isMalignant() const {return _isMalignant;}
        bool hasNodule() const {return _hasNodule;}
        cv::Mat getImage() {return _image;}
        bool LoadImage(std::string directory);
        void DisplayImage();
        void BoxNodule();
        cv::Mat ExtractNodule();
};

inline std::ostream& operator<<(std::ostream &strm, const Radiograph &a) {return strm << "Radiograph(" << a.getFilename() << ", " << a.getSubtlety() << ", " << a.getSize() << ", " << a.getAge() << ", " << a.isMale() << ", " << a.getX() << ", " << a.getY() << ", " << a.isMalignant() << ", " << a.hasNodule() << ")";}
