// This file is part of the AliceVision project and is made available under
// the terms of the MPL2 license (see the COPYING.md file).

#include "aliceVision/matchingImageCollection/pairBuilder.hpp"
#include "testing/testing.h"
#include "aliceVision/sfm/SfMData.hpp"
#include "aliceVision/sfm/View.hpp"

#include <iostream>
#include <algorithm>
#include <memory>

using namespace std;

using namespace aliceVision;

// Check pairs follow a weak ordering pair.first < pair.second
template<typename IterablePairs>
bool checkPairOrder(const IterablePairs & pairs)
{
  for (typename IterablePairs::const_iterator iterP = pairs.begin(); iterP != pairs.end();
    ++iterP)
  {
    if (iterP->first >= iterP->second)
      return false;
  }
  return true;
}

TEST(matchingImageCollection, exhaustivePairs)
{
  sfm::Views views;
  {
    // Empty
    PairSet pairSet = exhaustivePairs(views);
    EXPECT_EQ( 0, pairSet.size());
  }
  {
    std::vector<IndexT> indexes = {{ 12, 54, 89, 65 }};
    for( IndexT i: indexes )
    {
      views[i] = std::make_shared<sfm::View>("filepath", i);
    }


    PairSet pairSet = exhaustivePairs(views);
    EXPECT_TRUE( checkPairOrder(pairSet) );
    EXPECT_EQ( 6, pairSet.size());
    EXPECT_TRUE( pairSet.find(std::make_pair(12,54)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(12,89)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(12,65)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(54,89)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(54,65)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(65,89)) != pairSet.end() );
  }
}

TEST(matchingImageCollection, contiguousWithOverlap)
{
  sfm::Views views;
  {
    // Empty
    PairSet pairSet = contiguousWithOverlap(views, 1);
    EXPECT_EQ( 0, pairSet.size());
  }
  {
    std::vector<IndexT> indexes = {{ 12, 54, 65, 89 }};
    for( IndexT i: indexes )
    {
      views[i] = std::make_shared<sfm::View>("filepath", i);
    }

    PairSet pairSet = contiguousWithOverlap(views, 1);
    EXPECT_TRUE( checkPairOrder(pairSet) );
    EXPECT_EQ( 3, pairSet.size());
    EXPECT_TRUE( pairSet.find(std::make_pair(12, 54)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(54, 65)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(65, 89)) != pairSet.end() );
  }
  {
    std::vector<IndexT> indexes = {{ 11, 12, 54, 65, 89, 99 }};
    for( IndexT i: indexes )
    {
      views[i] = std::make_shared<sfm::View>("filepath", i);
    }

    PairSet pairSet = contiguousWithOverlap(views, 3);
    EXPECT_TRUE( checkPairOrder(pairSet) );
    EXPECT_EQ( 12, pairSet.size());
    EXPECT_TRUE( pairSet.find(std::make_pair(11, 12)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(11, 54)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(11, 65)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(12, 54)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(12,65)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(12,89)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(54,65)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(54,89)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(54,99)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(65,89)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(65,99)) != pairSet.end() );
    EXPECT_TRUE( pairSet.find(std::make_pair(89,99)) != pairSet.end() );
  }
}

TEST(matchingImageCollection, IO)
{
  PairSet pairSetGT;
  pairSetGT.insert( std::make_pair(0,1) );
  pairSetGT.insert( std::make_pair(1,2) );
  pairSetGT.insert( std::make_pair(2,0) );

  PairSet pairSetGTsorted;
  pairSetGTsorted.insert( std::make_pair(0,1) );
  pairSetGTsorted.insert( std::make_pair(0,2) );
  pairSetGTsorted.insert( std::make_pair(1,2) );

  EXPECT_TRUE( savePairs("pairsT_IO.txt", pairSetGT));

  PairSet loaded_Pairs;
  EXPECT_TRUE( loadPairs("pairsT_IO.txt", loaded_Pairs));
  EXPECT_TRUE( std::equal(loaded_Pairs.begin(), loaded_Pairs.end(), pairSetGTsorted.begin()) );
}

/* ************************************************************************* */
int main() { TestResult tr; return TestRegistry::runAllTests(tr);}
/* ************************************************************************* */
