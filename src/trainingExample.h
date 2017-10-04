/**
 * @file  trainingExample.h
 * RapidLib
 *
 * @author   Michael Zbyszynski
 * @date    22 Nov 2017
 * @copyright Copyright © 2016 Goldsmiths. All rights reserved.
 */

#ifndef trainingExample_h
#define trainingExample_h

#include <vector>
#include <string>

/** This is used by both NN and KNN models for training */
template<typename T>
struct trainingExample {
    std::vector<T> input;
    std::vector<T> output;
};

/** This is used by DTW models for training */
template<typename T>
struct trainingSeries {
    std::vector<std::vector<T> > input;
    std::string label;
};

#endif
