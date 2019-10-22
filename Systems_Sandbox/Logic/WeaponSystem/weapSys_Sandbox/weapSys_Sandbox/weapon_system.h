#ifndef WEAPON_SYSTEM_H
#define WEAPON_SYSTEM_H

#include <queue>
#include "weapon_component.h"

struct weaponSystem
{
  weaponSystem(std::string name_) : name(name_)
  {
    weapComps.reserve(weaponComponent::COUNT);
    weapComps.resize(weaponComponent::COUNT); //Needed for Microsoft compiler
  }

  std::string name;

  void addWeapComp(modCompCondtional *modCompCond)
  {
    weaponComponent* weapComp = modCompCond->rulesForAdding();

    if (weapComp != nullptr)
    {
      addWeapComp(weapComp);
    }
  }

  void removeWeapComp(modCompCondtional *modCompCond)
  {
    if (modCompCond->rulesForRemoving())
    {
      removeWeapComp(modCompCond->weapCompAttacheType);
    }
  }

  unsigned weaponRoundCount();

private:
  std::vector<weaponComponent*> weapComps;
  typedef std::pair<weaponStage::triggerDirection,
    weaponComponent::WEAP_COMPS_ENUMS> setCompInfo;
  std::queue<setCompInfo> setCompQueue;
public:
  unsigned roundCyclesFired = 0;

public:
  weaponComponent* getWeapComp(weaponComponent::WEAP_COMPS_ENUMS key)
  {
    return weapComps[key];
  }

  void addWeapComp(weaponComponent* weapComp)
  {
    if (weapComp != nullptr)
    {
      weapComp->parent = this;

      //Valid type check
      if (static_cast<unsigned>(weapComp->wcType) < weapComps.capacity())
      {
        weapComps[weapComp->wcType] = weapComp;
      }
    }
  }

  void removeWeapComp(weaponComponent::WEAP_COMPS_ENUMS type)
  {
    //Free to World
    {
      //NOTE: For now that means deleting the object; no management of memory 
      delete weapComps[type];
    }
    weapComps[type] = nullptr;
  }

  void setComponent(weaponStage::triggerDirection condition,
    weaponComponent::WEAP_COMPS_ENUMS key);

  bool IsActive(weaponComponent::WEAP_COMPS_ENUMS key)
  {
    if (weapComps[key] != nullptr)
    {
      return weapComps[key]->IsActive();
    }

    return false;
  }

  bool IsReady(weaponComponent::WEAP_COMPS_ENUMS key)
  {
    if (weapComps[key] != nullptr)
    {
      return weapComps[key]->IsReady();
    }

    return false;
  }

  void debug();

  //private: //TEMP: DISABLED FOR DEVELOPMENT

    //RESEARCH: Give weapComp default weaponStages of Ready & Active
    //          These stages won't have user defined logic (i.e. script),
    //          but rather set logic to call a defined weapComp
    //          or weaponStage (set to either ready or active).
    //          Can still be set to nothing (e.g. compToActivate = nullptr)
  //TODO: Combine logic of 'activateComponent'
  //      and 'readyComponent' into single logic call
  bool activateComponent(weaponComponent::WEAP_COMPS_ENUMS key);

  bool readyComponent(weaponComponent::WEAP_COMPS_ENUMS key);
};

#endif
