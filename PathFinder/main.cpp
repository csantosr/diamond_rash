#include "AStar/AStar.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <map>

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
    if (idx == 62) return '@'; // Player
    return '#'; // Player with key
}

char* getBoard() {
    std::string tile_path = samples::findFile("/Users/csantosr/Documents/Universidad/Inteligentes/diamond_rash/img/tilemap_completed.png");
    std::string board_path = samples::findFile("/Users/csantosr/Documents/Universidad/Inteligentes/diamond_rash/img/ss.png");
    Mat tile_map = imread(tile_path, IMREAD_COLOR);
    Mat board = imread(board_path, IMREAD_COLOR);
    if(tile_map.empty() || board.empty())
    {
        std::cout << "Could not read the image: " << tile_path << std::endl;
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

    static char indexesOfTileMap[96];
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
    return indexesOfTileMap;
}

bool isRockPathPossible(AStar::CoordinateList path, char* board) {
    AStar::CoordinateList::iterator pathIterator;
    ushort pathIdx = 0;
    for (pathIterator = path.begin(); pathIterator != path.end(); pathIterator++,pathIdx++) {
        if (pathIdx == 0) {
            continue;
        }
//        std::cout << "FROM (" << path[pathIdx-1].x << "," << path[pathIdx-1].y << ") TO (" << pathIterator->x << "," << pathIterator->y << ");" << std::endl;
        if (pathIterator->x > path[pathIdx-1].x) {
            std::cout << path[pathIdx-1].x -1 << " " << path[pathIdx-1].y << " " << board[(path[pathIdx-1].x -1) + (path[pathIdx-1].y * 8)] << " A" << std::endl;
            if (board[(path[pathIdx-1].x -1) + (path[pathIdx-1].y * 8)] != 'P' && board[(path[pathIdx-1].x -1) + (path[pathIdx-1].y * 8)]  != 'R') {
                return false;
            }
        } else if (pathIterator->x < path[pathIdx-1].x) {
            std::cout << path[pathIdx-1].x + 1 << " " << path[pathIdx-1].y << " " <<board[(path[pathIdx-1].x +1) + (path[pathIdx-1].y * 8)] << " B" << std::endl;
            if (board[(path[pathIdx-1].x +1) + (path[pathIdx-1].y * 8)] != 'P' && board[(path[pathIdx-1].x +1) + (path[pathIdx-1].y * 8)] != 'R') {
                return false;
            }
        } else if (pathIterator->y > path[pathIdx-1].y) {
            std::cout << path[pathIdx-1].x << " " << path[pathIdx-1].y -1 << " " <<  board[(path[pathIdx-1].x ) + ((path[pathIdx-1].y -1)* 8)] << " C" << std::endl;
            if (board[(path[pathIdx-1].x ) + ((path[pathIdx-1].y -1)* 8)] != 'P' && board[(path[pathIdx-1].x ) + ((path[pathIdx-1].y -1)* 8)] != 'R') {
                return false;
            }
        } else if (pathIterator->y < path[pathIdx-1].y) {
            std::cout << path[pathIdx-1].x << " " << path[pathIdx-1].y +1 << " " << board[(path[pathIdx-1].x ) + ((path[pathIdx-1].y +1)* 8)] << " D" << std::endl;
            if (board[(path[pathIdx-1].x ) + ((path[pathIdx+1].y +1)* 8)] != 'P' && board[(path[pathIdx-1].x ) + ((path[pathIdx+1].y +1)* 8)] != 'R') {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    char* indexesOfTileMap = getBoard();
    std::vector<AStar::Vec2i> coins;
    std::vector<AStar::Vec2i> keyDoors;
    std::vector<AStar::Vec2i> keys;
    std::vector<AStar::Vec2i> rocks;
    std::vector<AStar::Vec2i> rockRests;
    std::vector<AStar::Vec2i> rockRestsAux;

    std::vector<AStar::Vec2i>::iterator it;
    bool goalReached = false, playerHasKey = false, hasRocks = false;

    AStar::Generator generator;
    generator.setWorldSize({8, 12});
    generator.setHeuristic(AStar::Heuristic::manhattan);
    generator.setDiagonalMovement(false);

    static AStar::Vec2i player, goal;

    for (int i = 0; i < 96; ++i) {
        if (indexesOfTileMap[i] == '@') {
            player = {i % 8, i / 8};
            playerHasKey = false;
        }
        if (indexesOfTileMap[i] == '#') {
            player = {i % 8, i / 8};
            playerHasKey = true;
        }
        if (indexesOfTileMap[i] == 'G') {
            generator.addCollision({i % 8, i / 8});
            goal = {i % 8, i / 8};
        }
        if (indexesOfTileMap[i] == 'W') {
            generator.addCollision({i % 8, i / 8});
        }
        if (indexesOfTileMap[i] == 'C') {
            coins.push_back({i % 8, i / 8});
        }
        if (indexesOfTileMap[i] == 'D') {
            keyDoors.push_back({i % 8, i / 8});
            generator.addCollision({i % 8, i / 8});
        }
        if (indexesOfTileMap[i] == 'R') {
            rocks.push_back({i % 8, i / 8});
            hasRocks = true;
        }
        if (indexesOfTileMap[i] == 'K') {
            keys.push_back({i % 8, i / 8});
        }
        if (indexesOfTileMap[i] == 'H') {
            rockRests.push_back({i % 8, i / 8});
        }
    }

    AStar::CoordinateList path;

    while(!goalReached) {
        if (coins.empty()) {
            generator.removeCollision(goal);
        }
        if (coins.empty() && keyDoors.empty() && keyDoors.empty()) {
            AStar::CoordinateList pathToGoal = generator.findPath(goal, player);
            path.insert(path.end(), pathToGoal.begin(), pathToGoal.end());
            player = goal;
        } else {
            uint i = 0, min = 1000, minIdx = 0;
            std::vector<AStar::Vec2i> mustPassPoints;
            std::vector<AStar::Vec2i>::iterator mustPassPointsIterator;
            if (playerHasKey) {
                mustPassPoints.reserve(coins.size() + keyDoors.size());
                mustPassPoints.insert(mustPassPoints.end(), coins.begin(), coins.end());
                mustPassPoints.insert(mustPassPoints.end(), keyDoors.begin(), keyDoors.end());
            } else {
                mustPassPoints.reserve(coins.size() + keys.size());
                mustPassPoints.insert(mustPassPoints.end(), coins.begin(), coins.end());
                mustPassPoints.insert(mustPassPoints.end(), keys.begin(), keys.end());
            }
            for (mustPassPointsIterator = mustPassPoints.begin(); mustPassPointsIterator != mustPassPoints.end(); mustPassPointsIterator++,i++) {
                AStar::CoordinateList pathToCoin = generator.findPath(*mustPassPointsIterator, player);
                if (pathToCoin[0].x == player.x && pathToCoin[0].y == player.y && pathToCoin.size() < min) {
                    min = pathToCoin.size();
                    minIdx = i;
                }
            }
            AStar::CoordinateList pathToMustPass = generator.findPath(mustPassPoints[minIdx], player);
            path.insert(path.end(), pathToMustPass.begin(), pathToMustPass.end());
            player = mustPassPoints[minIdx];
            if (minIdx >= coins.size()) {
                uint transIdx = minIdx - coins.size();
                if (playerHasKey) {
                    generator.removeCollision(mustPassPoints[minIdx]);
                    keyDoors.erase(std::next(keyDoors.begin(), transIdx), std::next(keyDoors.begin(), transIdx + 1));
                } else {
                    keys.erase(std::next(keys.begin(), transIdx), std::next(keys.begin(), transIdx + 1));
                }
                playerHasKey = !playerHasKey;
            } else {
                coins.erase(std::next(coins.begin(), minIdx), std::next(coins.begin(), minIdx + 1));
            }
        }
        if (player == goal) {
            goalReached = true;
        }
    }

    for (int i = 0; i < 96; ++i) {
        if (indexesOfTileMap[i] == 'P') {
            AStar::Vec2i coord = {i % 8, i / 8};
            bool present = false;
            for (auto step : path) {
                if (step == coord) {
                    present = true;
                }
            }
            if (!present) {
                rockRestsAux.push_back(coord);
            }
        }
    }

    std::map<int, AStar::CoordinateList> rockMap;

    uint rockIdx = 0;
    for (auto rock : rocks) {
        uint min = 1000, minIdx = 0, index = 0;
        for (auto rockRest: rockRests) {
            AStar::CoordinateList pathToRest = generator.findPath(rockRest, rock);
            if (pathToRest[0].x == rock.x && pathToRest[0].y == rock.y && pathToRest.size() < min) {
                bool isPossible = isRockPathPossible(pathToRest, indexesOfTileMap);
//                std::cout << "[" << rock.x << "," << rock.y << "]: " << isPossible << std::endl;
                AStar::CoordinateList virtualCollisions;
                int tries = 4;
                while (!isPossible && tries > 0) {
                    std::cout << "TRY " << tries << std::endl;
                    virtualCollisions.push_back(pathToRest[1]);
                    generator.addCollision(pathToRest[1]);
                    pathToRest = generator.findPath(rockRests[minIdx], rock);
//                    for (auto step : pathToRest) {
//                        std::cout << "STEP: (" << step.x << "," << step.y << ");" << std::endl;
//                    }
                    isPossible = isRockPathPossible(pathToRest, indexesOfTileMap);
//                    std::cout << "[" << rock.x << "," << rock.y << "]: " << isPossible << std::endl;
                    tries--;
                }
                for (auto virtualCollision : virtualCollisions) {
                    generator.removeCollision(virtualCollision);
                }
                if (isPossible) {
                    rockMap.insert(std::make_pair(rockIdx, generator.findPath(rockRests[minIdx], rock)));
                    rockRests.erase(std::next(rockRests.begin(), index), std::next(rockRests.begin(), index + 1));
                    min = pathToRest.size();
                    minIdx = index;
                }
            }
            index++;
        }
        rockIdx++;
    }

    for (const auto& rockPair: rockMap) {
        std::cout << "[" << rockPair.first << "]: ";
        for (auto pathCoor : rockPair.second) {
            std::cout << "(" << pathCoor.x << "," << pathCoor.y << "), ";
        }
        std::cout << std::endl;
    }

    if (hasRocks) {}

    AStar::CoordinateList::iterator pathIterator;
    ushort pathIdx = 0;
    std::vector<char> pathDescribed;
    for (pathIterator = path.begin(); pathIterator != path.end(); pathIterator++,pathIdx++) {
        if (pathIdx == 0) {
            continue;
        }
        if (pathIterator->x > path[pathIdx-1].x) {
            pathDescribed.push_back('r');
        } else if (pathIterator->x < path[pathIdx-1].x) {
            pathDescribed.push_back('l');
        } else if (pathIterator->y > path[pathIdx-1].y) {
            pathDescribed.push_back('d');
        } else if (pathIterator->y < path[pathIdx-1].y) {
            pathDescribed.push_back('u');
        }
    }

    std::cout << pathDescribed.data();
}