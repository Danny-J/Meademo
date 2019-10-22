#include "weapon_component.h"
#include "weapon_component_instances.h"
#include "weapon_system.h"

bool weaponSystem::setComponent(weaponStage::triggerDirection condition,
  weaponComponent::WEAP_COMPS_ENUMS key)
{
  bool countCycle = false;

  //DEBUG
  {
    std::cout << "@Setting " << key << " to ";

    if (condition == weaponStage::TD_ACTIVE)
      std::cout << "ACTIVE" << std::endl;

    if (condition == weaponStage::TD_READY)
      std::cout << "READY" << std::endl;
  }

  //If queue is not empty, that means the queue is already processing info
  bool QueueAlreadyProcessing = (setCompQueue.empty() == false);

  setCompQueue.push(std::make_pair(condition, key));

  if (QueueAlreadyProcessing)
  {
    //this return will be made internally
      //no external call will get this return
    //since queue is already processing
    return true; //keep queue running
  }

  while (setCompQueue.empty() == false)
  {
    setCompInfo curComp = setCompQueue.front();
    weaponStage::triggerDirection const & trigDir = curComp.first;
    weaponComponent::WEAP_COMPS_ENUMS const & wce = curComp.second;

    if (trigDir == weaponStage::TD_READY)
    {
      //TODO: Move this logic to within weapComp_Action
        //NOTE: Goal to distinguish between each completed cycle in auto fire
      if (countCycle && wce == weaponComponent::ACTION)
      {
        std::cout << "Completed Round Cycle: "
          << ++roundCyclesFired << std::endl;
      }

      //REVIEW: If weapCompenent exists
      if (weapComps[wce] != nullptr)
        if (readyComponent(wce) == false)
        {
          while (setCompQueue.empty() == false)
          {
            setCompQueue.pop();
          }

          return false;
        }
    }
    else if (trigDir == weaponStage::TD_ACTIVE)
    {
      //NOTE: Goal to distinguish between each completed cycle in auto fire
      if (wce == weaponComponent::ACTION)
      {
        if (countCycle == false)
        {
          roundCyclesFired = 0;
          countCycle = true;
        }
        std::cout << "\nFiring Round Cycle: "
          << (roundCyclesFired + 1) << std::endl;
      }

      //REVIEW: If weapCompenent exists
      if (weapComps[wce] != nullptr)
        if (activateComponent(wce) == false)
        {
          while (setCompQueue.empty() == false)
          {
            setCompQueue.pop();
          }

          return false;
        }
    }

    setCompQueue.pop();
  }

  return true;
}

bool weaponSystem::activateComponent(weaponComponent::WEAP_COMPS_ENUMS key)
{
  weaponComponent* thisComp = weapComps[key];

  if (thisComp != nullptr)
  {
    if (thisComp->activateComponent())
    {
      std::cout
        << "***SUCCESS: "
        << thisComp->wcType
        << " ACTIVATED"
        << std::endl;

      weaponComponent* nextComp = nullptr;

      if (weapComps[key]->wcTypeToActivate < weaponComponent::COUNT)
      {
        nextComp = weapComps[weapComps[key]->wcTypeToActivate];
      }

      if (nextComp != nullptr)
      {
        std::cout
          << "**" << thisComp->wcType
          << " is set to activate "
          << nextComp->wcType
          << std::endl;

        if (
          nextComp->conditionToActivate == weaponStage::TD_EITHER
          || (nextComp->conditionToActivate == weaponStage::TD_READY
            && nextComp->IsReady())
          || (nextComp->conditionToActivate == weaponStage::TD_ACTIVE
            && nextComp->IsActive())
          )
        {
          setComponent(weaponStage::TD_ACTIVE, nextComp->wcType);
        }
        else
        {
          std::cout
            << nextComp->wcType
            << " did not satisfy condition to be activated"
            << std::endl;

          std::cout << "Condition needs to be: ";

          switch (nextComp->conditionToActivate)
          {
          case(weaponStage::TD_READY):
            std::cout << "READY" << std::endl;
            break;
          case(weaponStage::TD_ACTIVE):
            std::cout << "ACTIVE" << std::endl;
            break;
          case(weaponStage::TD_EITHER):
            std::cout << "EITHER; this maybe an error..." << std::endl;
            break;
          default:;
          }
        }
      }
      else //DEBUG
      {
        //std::cout
        //<< thisComp->wcType
        //<< " has no component to activate"
        //<< std::endl;
      }

      return true;
    }
    else
    {
      std::cout
        << "*NOTICE:  "
        << thisComp->wcType
        << " HAS NOT ACTIVATED"
        << std::endl;
    }
  }

  return false;
}

bool weaponSystem::readyComponent(weaponComponent::WEAP_COMPS_ENUMS key)
{
  weaponComponent* thisComp = weapComps[key];

  if (thisComp != nullptr)
  {
    if (thisComp->readyComponent())
    {
      std::cout
        << "***SUCCESS: "
        << thisComp->wcType
        << " READIED"
        << std::endl;

      weaponComponent* nextComp = nullptr;

      if (weapComps[key]->wcTypeToReady < weaponComponent::COUNT)
      {
        nextComp = weapComps[weapComps[key]->wcTypeToReady];
      }

      if (nextComp != nullptr)
      {
        std::cout
          << "**" << thisComp->wcType
          << " is set to ready "
          << nextComp->wcType
          << std::endl;

        if (
          nextComp->conditionToReady == weaponStage::TD_EITHER
          || (nextComp->conditionToReady == weaponStage::TD_READY
            && nextComp->IsReady())
          || (nextComp->conditionToReady == weaponStage::TD_ACTIVE
            && nextComp->IsActive())
          )
        {
          setComponent(weaponStage::TD_READY, nextComp->wcType);
        }
        else
        {
          //DEBUG
          {
            std::cout
              << nextComp->wcType
              << " did not satisfy condition to be readied"
              << std::endl;

            std::cout << "Condition needs to be: ";

            switch (nextComp->conditionToReady)
            {
            case(weaponStage::TD_READY):
              std::cout << "READY" << std::endl;
              break;
            case(weaponStage::TD_ACTIVE):
              std::cout << "ACTIVE" << std::endl;
              break;
            case(weaponStage::TD_EITHER):
              std::cout << "EITHER; this maybe an error..." << std::endl;
              break;
            default:;
            }
          }
        }
      }
      else
      {
        //DEBUG
        {
          //std::cout
          //<< thisComp->wcType
          //<< " has no component to ready"
          //<< std::endl;
        }
      }

      return true;
    }
    else
    {
      //DEBUG
      {
        std::cout
          << "*NOTICE:  "
          << thisComp->wcType
          << " HAS NOT READIED"
          << std::endl;
      }
    }
  }

  return false;
}

//TODO: Should call debug of all weaponComps
void weaponSystem::debug()
{
  std::cout << "**DEBUG START: weaponSystem**" << std::endl;

  for (
    weaponComponent::WEAP_COMPS_ENUMS i
    = static_cast<weaponComponent::WEAP_COMPS_ENUMS>(0);
    i < weaponComponent::COUNT;
    i = weaponComponent::WEAP_COMPS_ENUMS(static_cast<int>(i) + 1)
    )
  {
    if (weapComps[i] != nullptr)
    {
      //std::cout << "**DEBUG START: WeapComp**" << std::endl;

      weapComps[i]->debug();

      //std::cout << "***DEBUG END: WeapComp***" << std::endl;
    }
    else
    {
      //TODO: Replace 'i' with a enumToString translation
      std::cout << "*This WeapSys is missing WeapComp type of: " << i << std::endl;
    }
  }

  std::cout << "***DEBUG END: weaponSystem***" << std::endl;
}

unsigned weaponSystem::weaponRoundCount()
{
  unsigned roundCountTotal = 0;

  if (reinterpret_cast<weapComp_Mag*>(weapComps[weaponComponent::MAGIZINE]) == nullptr)
  {
    //std::cout << "*No Magizine" << std::endl;
  }
  else
  {
    //reinterpret_cast<weapComp_Mag*>(weapComps[weaponComponent::MAGIZINE])->debug();
    roundCountTotal += reinterpret_cast<weapComp_Mag*>(weapComps[weaponComponent::MAGIZINE])->magRoundCount();
  }

  if (reinterpret_cast<weapComp_Mag*>(weapComps[weaponComponent::CHAMBER]) == nullptr)
  {
    //std::cout << "*No Chamber" << std::endl;
  }
  else
  {
    //reinterpret_cast<weapComp_Mag*>(weapComps[weaponComponent::CHAMBER])->debug();
    roundCountTotal += reinterpret_cast<weapComp_Round*>(
      weapComps[weaponComponent::CHAMBER])->HasRound(); //HACK
  }

  if (reinterpret_cast<weapComp_Mag*>(weapComps[weaponComponent::BOLT]) == nullptr)
  {
    //std::cout << "*No Bolt" << std::endl;
  }
  else
  {
    //reinterpret_cast<weapComp_Mag*>(weapComps[weaponComponent::BOLT])->debug();
    roundCountTotal += reinterpret_cast<weapComp_Round*>(
      weapComps[weaponComponent::BOLT])->HasRound(); //HACK
  }

  return roundCountTotal;
}
