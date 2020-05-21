#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#include <iostream>
#include <iomanip>
#include <vector>

//#include "weapon_system.h"
struct weaponSystem;

//Defines the behavior of the weaponComponent it is inserted into
struct weaponStage
{
  //RENAME: TriggerDestination
  enum triggerDirection
  {
    TD_ACTIVE,
    TD_READY,
    TD_EITHER,
    //TD_NOT_ACTIVE,
    //TD_NOT_READY,
    TD_COUNT //NEEDS TO REMAIN LAST
  };

  weaponStage(triggerDirection trigDirection_)
    : trigDirection(trigDirection_)
  {}

  //std::string name;         //UNUSED
  //float triggerPos;         //SUSPENDED //value from 0.0f to 1.0f
  //float rateOfChangeFactor; //SUSPENDED //rate at which to APPROACH this stage
  //unsigned rateOfFire;      //TEMP //MOVED to WeaponSystem

    //TODO: DEPRECATE
  virtual bool activationStage(struct weaponComponent* thisComp) = 0;
  triggerDirection trigDirection;

  // changeInPos / time(seconds)
//void rateOfPosChangeFixed(float);    //SUSPENDED

  // seconds to get to newPos; (newPos - curPos) / time[in seconds]
//void rateOfPosChangeRelative(float); //SUSPENDED
};

//can be affected externally from system or by other weapon components
struct weaponComponent
{
  //TODO: add debug() so all weaponComponents can print
  //      debug info relative to debugging their use cases

  enum WEAP_COMPS_ENUMS
  {
    TRIGGER,  //0
    HAMMER,   //1
    ACTION,   //2
    FEEDPORT, //3
    BOLT,     //4
    CHAMBER,  //5
    BARREL,   //6
    MUZZLE,   //7
    MAGIZINE, //8
    COUNT     //COUNT
  };

  friend std::ostream& operator<<(std::ostream& os, WEAP_COMPS_ENUMS key);

  weaponComponent(WEAP_COMPS_ENUMS wcType_)
    : wcType(wcType_)
  {}

  virtual ~weaponComponent() {}

  friend struct weaponSystem;
  weaponSystem* parent = nullptr;
  //std::string name;
  //float curPos;                  //SUSPENDED
  //float curRateOfChange;         //SUSPENDED
  //void activateComponent(float); //SUSPENDED //value to set for curPos

  std::vector<weaponStage*> weaponStages; //sorted array using 'triggerPos' as key
  unsigned curStage = 0;
  WEAP_COMPS_ENUMS wcType;

  WEAP_COMPS_ENUMS wcTypeToActivate = weaponComponent::COUNT;
  weaponStage::triggerDirection conditionToActivate = weaponStage::TD_EITHER;

  WEAP_COMPS_ENUMS wcTypeToReady = weaponComponent::COUNT;
  weaponStage::triggerDirection conditionToReady = weaponStage::TD_EITHER;

  void setCompToActivate(WEAP_COMPS_ENUMS wcTypeToAct,
    weaponStage::triggerDirection trigDirCondition)
  {
    std::cout
      << std::left << std::setw(7)
      << wcType << " is setting: "
      << std::left << std::setw(7)
      << wcTypeToAct
      << " as component to ACTIVATE" << std::endl;

    wcTypeToActivate = wcTypeToAct;
    conditionToActivate = trigDirCondition;
  }

  void setCompToReady(WEAP_COMPS_ENUMS wcTypeToRdy,
    weaponStage::triggerDirection trigDirCondition)
  {
    std::cout
      << std::left << std::setw(7)
      << wcType << " is setting: "
      << std::left << std::setw(7)
      << wcTypeToRdy
      << " as component to READY" << std::endl;

    wcTypeToReady = wcTypeToRdy;
    conditionToReady = trigDirCondition;
  }

  //DEPRECATING
#if 0
  //param compToActivate_:  component to activate
  //param trigDirCondition: state component needs to be in for use
  void CompToActivate(weaponComponent* compToActivate_,
    weaponStage::triggerDirection trigDirCondition)
  {
    std::cout
      << std::left << std::setw(7)
      << wcType << " is setting: "
      << std::left << std::setw(7)
      << compToActivate_->wcType
      << " as component to ACTIVATE" << std::endl;

    compToActivate = compToActivate_;
    conditionToActivate = trigDirCondition;
  }

  void CompToReady(weaponComponent* compToReady_,
    weaponStage::triggerDirection trigDirCondition)
  {
    std::cout
      << std::left << std::setw(7)
      << wcType << " is setting: "
      << std::left << std::setw(7)
      << compToReady_->wcType
      << " as component to READY" << std::endl;

    compToReady = compToReady_;
    conditionToReady = trigDirCondition;
  }
#endif

  bool IsReady()
  {
    return curStage == 0;
  }

  bool IsActive()
  {
    return curStage == (weaponStages.size() + 1);
  }

  virtual void debug()
  {
    std::cout
      << "Name: "
      << std::left << std::setw(8)
      << wcType
      << " \\\\ Status: ";

    if (IsReady())
    {
      std::cout << "READY" << std::endl;
    }
    else if (IsActive())
    {
      std::cout << "ACTIVE" << std::endl;
    }
    else
    {
      std::cout << "SOMEWHERE IN-BETWEEN" << std::endl;
    }
  }

protected:
  bool activateComponent();
  bool readyComponent();
};

struct modCompCondtional
{
  modCompCondtional(weaponComponent::WEAP_COMPS_ENUMS weapCompAttacheType_)
    : weapCompAttacheType(weapCompAttacheType_)
  {}

#if 0
  modCompCondtional(const modCompCondtional *other,
    weaponComponent* parent_)
    : modCompCondtional(*other)
  {
    parent = parent_;
  }
#endif

  const weaponComponent::WEAP_COMPS_ENUMS weapCompAttacheType;
  weaponComponent* parent = nullptr;
  virtual weaponComponent* rulesForAdding() = 0;
  virtual bool rulesForRemoving() = 0;
};

#endif
