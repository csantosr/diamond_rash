#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

#define CELL_SIZE 64

using namespace cv;

char parseIndexToChar(int idx) {
    if (idx < 7) return 'W'; // Wall
    if (idx < 16) return 'P'; // Path
    if (idx == 16) return 'C'; // Coin
    if (idx < 19) return 'G'; // Goal
    if (idx == 19) return 'X'; // Closed door
    if (idx == 20) return 'O'; // Opened door
    if (idx == 21) return 'H'; // Hole
    if (idx == 22) return 'P'; // Path
    if (idx < 25) return 'K'; // Key
    if (idx < 34) return 'L'; // Lava
    if (idx == 34) return 'D'; // Door
    if (idx < 55) return 'W'; // Wall
    if (idx < 57) return 'B'; // Button
    if (idx == 57) return 'W'; // Wall
    if (idx == 58) return 'S'; // Spikes
    if (idx == 59) return 'R'; // Rock
    if (idx == 60) return '@'; // Player
    if (idx == 61) return 'G'; // Goal
    return '@'; // Player
}

int main()
{
    std::string tile_path = samples::findFile("/Users/csantosr/Documents/Universidad/Inteligentes/diamond_rash/img/tilemap_completed.png");
    std::string board_path = samples::findFile("/Users/csantosr/Documents/Universidad/Inteligentes/diamond_rash/img/ss.png");
    Mat tile_map = imread(tile_path, IMREAD_COLOR);
    Mat board = imread(board_path, IMREAD_COLOR);
    if(tile_map.empty() || board.empty())
    {
        std::cout << "Could not read the image: " << tile_path << std::endl;
        return 1;
    }

    int board_h = board.rows;
    int board_w = board.cols;

    Mat clean_board = board(Range(CELL_SIZE * 2, board_h - CELL_SIZE), Range(CELL_SIZE, board_w - CELL_SIZE));

    Mat board_tiles[96];
    for (int h = 0; h < 12; ++h) {
        for (int w = 0; w < 8; ++w) {
            board_tiles[(h * 8) + w] = clean_board(Range(h * CELL_SIZE, (h * CELL_SIZE) + CELL_SIZE),
                                                   Range(w * CELL_SIZE, (w * CELL_SIZE) + CELL_SIZE));
        }
    }

    Mat map_tiles[72];
    for (int h = 0; h < 9; ++h) {
        for (int w = 0; w < 8; ++w) {
            map_tiles[(h * 8) + w] = tile_map(Range(h * CELL_SIZE, (h * CELL_SIZE) + CELL_SIZE),
                                              Range(w * CELL_SIZE, (w * CELL_SIZE) + CELL_SIZE));
        }
    }

    char indexesOfTileMap[96];
    for (int i = 0; i < 96; ++i) {
        int indexOfTileMap = 0;
        double maxValue = 0;
        Mat tileToCheck;
        if (i<8) {
            tileToCheck = board_tiles[i](Range(32, 64), Range(0, 63));
        } else {
            tileToCheck = board_tiles[i](Range(0, 64), Range(0, 63));
        }

        for (int j = 0; j < 65; ++j) {
            Mat result;
            Mat tileToCompare;
            if (i<8) {
                tileToCompare = map_tiles[j](Range(32, 64), Range(0, 63));
                result.create(32, 63, CV_32FC2);
            } else {
                tileToCompare = map_tiles[j](Range(0, 64), Range(0, 63));
                result.create(64, 63, CV_32FC2);
            }

            Mat tileToCheckGray;
            cvtColor(tileToCheck, tileToCheckGray, COLOR_BGR2GRAY);

            Mat tileToCompareGray;
            cvtColor(tileToCompare, tileToCompareGray, COLOR_BGR2GRAY);

            matchTemplate(tileToCheckGray, tileToCompareGray, result, TM_CCORR_NORMED);
            double minVal; double maxVal; Point minLoc; Point maxLoc;
            Point matchLoc;
            minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

            if (maxVal > maxValue) {
                maxValue = maxVal;
                indexOfTileMap = j;
            }
        }
        indexesOfTileMap[i] = parseIndexToChar(indexOfTileMap);
    }
    for (int i = 0; i < 96; ++i) {
        if (i%8 == 0 && i != 0) {
            std::cout << std::endl;
        }
        std::cout << indexesOfTileMap[i];
    }
//    waitKey(0);
    return 0;
}