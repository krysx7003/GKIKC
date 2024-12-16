#pragma once
#include <vector>
struct pointsRgb{
    //Pozycja
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    //Chyba powinny być tablice i.e. rgb dla a,d,s
    //Kolor
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
    //U i v tekstur
    float u = 0;
    float v = 0;
};
using namespace std;
class Egg{
    private:
	int density = 0;    //Ilość punktów na linii poziomej
    bool color = true;  //Czy punkty powinny mieć kolor
    float scale = 0.5f; 
    float pointSize = 5;
    int height,width;
    vector<vector<pointsRgb>> pointsMatrix;
    float randFloat(); 
    public:
    //Constructors
    Egg(int density);
    //Methods
    void generateMatrix();
    void draw();
    //Setters
    void setDensity(int newDensity);
    void setColor(float newColor);
    void setScale(float newScale);
    void setPointSize(float newPointSize);
    void setTextureSize(int newHeight,int newWidth);
    //Getters
    int getDensity();
    float getColor();
    float getScale();
    float getPointSize();
    void initMaterial();
    vector<vector<pointsRgb>> getPointsMatrix();
    //Deconstructors
    ~Egg();
};