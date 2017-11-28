/**
 * @file    searchWindow.cpp
 * RapidLib
 * @author  Michael Zbyszynski
 * @date    14 Sep 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#include "searchWindow.h"

template<typename T>
searchWindow<T>::searchWindow(const int seriesXSize, const int seriesYSize, const warpPath &shrunkenWarpPath, const int searchRadius) :
minValues(seriesXSize, -1), maxValues(seriesXSize, 0), minMaxValues(seriesXSize, std::make_pair(-1, 0)), maxY(seriesYSize - 1) {
    
    //Current location of higher resolution path
    std::pair<int, int> currentIndex = shrunkenWarpPath.indices[0];
    
    //Last evaluated part of low resolution path
    std::pair<int, int> lastWarped = std::make_pair(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    
    int blockSize = 2; //TODO: something other than 2? Different for x and y?
    
    //project each part of the low-res path to high res cells
    for ( auto &xyIndex : shrunkenWarpPath.indices) {
        
        if (xyIndex.first > lastWarped.first) {
            currentIndex.first += blockSize;
        }
        if (xyIndex.second > lastWarped.second) {
            currentIndex.second += blockSize;
        }
        
        if ((xyIndex.first > lastWarped.first) && (xyIndex.second > lastWarped.second))
        {
            markVisited(currentIndex.first - 1, currentIndex.second);
            markVisited(currentIndex.first, currentIndex.second - 1);
        }
        
        for (int j = 0; j < blockSize; ++j) {
            markVisited(currentIndex.first + j, currentIndex.second);
            markVisited(currentIndex.first + j, currentIndex.second + blockSize - 1); //TODO: These are redundant?
        }
        
        lastWarped = xyIndex;
    }
    
    if (searchRadius > 0) {
        expandWindow(1);
        expandWindow(searchRadius-1);
    }
}

template<typename T>
void searchWindow<T>::markVisited(int col, int row) {
    if (row <= maxY && col < minValues.size()) { //Don't mark beyond the edge of the window
        if (minValues[col] == -1) {
            minValues[col] = row;
            maxValues[col] = row;
            //size++;
        } else if (minValues[col] > row) {
            //size += minValues[col] - row;
            minValues[col] = row;
        } else if (maxValues[col] < row) {
            //size += row - maxValues[col];
            maxValues[col] = row;
            minMaxValues[col].second = row;
        }
    }
}

template<typename T>
void searchWindow<T>::expandWindow(int radius) {
    if (radius > 0) {
        
        //Add all cells in the current window to a vector.
        std::vector<std::pair<int, int>> windowCells;
        for (int currentX = 0; currentX < minValues.size(); ++currentX) {
            for (int currentY = minValues[currentX]; currentY <= maxValues[currentX]; ++currentY) {
                windowCells.push_back(std::make_pair(currentX, currentY));
            }
        }
        
        int maxX = int(minValues.size() - 1);
        
        for (auto &currentCell : windowCells) {
            
            if (currentCell.first != 0 && currentCell.second != maxY) { //move to upper left if possible
                //expand until edges are met
                int targetX = currentCell.first - radius;
                int targetY = currentCell.second + radius;
                
                if (targetX >= 0 && targetY <= maxY) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = std::max(0 - targetX, targetY - maxY);
                    markVisited(targetX + cellsPastEdge, targetY + cellsPastEdge);
                }
            }
            
            if (currentCell.second != maxY) { //move up if possible
                int targetX = currentCell.first;
                int targetY = currentCell.second + radius;
                if (targetY <= maxY) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = targetY - maxY;
                    markVisited(targetX, targetY - cellsPastEdge);
                }
            }
            
            if (currentCell.first != maxX && currentCell.second != maxY) { //move upper right if possible
                int targetX = currentCell.first + radius;
                int targetY = currentCell.second + radius;
                if (targetX <= maxX && targetY <= maxY) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = std::max(targetX - maxX, targetY - maxY);
                    markVisited(targetX - cellsPastEdge, targetY - cellsPastEdge);
                }
            }
            
            if (currentCell.first != 0) { //move left if possible
                int targetX = currentCell.first - radius;
                int targetY = currentCell.second;
                if (targetX != 0) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = (0 - targetX);
                    markVisited(targetX + cellsPastEdge, targetY);
                }
            }
            
            if (currentCell.first != maxX) { //move right if possible
                int targetX = currentCell.first + radius;
                int targetY = currentCell.second;
                if (targetX <= maxX) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = (targetX - maxX);
                    markVisited(targetX - cellsPastEdge, targetY);
                }
            }
            
            if (currentCell.first != 0 && currentCell.second != 0) { //move to lower left if possible
                int targetX = currentCell.first - radius;
                int targetY = currentCell.second - radius;
                
                if (targetX >= 0 && targetY >= 0) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = std::max(0 - targetX, 0 - targetY);
                    markVisited(targetX + cellsPastEdge, targetY + cellsPastEdge);
                }
            }
            
            if (currentCell.second != 0) { //move down if possible
                int targetX = currentCell.first;
                int targetY = currentCell.second - radius;
                if (targetY >= 0) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = 0 - targetY;
                    markVisited(targetX, targetY + cellsPastEdge);
                }
            }
            
            if (currentCell.first != maxX && currentCell.second != 0) { //move lower right if possible
                int targetX = currentCell.first + radius;
                int targetY = currentCell.second - radius;
                if (targetX <= maxX && targetY >= 0) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = std::max(targetX - maxX, 0 - targetY);
                    markVisited(targetX - cellsPastEdge, targetY + cellsPastEdge);
                }
            }
        }
    }
}

//explicit instantiation
template class searchWindow<double>;
template class searchWindow<float>;
