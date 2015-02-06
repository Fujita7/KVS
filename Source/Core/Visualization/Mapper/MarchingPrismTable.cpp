/*****************************************************************************/
/**
 *  @file   MarchingPrismTable.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "MarchingPrismTable.h"


namespace kvs
{

namespace MarchingPrismTable
{

const int TriangleID[64][10] =
{
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, //  0
    { 0, 6, 2,-1,-1,-1,-1,-1,-1,-1}, //  1
    { 0, 1, 7,-1,-1,-1,-1,-1,-1,-1}, //  2
    { 1, 6, 2, 1, 7, 6,-1,-1,-1,-1}, //  3

    { 1, 2, 8,-1,-1,-1,-1,-1,-1,-1}, //  4
    { 0, 1, 6, 1, 8, 6,-1,-1,-1,-1}, //  5
    { 0, 2, 7, 2, 8, 7,-1,-1,-1,-1}, //  6
    { 6, 8, 7,-1,-1,-1,-1,-1,-1,-1}, //  7

    { 3, 5, 6,-1,-1,-1,-1,-1,-1,-1}, //  8
    { 0, 3, 5, 0, 5, 2,-1,-1,-1,-1}, //  9
    { 0, 1, 7, 3, 5, 6,-1,-1,-1,-1}, // 10
    { 1, 5, 2, 1, 7, 5, 3, 5, 7,-1}, // 11

    { 1, 2, 8, 3, 5, 6,-1,-1,-1,-1}, // 12
    { 0, 3, 5, 0, 5, 1, 1, 5, 8,-1}, // 13
    { 0, 2, 7, 2, 8, 7, 3, 5, 6,-1}, // 14
    { 3, 5, 7, 5, 8, 7,-1,-1,-1,-1}, // 15

    { 3, 7, 4,-1,-1,-1,-1,-1,-1,-1}, // 16
    { 0, 6, 2, 3, 7, 4,-1,-1,-1,-1}, // 17
    { 0, 1, 3, 1, 4, 3,-1,-1,-1,-1}, // 18
    { 2, 3, 6, 2, 4, 3, 1, 4, 2,-1}, // 19

    { 3, 7, 4, 2, 8, 1,-1,-1,-1,-1}, // 20
    { 0, 6, 1, 1, 6, 8, 3, 7, 4,-1}, // 21
    { 0, 2, 3, 2, 8, 3, 3, 8, 4,-1}, // 22
    { 3, 6, 8, 3, 8, 4,-1,-1,-1,-1}, // 23

    { 5, 6, 7, 4, 5, 7,-1,-1,-1,-1}, // 24
    { 0, 7, 2, 2, 7, 5, 4, 5, 7,-1}, // 25
    { 0, 5, 6, 0, 1, 5, 1, 4, 5,-1}, // 26
    { 1, 5, 2, 1, 4, 5,-1,-1,-1,-1}, // 27

    { 5, 6, 7, 4, 5, 7, 1, 2, 8,-1}, // 28
    { 0, 7, 1, 4, 5, 8,-1,-1,-1,-1}, // 29
    { 0, 2, 6, 4, 5, 8,-1,-1,-1,-1}, // 30
    { 4, 5, 8,-1,-1,-1,-1,-1,-1,-1}, // 31

    { 4, 8, 5,-1,-1,-1,-1,-1,-1,-1}, // 32
    { 0, 6, 2, 4, 8, 5,-1,-1,-1,-1}, // 33
    { 0, 1, 7, 4, 8, 5,-1,-1,-1,-1}, // 34
    { 1, 6, 2, 1, 7, 6, 4, 8, 5,-1}, // 35

    { 1, 2, 5, 1, 5, 4,-1,-1,-1,-1}, // 36
    { 0, 5, 1, 1, 5, 4, 0, 6, 5,-1}, // 37
    { 0, 2, 7, 2, 5, 7, 4, 7, 5,-1}, // 38
    { 5, 7, 6, 4, 7, 5,-1,-1,-1,-1}, // 39

    { 3, 8, 6, 3, 4, 8,-1,-1,-1,-1}, // 40
    { 0, 3, 2, 2, 3, 8, 3, 4, 8,-1}, // 41
    { 0, 1, 6, 1, 8, 6, 3, 4, 7,-1}, // 42
    { 1, 8, 2, 3, 4, 7,-1,-1,-1,-1}, // 43

    { 1, 2, 3, 1, 3, 4,-1,-1,-1,-1}, // 44
    { 0, 3, 1, 1, 3, 4,-1,-1,-1,-1}, // 45
    { 0, 2, 6, 3, 4, 7,-1,-1,-1,-1}, // 46
    { 3, 4, 7,-1,-1,-1,-1,-1,-1,-1}, // 47

    { 3, 7, 5, 5, 7, 8,-1,-1,-1,-1}, // 48
    { 0, 6, 2, 3, 7, 5, 5, 7, 8,-1}, // 49
    { 0, 5, 3, 0, 8, 5, 0, 8, 1,-1}, // 50
    { 1, 8, 2, 3, 6, 5,-1,-1,-1,-1}, // 51

    { 2, 3, 7, 1, 2, 7,-1,-1,-1,-1}, // 52
    { 0, 7, 1, 3, 6, 5,-1,-1,-1,-1}, // 53
    { 0, 5, 3, 0, 2, 5,-1,-1,-1,-1}, // 54
    { 3, 6, 5,-1,-1,-1,-1,-1,-1,-1}, // 55

    { 6, 7, 8,-1,-1,-1,-1,-1,-1,-1}, // 56
    { 0, 7, 2, 2, 7, 8,-1,-1,-1,-1}, // 57
    { 0, 8, 6, 0, 1, 8,-1,-1,-1,-1}, // 58
    { 1, 8, 2,-1,-1,-1,-1,-1,-1,-1}, // 59

    { 2, 6, 7, 1, 2, 7,-1,-1,-1,-1}, // 60
    { 0, 7, 1,-1,-1,-1,-1,-1,-1,-1}, // 61
    { 0, 2, 6,-1,-1,-1,-1,-1,-1,-1}, // 62
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}  // 63
};

const int VertexID[9][2] =
{
    { 0, 1 },
    { 1, 2 },
    { 2, 0 },
    { 3, 4 },
    { 4, 5 },
    { 5, 3 },
    { 0, 3 },
    { 1, 4 },
    { 2, 5 }
};

} // end of namespace MarchingPrismTable

} // end of namespace kvs
