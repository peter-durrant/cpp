// Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "KdTree.h"

int main()
{
    std::valarray <InputOutputDataPoint> data = {
        { { 1,2,3,4 },{ 1 } },
        { { 2,2,3,4 },{ 2 } },
        { { 1,2,3,4 },{ 3 } },
        { { 3,5,3,4 },{ 4 } },
        { { 1,7,1,14 },{ 5 } } };
    KdTree<InputOutputDataPoint> kdTree(data);
    return 0;
}

