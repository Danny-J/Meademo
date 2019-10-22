#include "weapon_system.h"
#include "weapon_component_instances.h"

bool weapComp_Round::activateRound()
{
  bool roundActivated = false;

  if (HasRound())
  {
    std::cout << "*Activating round in:     " << wcType << std::endl;
    roundActivated = round->activateRound();
  }
  else
  {
    std::cout << "*No round to activate in: " << wcType << std::endl;
  }

  return roundActivated;
}

bool weapComp_Round::RoundTransferTo(weaponComponent::WEAP_COMPS_ENUMS key)
{
  return
    RoundTransfer(
      this, reinterpret_cast<weapComp_Round*>(parent->getWeapComp(key))
    );
}

bool weapComp_Round::RoundTransferFrom(weaponComponent::WEAP_COMPS_ENUMS key)
{
  return
    RoundTransfer(
      reinterpret_cast<weapComp_Round*>(parent->getWeapComp(key)), this
    );
}

bool weapComp_Round::RoundTransfer(weapComp_Round* srce, weapComp_Round* dest)
{
  //NOTE: This seperation of return logic allows for callers to create
  //      defined behavior depending on whether a weapComp (srce & dest equally)
  //      started with a round or not before making this function call 
  bool srceHasRound = srce->HasRound();
  bool destHasRound = dest->HasRound();

  //**DEBUG START**
  {
    if (srceHasRound)
    {
      std::cout
        << std::left << std::setw(26)
        << "*Round at source: "
        << srce->wcType
        << std::endl;
    }
    else
    {
      std::cout
        << std::left << std::setw(26)
        << "*No round at source: "
        << srce->wcType
        << std::endl;
    }

    if (destHasRound)
    {
      std::cout
        << std::left << std::setw(26)
        << "*Round already at destination: "
        << dest->wcType
        << std::endl;
    }
    else
    {
      std::cout
        << std::left << std::setw(26)
        << "*No round at destination: "
        << dest->wcType
        << std::endl;
    }
  }
  //***DEBUG END***

  if (srceHasRound)
  {
    if (destHasRound) //JAM
    {
      std::cout << "!Weapon Jam" << std::endl;

      return false;
    }
    else //if(!destHasRound) //TRANSFERRING
    {
      std::cout
        << std::left << std::setw(26)
        << "*Transferring round: "
        << srce->wcType
        << " -> "
        << dest->wcType
        << std::endl;

      //USE std::move
      dest->round = srce->round;
      srce->round = nullptr;

      return true;
    }
  }
  else //if(!srceHasRound)
  {
    if (destHasRound) //NOTHING, but round at source
    {
      return true;
    }
    else //if(!destHasRound) //NOTHING
    {
      return false;
    }
  }
}

void weapComp_Port::addComp()
{
  parent->addWeapComp(modCompCond);
}

void weapComp_Port::removeComp()
{
  parent->removeWeapComp(modCompCond);
}

bool weapComp_Port::IsObjectAttached()
{
  return parent->getWeapComp(modCompCond->weapCompAttacheType) != nullptr;
}

bool weapComp_Port::IsObjectDisattached()
{
  return parent->getWeapComp(modCompCond->weapCompAttacheType) == nullptr;
}

void weapComp_Port::debug()
{
  weaponComponent::debug();

  //std::cout << "*DEBUG START weapComp_Port*" << std::endl;

  if (parent->getWeapComp(modCompCond->weapCompAttacheType) != nullptr)
  {
    std::cout << "*Object is attached" << std::endl;

    parent->getWeapComp(modCompCond->weapCompAttacheType)->debug();
  }
  else
  {
    std::cout << "*No object is attached" << std::endl;
  }

  //std::cout << "**DEBUG END weapComp_Port**" << std::endl;
}
