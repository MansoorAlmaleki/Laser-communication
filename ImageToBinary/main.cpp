#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{

    unsigned int ResolutionX;
    unsigned int ResolutionY;
    unsigned int MaxValue;
    unsigned int MaxImageSize = 128;//Square format only, Image size no more than 128 pixels
    string Version;

    string FolderPath = "InputImage/";
    string FileName;

    ifstream ImageFile;
    ofstream DecimalFile;
    DecimalFile.open("Output/DecimalFile.txt");

    cout << "============================================" <<endl;
    cout << "              Bitmap to Binary              " <<endl;
    cout << "============================================" <<endl;
    cout <<endl;
    cout << "   >>>>>>>>>>>>>REQUIREMENTS<<<<<<<<<<<<<   " <<endl;
    cout << "             SQUARE FORMAT ONLY             " <<endl;
    cout << "          MAX 128 PIXEL RESOLUTION          "  <<endl;
    cout <<endl;
    cout <<endl;
    cout <<endl;




    while(true){
        cout << "Enter File name (CASE SENSITIVE)" <<endl;
        cin >> FileName;

        ImageFile.open(FolderPath + FileName);

        if(ImageFile.is_open()){

            ImageFile >> Version >> ResolutionX >> ResolutionY >> MaxValue;
            if(Version == "P3" && ResolutionX <= MaxImageSize && ResolutionY <= MaxImageSize){
                break;
            }
            else{
                ImageFile.close();
                cout << "INVALID FILE PARAMETERS!";
                cout <<endl;
            }

        }
        else{
            ImageFile.close();
            cout << "FILE COULD NOT BE LOADED!" <<endl;
            cout << endl;
        }

    }

    for(int i = 0; i < ResolutionX*ResolutionY; i++){

        int R;
        int G;
        int B;
        int average;

        ImageFile >> R >> G >> B;
        average = (R+G+B)/3;

        DecimalFile << average;

        if(i < ResolutionX*ResolutionY-1){
            DecimalFile << ",";
        }

    }
    DecimalFile.close();

    cout << "File saved";

    return 0;
}
