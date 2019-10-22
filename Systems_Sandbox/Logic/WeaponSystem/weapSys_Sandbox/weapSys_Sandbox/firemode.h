#ifndef FIREMODE_H
#define FIREMODE_H

#include <iostream>
#include <vector>
#include <string>

struct FireMode
{
  FireMode
  (
    std::string name_,
    unsigned roundsPerPull_,
    bool triggerCheck_,
    bool resetOnTriggerRelease_
  )
    : name(name_)
    , roundsPerPull(roundsPerPull_)
    , triggerCheck(triggerCheck_)
    , resetOnTriggerRelease(resetOnTriggerRelease_)

  {}

  std::string name;
  unsigned roundsPerPull; //0 implies full auto //infinity in a place of zero
  bool triggerCheck; //to check if trigger is activie before continuing on sequence
  bool resetOnTriggerRelease; //if(remainingRoundsPerPull == 0){/*reset anyway*/}
};

struct FireModes
{
  std::vector<FireMode> firemodes;
  unsigned firemodeIndex = 0;

  FireMode curFiremode()
  {
    return firemodes[firemodeIndex];
  }

  FireMode cycleFiremodes()
  {
    unsigned lastFireMode = firemodeIndex;

    ++firemodeIndex;

    if (firemodeIndex == firemodes.size())
    {
      firemodeIndex = 0;
    }

    FireMode firemodeToReturn = firemodes[firemodeIndex];

    std::cout << "*Changing fire modes: "
      << firemodes[lastFireMode].name
      << " -> "
      << firemodes[firemodeIndex].name
      << std::endl;

    return firemodeToReturn;
  }

  //TODO: Each firemode should have its own debug to dump all its info
  void debug()
  {
    std::cout << "*Current fire mode: "
      << firemodes[firemodeIndex].name << std::endl;
  }
};

#endif
