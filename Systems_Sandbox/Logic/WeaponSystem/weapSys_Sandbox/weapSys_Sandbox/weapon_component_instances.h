#ifndef WEAPON_COMPONENT_INSTANCES_H
#define WEAPON_COMPONENT_INSTANCES_H

#include "firemode.h"
#include "weapon_component.h"

//TEMP: For development
static FireMode semi("Semi-Auto", 1, false, true);
static FireMode burst3("Burst 3", 3, true, true);
static FireMode full("Full-Auto", 0, true, true);

//Each weaponStage of the weaponComponent contain their unique firemode
//that will affect the firemode of the weapComp_Action
struct weapComp_Action : public weaponComponent
{
  weapComp_Action(WEAP_COMPS_ENUMS wcType_)
    : weaponComponent(wcType_)
  {
    firemodes.firemodes.push_back(semi);
    firemodes.firemodes.push_back(burst3);
    firemodes.firemodes.push_back(full);
  }

  virtual ~weapComp_Action() {}

  FireMode curFiremode()
  {
    return firemodes.curFiremode();
  }

  FireModes firemodes;

  unsigned remainingRoundsPerPull; //once reached 0, end sequence

  void debug()
  {
    weaponComponent::debug();
    firemodes.debug();
  }
};

struct ws_projectile
{
  //BLANK
};

struct ws_round
{
  friend struct weapComp_Round; //friend declaration

  ws_projectile *projectile = new ws_projectile;

  ~ws_round()
  {
    if (projectile != nullptr)
    {
      delete projectile;
    }
  }

private:
  bool activateRound()
  {
    if (projectile != nullptr)
    {
      std::cout << "*ActivatingRound" << std::endl;
      delete projectile;
      projectile = nullptr;

      return true;
    }

    return false;
  }
};

//Weapon Component that can contain a ws_round(s)
struct weapComp_Round : public weaponComponent
{
  weapComp_Round(WEAP_COMPS_ENUMS wcType_)
    : weaponComponent(wcType_)
  {}

  virtual ~weapComp_Round() {}

  void DestoryRound()
  {
    if (HasRound())
    {
      delete round;
      round = nullptr;
    }
  }

  bool HasRound()
  {
    return round != nullptr;
  }

  bool RoundTransferTo(weaponComponent::WEAP_COMPS_ENUMS key);
  bool RoundTransferFrom(weaponComponent::WEAP_COMPS_ENUMS key);
  static bool RoundTransfer(weapComp_Round* srce, weapComp_Round* dest);

  bool activateRound();

  virtual void debug() override
  {
    weaponComponent::debug();

    //std::cout << "*DEBUG START weapComp_Round*" << std::endl;

    if (round == nullptr)
    {
      std::cout << "Round NOT in: " << wcType << std::endl;
    }
    else
    {
      std::cout << "Round IS in:  " << wcType << std::endl;
    }

    //std::cout << "**DEBUG END weapComp_Round**" << std::endl;
  }

protected:
  ws_round* round = nullptr;
};

struct weapComp_Mag : public weapComp_Round
{
  weapComp_Mag(WEAP_COMPS_ENUMS wcType_, unsigned maxRoundCount_)
    : weapComp_Round(wcType_), maxRoundCount(maxRoundCount_)
  {
  }

  ws_round roundToSet; //DEBUG

  unsigned magRoundCount()
  {
    unsigned roundCountTotal = roundCount;

    if (HasRound())
    {
      ++roundCountTotal;
    }

    return roundCountTotal;
  }

  unsigned magMaxCapacity()
  {
    return maxRoundCount;
  }

  bool IsFull()
  {
    return magRoundCount() == magMaxCapacity();
  }

  bool IsEmpty()
  {
    return magRoundCount() == 0;
  }

  void Reset()
  {
    std::cout << "Resetting: " << wcType << std::endl;

    roundCount = maxRoundCount;

    if (HasRound() == false)
    {
      SetRoundInternally();
    }
  }

  bool ReadyRound()
  {
    std::cout << "Readying Round" << std::endl;

    bool hasInternalRound = HasRound();
    bool hasReserveRounds = (roundCount > 0);

    if (hasInternalRound)
    {
      std::cout << "*Round already set to ready" << std::endl;

      if (hasReserveRounds)
      {
        std::cout << "*More Rounds Ready" << std::endl;
        return false;
      }
      else
      {
        std::cout << "*Last round in Mag" << std::endl;
        return true;
      }
    }
    else
    {
      if (hasReserveRounds)
      {
        std::cout << "*Setting round to ready" << std::endl;
        SetRoundInternally();
        return true;
      }
      else
      {
        std::cout << "*Magizine empty" << std::endl;
        return false;
      }
    }
  }

  void debug() override
  {
    weapComp_Round::debug();

    //TODO: Move logic in else to front of condition calls
    if (IsFull())
    {
      std::cout << "*Magizine FULL" << std::endl;
    }
    else if (IsEmpty())
    {
      std::cout << "*Magizine EMPTY" << std::endl;
    }
    else //Somewhere inbetween
    {
      std::cout
        << "*"
        << "Rounds in Magizine: "
        << magRoundCount()
        << " out of "
        << magMaxCapacity()
        << std::endl;
    }
  }

private:
  void SetRoundInternally()
  {
    --roundCount;
    round = new ws_round;
  }

private:
  //std::vector<ws_round*> roundsContained; //SUSPENDED
  unsigned roundCount = 0;
  unsigned maxRoundCount = 1;
};

//Responsible for making REQUESTS to the system to add & remove set weaponComponents
struct weapComp_Port : public weaponComponent
{
  weapComp_Port(
    WEAP_COMPS_ENUMS wcType_,
    modCompCondtional* modCompCond_
  )
    : weaponComponent(wcType_)
    , modCompCond(modCompCond_)
  {
    modCompCond->parent = this;
    //modCompCond(modCompCond_, this); //WIP
  }

  virtual ~weapComp_Port() {}

  modCompCondtional *modCompCond = nullptr;

  //void connectComp();

  void addComp();
  void removeComp();
  bool IsObjectAttached();
  bool IsObjectDisattached();
  void debug() override;
};

#endif
