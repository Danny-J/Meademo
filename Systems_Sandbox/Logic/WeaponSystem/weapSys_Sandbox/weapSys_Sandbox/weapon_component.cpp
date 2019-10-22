#include <iomanip>

#include "weapon_component.h"

std::ostream& operator<<(std::ostream& os, weaponComponent::WEAP_COMPS_ENUMS key)
{
  switch (key)
  {
  case(weaponComponent::TRIGGER):
    os << "Trigger";
    break;
  case(weaponComponent::HAMMER):
    os << "Hammer";
    break;
  case(weaponComponent::ACTION):
    os << "Action";
    break;
  case(weaponComponent::FEEDPORT):
    os << "Feedport";
    break;
  case(weaponComponent::BOLT):
    os << "Bolt";
    break;
  case(weaponComponent::CHAMBER):
    os << "Chamber";
    break;
  case(weaponComponent::BARREL):
    os << "Barrel";
    break;
  case(weaponComponent::MUZZLE):
    os << "Muzzle";
    break;
  case(weaponComponent::MAGIZINE):
    os << "Magizine";
    break;
  default:
    os << "UNKNOWN";
    break;
  }

  return os;
}

bool weaponComponent::activateComponent()
{
  std::cout << "Activating " << wcType << ":" << std::endl;

  if (curStage == 0)
  {
    ++curStage;
  }

  while (curStage < weaponStages.size() + 1)
  {
    unsigned curStageMod = curStage - 1;

    if (
      weaponStages[curStageMod]->trigDirection == weaponStage::TD_ACTIVE
      || weaponStages[curStageMod]->trigDirection == weaponStage::TD_EITHER
      )
    {
      if (weaponStages[curStageMod]->activationStage(this) == false)
      {
        return false;
      }
    }

    ++curStage;
  }

  return true;
}

bool weaponComponent::readyComponent()
{
  std::cout << "Readying " << wcType << ":" << std::endl;

  if (curStage == weaponStages.size() + 1)
  {
    --curStage;
  }

  while (curStage > 0)
  {
    unsigned curStageMod = curStage - 1;

    if (
      weaponStages[curStageMod]->trigDirection == weaponStage::TD_READY
      || weaponStages[curStageMod]->trigDirection == weaponStage::TD_EITHER
      )
    {
      if (weaponStages[curStageMod]->activationStage(this) == false)
      {
        return false;
      }
    }

    --curStage;
  }

  return true;
}
