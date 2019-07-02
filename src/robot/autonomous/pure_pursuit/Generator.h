#define ARRAY_SIZE 5
#define X_INDEX 0
#define Y_INDEX 1
#define CURV_IND 2

class Generator
{
private:
    /* data */
public:
    Generator(/* args */);
    ~Generator();
    void generateCurvature(double path[][ARRAY_SIZE]);
};
