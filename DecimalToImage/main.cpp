#include <iostream>
#include <cstdint>
#include <fstream>
#include <bitset>
#include <string>

using namespace std;

int main()
{
    int ImageResolution = 128;//alter this value to match the Square format image size(NO MORE THAN 128PIXELS)
    int MaxValue = 255;

    string FolderPath = "Input/";
    string FileName;

    ifstream DataFile;
    ofstream BinaryFile;

    cout << "============================================" << endl;
    cout << "            'Decimal' to Bitmap             " << endl;
    cout << "============================================" << endl;
    cout << "   >>>>>>>>>>>>>REQUIREMENTS<<<<<<<<<<<<<   " << endl;
    cout << "            128 PIXEL RESOLUTION            " << endl;
    cout << endl;

    while(true){
        cout << "Enter File Name (CASE SENSITIVE): ";
        cin >> FileName;
        cout << endl;

            DataFile.open(FolderPath + FileName);
            if(DataFile.is_open()){
                BinaryFile.open("Output/BitmapImage.ppm");
                BinaryFile << "P3\n" << ImageResolution << " " << ImageResolution << "\n" << MaxValue << "\n";
                break;
            }else{
                cout << "FILE COULD NOT OPEN!" << endl << endl;
            }

    }

    uint32_t BinaryString;//FORMAT: (Bit Reset: 1bit)->(PositionX: 8bits)->(Bit Reset: 1bit)->(PositionY: 8bits)->(Bit Reset: 1bit)->(Color: 8bits)->(Bit Reset: 1bit), 0 00000000 0 00000000 0 00000000 0
    //first 28 least significant bits store data 
    uint8_t ImageArray[128][128] = {0};

    int PositionX;
    int PositionY;
    int Color;
    int counter;

    while(true){

        counter++;
        DataFile >> BinaryString;
        PositionX = (BinaryString >> 19) & 0xFF;// bit mask 0b11111111 which only selects the first 8 Least significant bits of the shifted 32 bit Binary string
        PositionY = (BinaryString >> 10) & 0xFF;
        Color     = (BinaryString >> 1) & 0xFF;
        if (PositionX < ImageResolution && PositionY < ImageResolution) {
            ImageArray[PositionY][PositionX] = Color;
        }

        if(counter >= (ImageResolution*ImageResolution)){
            break;
        }
    }

    for(int y = 0; y < ImageResolution; y++){
        for(int x = 0; x < ImageResolution; x++){

            int PixelColor = ImageArray[y][x];
            for(int i = 0; i < 3; i++){
                BinaryFile << PixelColor << " ";
            }
            BinaryFile << "\n";//creates a new line after each pixel rgb value
        }
    }

    DataFile.close();
    BinaryFile.close();

    cout << "Image conversion complete, Check output/BitmapImage.ppm" << endl;

    return 0;
}
