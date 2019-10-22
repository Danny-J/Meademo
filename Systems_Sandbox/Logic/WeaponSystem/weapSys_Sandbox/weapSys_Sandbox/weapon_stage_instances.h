#ifndef WEAPON_STAGE_INSTANCES_H
#define WEAPON_STAGE_INSTANCES_H

//#include "weapon_stage_instances.h"
//#include "weapon_component.h"
#include "weapon_component_instances.h"

////////////////////////////////////////////////////////////////////////////////
//***LOGIC***

//NOTE: **For rifle auto; could differ for other weapon types
//Trigger: ready when released; activates action**
  //releasing(TD_READY);
  //pulling(TD_ACTIVE);
//Hammer: ready when cocked; activates chamber**
  //cocking(TD_READY);
  //engaging(TD_ACTIVE);
//Action: uses firemodes; activates hammer**
  //reacting(TD_READY);
  //actioning(TD_ACTIVE);
//Feedport: ready when unloaded
  //unloading(TD_READY);
  //loading(TD_ACTIVE);
//Bolt: ready when opened
  //opening(TD_READY);    //BOLT
  //feeding(TD_ACTIVE);   //Round TO   BOLT; FROM MAG
  //ejecting(TD_READY);   //Round FROM BOLT; TO   EJECTION
  //chambing(TD_ACTIVE);  //Round TO   CHAMBER; FROM BOLT
  //extracting(TD_READY); //Round FROM CHAMBER; TO   BOLT
  //closing(TD_ACTIVE);   //BOLT
//Chamber: ready when emptied
  //emptying(TD_READY);
  //firing(TD_ACTIVE); //was in Round
//Magizine: ready when round is ready for transfer
  //cyclingInternally(TD_READY);
  //cyclingExternally(TD_ACTIVE);

//EJECTIONPORT
  //NO STAGES

////////////////////////////////////////
//==TRIGGER==
  //pulling(TD_ACTIVE);
  //releasing(TD_READY);

//Pulling
struct weapStage_pulling : public weaponStage
{
  weapStage_pulling() : weaponStage(TD_ACTIVE)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Pulling" << std::endl;

    weapComp_Action* compAction = reinterpret_cast<weapComp_Action*>(
      thisComp->parent->getWeapComp(weaponComponent::ACTION));

    if (compAction->curFiremode().resetOnTriggerRelease)
    {
      compAction->remainingRoundsPerPull = compAction->curFiremode().roundsPerPull;
    }

    return true;
  }
};

//Releasing
struct weapStage_releasing : public weaponStage
{
  weapStage_releasing() : weaponStage(TD_READY)
  {}

  bool activationStage(struct weaponComponent* /* thisComp */)
  {
    std::cout << "Releasing" << std::endl;

    return true;
  }
};

////////////////////////////////////////
//==HAMMER==
  //cocking(TD_READY);
  //engaging(TD_ACTIVE);

//*Cocking
//  -ready hammer (animation + sound)
struct weapStage_cocking : public weaponStage
{
  weapStage_cocking() : weaponStage(TD_READY)
  {}

  bool activationStage(struct weaponComponent* /* thisComp */)
  {
    /*
      playAnimation(HammerPos<animation>(hammerPosFullCock));
      playSound(HammerPos<sound>(hammerPosFullCock));

      state.next();
    */

    std::cout << "Cocking" << std::endl;

    return true;
  }
};

//*Engaging
//  -engage hammer (animation + sound)
struct weapStage_engaging : public weaponStage
{
  weapStage_engaging() : weaponStage(TD_ACTIVE)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Engaging" << std::endl;

    if (thisComp->parent->getWeapComp(weaponComponent::BOLT)->IsActive() == false)
    {
      std::cout << "*Bolt is not activated" << std::endl;
      return false;
    }

    return true;
  }
};

////////////////////////////////////////
//==ACTION==
  //readying(TD_READY);
  //actioning(TD_ACTIVE);

//Reacting:
struct weapStage_reacting : public weaponStage
{
  weapStage_reacting() : weaponStage(TD_READY)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Reacting" << std::endl;

    weapComp_Action* thisAction = reinterpret_cast<weapComp_Action*>(thisComp);

    //TODO: Add logic for proposition thisAction->curFiremode().resetOnTriggerRelease

    if (thisAction->curFiremode().triggerCheck &&
      thisComp->parent->getWeapComp(weaponComponent::TRIGGER)->IsActive() == false)
    {
      std::cout << "*Trigger is not active" << std::endl;
      return true;
    }

    thisAction->parent->setComponent(TD_ACTIVE, weaponComponent::BOLT);

    if (thisAction->remainingRoundsPerPull > 0
      || thisAction->curFiremode().roundsPerPull == 0) //i.e. is full auto
    {
      thisComp->parent->setComponent(TD_ACTIVE, thisComp->wcType);
      std::cout << "*Re-Actioning" << std::endl;
      //return false; //EXPERIMENTAL
    }
    //else //EXPERIMENTAL
    {
      //TODO: can set as CompToReady
    //thisAction->parent->setComponent(TD_READY, weaponComponent::TRIGGER);
      return true;
    }
  }
};

//Actioning:
struct weapStage_actioning : public weaponStage
{
  weapStage_actioning() : weaponStage(TD_ACTIVE)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Actioning" << std::endl;

    weapComp_Action* thisAction = reinterpret_cast<weapComp_Action*>(thisComp);

    if (thisAction->remainingRoundsPerPull == 0)
    {
      if (thisAction->curFiremode().roundsPerPull > 0) //i.e. not full auto
      {
        thisComp->parent->setComponent(TD_READY, thisComp->wcType);

        std::cout << "*No more rounds remaining this pull" << std::endl;
        return false;
      }
    }
    else
    {
      --thisAction->remainingRoundsPerPull;
    }

    return true;
  }
};

////////////////////////////////////////
//==FEEDPORT==
  //unloading(TD_READY);
  //loading(TD_ACTIVE);

//Unloading:
struct weapStage_unloading : public weaponStage
{
  weapStage_unloading() : weaponStage(TD_READY)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Unloading" << std::endl;

    reinterpret_cast<weapComp_Port*>(thisComp)->removeComp();

    return true;
  }
};

//Loading:
struct weapStage_loading : public weaponStage
{
  weapStage_loading() : weaponStage(TD_ACTIVE)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Loading" << std::endl;

    //NOTE: How to take magizine (a weapon component)
    //external to the system to within the system?
      //ANSWER: There may be logic required at the weaponSystem level
      //for adding & removing weaponComponents to & from the system 

    reinterpret_cast<weapComp_Port*>(thisComp)->addComp();

    return true;
  }
};

////////////////////////////////////////
//==BOLT==
  //opening(TD_READY);
  //feeding(TD_ACTIVE);
  //ejecting(TD_READY);
  //chambing(TD_ACTIVE); //was in Chamber //take round from BOLT
  //extracting(TD_READY);
  //closing(TD_ACTIVE);

//Opening:
struct weapStage_opening : public weaponStage
{
  weapStage_opening() : weaponStage(TD_READY)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Opening" << std::endl;

    thisComp->parent->setComponent(TD_READY, weaponComponent::MAGIZINE);

    return true;
  }
};

//Feeding:
//  -bolt beginning to close (animation + sound)
//  -transitionRoundFromMagToBolt (transfer of object)
struct weapStage_feeding : public weaponStage
{
  weapStage_feeding() : weaponStage(TD_ACTIVE)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    /*
      animation* boltAnimation = BoltPos<animation>(boltPosFeeding);
      BindEvent(boltAnimation, END, transitionRoundFromMagToBolt);

      playAnimation(boltAnimation);
      playSound(BoltPos<sound>(boltPosFeeding));
    */

    std::cout << "Feeding" << std::endl;

    //reinterpret_cast<weapComp_Round*>(thisComp)->debug(); //DEBUG

    bool returnVal = thisComp->parent->activateComponent(weaponComponent::MAGIZINE);

    //TEMP: forcing success
    if (returnVal == false)
    {
      std::cout
        << "*FORCING Success; result was "
        << std::boolalpha
        << returnVal
        << std::endl;
      returnVal = true; //DESIRED_EFFECT
    }

    //reinterpret_cast<weapComp_Round*>(thisComp)->debug(); //DEBUG

    return returnVal;
  }
};

//Ejecting:
//  -ejectRoundFromBolt (sound + particle effects + spawning object)
struct weapStage_ejecting : public weaponStage
{
  weapStage_ejecting() : weaponStage(TD_READY)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    /*
      playSound(firearm.ejectionSound);
      spawnParticleEffect(round.ejectionParticleEffects);
      spawnObject(bolt.Round);
      bolt.Round = NULL;
    */

    std::cout << "Ejecting" << std::endl;

    if (
      reinterpret_cast<weapComp_Round*>(
        thisComp->parent->getWeapComp(weaponComponent::BOLT))->HasRound()
      )
    {
      reinterpret_cast<weapComp_Round*>(
        thisComp->parent->getWeapComp(weaponComponent::BOLT))->DestoryRound();

      //reinterpret_cast<weapComp_Round*>(
      //  thisComp->parent->weapComps[weaponComponent::BOLT])->RoundTransferTo(nullptr);
    }

    return true;
  }
};

//Chambering:
//  -bolt finsihing to close (animation + sound)
//  -transitionRoundFromBoltToChamber (transfer of object)
struct weapStage_chambing : public weaponStage
{
  weapStage_chambing() : weaponStage(TD_ACTIVE)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    /*
      animation* boltAnimation = BoltPos<animation>(boltPosChambering);
      BindEvent(boltAnimation, END, transitionRoundFromBoltToChamber);

      playAnimation(boltAnimation);
      playSound(BoltPos<sound>(boltPosChambering));
    */

    std::cout << "Chambing" << std::endl;

    bool returnVal = reinterpret_cast<weapComp_Round*>(thisComp)->RoundTransferTo(
      weaponComponent::CHAMBER);
    //reinterpret_cast<weapComp_Round*>(thisComp->parent->weapComps[weaponComponent::CHAMBER]));

  //TEMP: forcing success
    if (returnVal == false)
    {
      std::cout << "*FORCING Success; result was " << std::boolalpha << returnVal << std::endl;
      returnVal = true; //DESIRED_EFFECT
    }

    return returnVal;
  }
};

//Extracting:
//  -transitionRound(Remains)FromChamberToBolt (transfer of object)
struct weapStage_extracting : public weaponStage
{
  weapStage_extracting() : weaponStage(TD_READY)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    /*
      animation* boltAnimation = BoltPos<animation>(boltPosExtracting);
      BindEvent(boltAnimation, END, transitionRoundFromChamberToBolt);

      playAnimation(boltAnimation);
      playSound(BoltPos<sound>(boltPosExtracting));

      //state.next(); //DEPRECATED
    */

    std::cout << "Extracting" << std::endl;

    thisComp->parent->readyComponent(weaponComponent::CHAMBER/* false */);
    //thisComp->parent->setComponent(TD_READY, weaponComponent::CHAMBER);

    return true;
  }
};

//Closing
struct weapStage_closing : public weaponStage
{
  weapStage_closing() : weaponStage(TD_ACTIVE)
  {}

  bool activationStage(struct weaponComponent* /* thisComp */)
  {
    std::cout << "Closing" << std::endl;

    return true;
  }
};

////////////////////////////////////////
//==CHAMBER==
  //emptying(TD_READY);
  //firing(TD_ACTIVE); //was in Round

//Emptying:
struct weapStage_emptying : public weaponStage
{
  weapStage_emptying() : weaponStage(TD_READY)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Emptying" << std::endl;

    bool returnVal = reinterpret_cast<weapComp_Round*>(thisComp)->RoundTransferTo(
      weaponComponent::BOLT);
    //reinterpret_cast<weapComp_Round*>(thisComp->parent->weapComps[weaponComponent::BOLT]));

  //TEMP: forcing success
    if (returnVal == false)
    {
      std::cout << "*FORCING Success; result was " << std::boolalpha << returnVal << std::endl;
      returnVal = true; //DESIRED_EFFECT
    }

    return returnVal;
  }
};

//Firing:
//  -*engage hammer (animation + sound)
//  -activateRoundInChamber (activate external system)
//  -if successful: continue
//  -else: don't continue
struct weapStage_firing : public weaponStage
{
  weapStage_firing() : weaponStage(TD_ACTIVE)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    /*
      playAnimation(HammerPos<animation>(hammerPosEngaged));
      playSound(HammerPos<sound>(hammerPosEngaged));
    */

    std::cout << "Firing" << std::endl;

    bool roundActivated = false;

    //if(reinterpret_cast<weapComp_Round*>(thisComp)->round != nullptr)
    {
      roundActivated = reinterpret_cast<weapComp_Round*>(thisComp)->activateRound();
    }

    if (roundActivated)
    {
      //thisComp->parent->weapComps[weaponSystem::BOLT]->readyComponent();
      thisComp->parent->setComponent(TD_READY, weaponComponent::BOLT);
    }

    return roundActivated;
  }
};

////////////////////////////////////////
//==MAGIZINE==
  //cyclingInternally(TD_READY);
  //cyclingExternally(TD_ACTIVE);

//Cycling Internally:
struct weapStage_cyclingInternally : public weaponStage
{
  weapStage_cyclingInternally() : weaponStage(TD_READY)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Cycling Internally" << std::endl;

    reinterpret_cast<weapComp_Mag*>(thisComp)->ReadyRound();

    return true;
  }
};

//Cycling Externally:
struct weapStage_cyclingExternally : public weaponStage
{
  weapStage_cyclingExternally() : weaponStage(TD_ACTIVE)
  {}

  bool activationStage(struct weaponComponent* thisComp)
  {
    std::cout << "Cycling Externally" << std::endl;

    //reinterpret_cast<weapComp_Round*>(thisComp)->debug(); //DEBUG

    bool returnVal =
      reinterpret_cast<weapComp_Round*>(thisComp)
      ->RoundTransferTo(weaponComponent::BOLT);

    //reinterpret_cast<weapComp_Round*>(thisComp)->debug(); //DEBUG

    //TEMP: forcing success
    if (returnVal == false)
    {
      std::cout
        << "*FORCING Success; result was "
        << std::boolalpha
        << returnVal
        << std::endl;

      returnVal = true; //DESIRED_EFFECT
    }

    return returnVal;
  }
};

struct modCompCond_Mag : public modCompCondtional
{
  modCompCond_Mag() : modCompCondtional(weaponComponent::MAGIZINE)
  {}

  weaponComponent* rulesForAdding() override
  {
    std::cout << "Applying rule for adding magizine" << std::endl;
    //reachableMag->Reset();
    //return reachableMag;

    //TODO: Implement functionality to speficiy where the
    //      magizine is pulled from in the game world
    weapComp_Mag* reachableMag
      = new weapComp_Mag(weaponComponent::MAGIZINE, 10);

    reachableMag->Reset();

    //TODO: This logic should be in weapComp_Mag
    reachableMag->weaponStages.push_back(new weapStage_cyclingInternally);
    reachableMag->weaponStages.push_back(new weapStage_cyclingExternally);

    return reachableMag;
  }

  bool rulesForRemoving() override
  {
    std::cout << "Applying rule for removing magizine" << std::endl;
    return true;
  }
};

#endif
