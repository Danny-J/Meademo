#include <iostream>
#include "weapon_system.h"
#include "weapon_stage_instances.h"

int main()
{
  //////////////////////////////
  //Weapon Components

  //TODO: Need to take all data relating to Weapon System and Serialize them

    //Pre-fire
  weaponComponent *trigger
    = new weaponComponent(weaponComponent::TRIGGER);
  weaponComponent *hammer
    = new weaponComponent(weaponComponent::HAMMER);
  weapComp_Action *action
    = new weapComp_Action(weaponComponent::ACTION);

  //Firing
  weapComp_Port   *feedPort
    = new weapComp_Port(weaponComponent::FEEDPORT, new modCompCond_Mag);
  weapComp_Round  *bolt
    = new weapComp_Round(weaponComponent::BOLT);
  weapComp_Round  *chamber
    = new weapComp_Round(weaponComponent::CHAMBER);
  //weaponComponent *ejectPort  = new weaponComponent("EjectPort", weaponComponent::CHAMBER); //UNUSED

  //Post-fire
  weaponComponent *barrel
    = new weaponComponent(weaponComponent::BARREL);
  weaponComponent *muzzle
    = new weaponComponent(weaponComponent::MUZZLE);

  //////////////////////////////
  //Initializing Weapon Comps

  std::cout << "\n**Initializing Weapons: START**" << std::endl;

#if 0

  //TODO: REVISE
  {
    std::cout << "Weapon: Opened-bolt Rifle" << std::endl;

    //Opened-bolt Rifle
    trigger->CompToActivate(action, TD_READY);
    action->CompToActivate(bolt, TD_READY);
    //***No hammer
    bolt->CompToActivate(chamber, TD_EITHER);

    chamber->CompToReady(bolt, TD_ACTIVE);
    bolt->CompToReady(action, TD_EITHER);
  }

#elif 1

  {
    std::cout << "Weapon: Closed-bolt Rifle" << std::endl;

    //Closed-bolt Rifle
    trigger->setCompToActivate(weaponComponent::ACTION, weaponStage::TD_READY);
    action->setCompToActivate(weaponComponent::HAMMER,  weaponStage::TD_READY);
    hammer->setCompToActivate(weaponComponent::CHAMBER, weaponStage::TD_EITHER);
    //hammer->setCompToActivate(weaponComponent::BOLT, TD_ACTIVE);
    //bolt->setCompToActivate(weaponComponent::CHAMBER, TD_EITHER);

    //chamber->setCompToReady(weaponComponent::BOLT, TD_ACTIVE);
    bolt->setCompToReady(weaponComponent::HAMMER, weaponStage::TD_EITHER);
    hammer->setCompToReady(weaponComponent::ACTION, weaponStage::TD_EITHER);
    //action->setCompToReady(weaponComponent::TRIGGER, weaponStage::TD_EITHER);
  }

#else

  //TODO: REVISE
  {
    std::cout << "Weapon: Semi-auto Pistol" << std::endl;

    //Semi-auto Pistol; bolt is slide
    trigger->CompToActivate(hammer, TD_EITHER);
    hammer->CompToActivate(bolt, TD_ACTIVE);

    chamber->CompToReady(bolt, TD_ACTIVE);
    bolt->CompToReady(hammer, TD_EITHER);
  }

#endif

  std::cout << "***Initializing Weapons: END***" << std::endl;

  //////////////////////////////
  //Assign weaponStage to weaponComponent

      //Trigger
  trigger->weaponStages.push_back(new weapStage_pulling);
  trigger->weaponStages.push_back(new weapStage_releasing);

  //Hammer
  hammer->weaponStages.push_back(new weapStage_cocking);
  hammer->weaponStages.push_back(new weapStage_engaging);

  //Action
  action->weaponStages.push_back(new weapStage_reacting);
  action->weaponStages.push_back(new weapStage_actioning);

  //Feedport
  feedPort->weaponStages.push_back(new weapStage_unloading);
  feedPort->weaponStages.push_back(new weapStage_loading);

  //Bolt
  bolt->weaponStages.push_back(new weapStage_opening);
  bolt->weaponStages.push_back(new weapStage_feeding);
  bolt->weaponStages.push_back(new weapStage_ejecting);
  bolt->weaponStages.push_back(new weapStage_chambing); //was in Chamber
  bolt->weaponStages.push_back(new weapStage_extracting);
  bolt->weaponStages.push_back(new weapStage_closing);

  //Chamber
  chamber->weaponStages.push_back(new weapStage_emptying);
  chamber->weaponStages.push_back(new weapStage_firing); //was in Round

  //MOVED: To internal logic of weapComp_Mag
    //Magizine
  //detactableMag->weaponStages.push_back(cyclingInternally);
  //detactableMag->weaponStages.push_back(cyclingExternally);

  //TODO: Round

//////////////////////////////
//Weapon Systems

  weaponSystem rifleAuto("Rifle Auto");

  //////////////////////////////
  //Assign weaponComponent to weaponSystem

      //NOTE: Needs to be in order of enum WEAP_COMPS_ENUMS
  rifleAuto.addWeapComp(trigger);  //TRIGGER,
  rifleAuto.addWeapComp(hammer);   //HAMMER,
  rifleAuto.addWeapComp(action);   //ACTION,
  rifleAuto.addWeapComp(feedPort); //FEEDPORT,
  rifleAuto.addWeapComp(bolt);     //BOLT,
  rifleAuto.addWeapComp(chamber);  //CHAMBER,
  rifleAuto.addWeapComp(barrel);   //BARREL,
  rifleAuto.addWeapComp(muzzle);   //MUZZLE,
  //rifleAuto.addWeapComp(nullptr);  //MAGIZINE,

  weaponSystem &curWeapon = rifleAuto;

  std::string input;

  while (input != "q")
  {
    input.clear();
    std::cout << "\nENTER COMMAND: ";
    std::cin >> input;

    if (input == "t") //Check weapon ammo count
    {
      std::cout << "STATUS[Ammo check]" << std::endl;

      unsigned roundCount = curWeapon.weaponRoundCount();

      std::cout << "*Rounds in system in all: " << roundCount << std::endl;
    }
    else if (input == "r") //Reload
    {
      std::cout << "ACTION[Reloading]" << std::endl;

      bool shouldReload = true;

      //If weapon already has magizine
      if (curWeapon.getWeapComp(weaponComponent::MAGIZINE))
      {
        if (reinterpret_cast<weapComp_Mag*>(
          curWeapon.getWeapComp(weaponComponent::MAGIZINE))->IsFull())
        {
          shouldReload = false;
        }
        else
        {
          std::cout << "*Releasing Magizine" << std::endl;
          curWeapon.setComponent(weaponStage::TD_READY, weaponComponent::FEEDPORT);
        }
      }

      if (shouldReload == true)
      {
        //Setting Bolt Open
        if (curWeapon.IsReady(weaponComponent::BOLT) == false)
        {
          curWeapon.setComponent(weaponStage::TD_READY, weaponComponent::BOLT);
        }

        std::cout << "*Beginning process to inserting Magizine" << std::endl;
        //reachableMag->Reset();
        //detactableMag->Reset();
        curWeapon.setComponent(weaponStage::TD_ACTIVE, weaponComponent::FEEDPORT);
        curWeapon.setComponent(weaponStage::TD_ACTIVE, weaponComponent::BOLT);
      }
      else
      {
        std::cout << "*Reloading Cancelled" << std::endl;
      }
    }
    else if (input == "f") //Fire Weapon; per trigger pull
    {
      std::cout << "ACTION[Firing]; Number of trigger pulls: ";

      unsigned triggerPulls = 1;

      std::cin >> triggerPulls;

      std::cout << triggerPulls << " PULL(s)" << std::endl;

      for (unsigned i = 0; i < triggerPulls; ++i)
      {
        curWeapon.setComponent(weaponStage::TD_ACTIVE, weaponComponent::TRIGGER);
        curWeapon.setComponent(weaponStage::TD_READY, weaponComponent::TRIGGER);
      }
    }
    else if (input == "c") //Fire Weapon; per rounds fired
    {
      std::cout << "ACTION[Firing]; Number of rounds to fire: ";

      unsigned roundsToFire = 1;

      std::cin >> roundsToFire;

      std::cout << roundsToFire << " ROUND(s) TO FIRE" << std::endl;

      unsigned i = 0;

      while (i < roundsToFire)
      {
        bool shouldCont = curWeapon.setComponent(weaponStage::TD_ACTIVE, weaponComponent::TRIGGER);
        curWeapon.setComponent(weaponStage::TD_READY, weaponComponent::TRIGGER);

        if (shouldCont == false)
        {
          break;
        }

        i += curWeapon.roundCyclesFired;

        //std::cout << "\ni: " << i << "\n" << std::endl; //DEBUG
      }
    }
    else if (input == "e") //FireModes
    {
      std::cout << "ACTION[Cycle FireModes]" << std::endl;

      reinterpret_cast<weapComp_Action*>(
        curWeapon.getWeapComp(weaponComponent::ACTION))
        ->firemodes.cycleFiremodes();
    }
    else if (input == "b") //Open Bolt
    {
      std::cout << "ACTION[Open Bolt]" << std::endl;

      curWeapon.setComponent(weaponStage::TD_READY, weaponComponent::BOLT);
    }
    else if (input == "n") //Close Bolt
    {
      std::cout << "ACTION[Close Bolt]" << std::endl;

      curWeapon.setComponent(weaponStage::TD_ACTIVE, weaponComponent::BOLT);
    }
    else if (input == "d") //debug of firearm
    {
      std::cout
        << "STATUS[Debug of WeaponSystem: "
        << std::right << std::setw(10)
        << curWeapon.name << "]"
        << std::endl;
      curWeapon.debug();
    }
    else if (input == "s") //status of firearm
    {
      std::cout
        << "STATUS[Weapon Component info of WeaponSystem: "
        << std::right << std::setw(10)
        << curWeapon.name << "]"
        << std::endl;

      for (
        weaponComponent::WEAP_COMPS_ENUMS i
        = static_cast<weaponComponent::WEAP_COMPS_ENUMS>(0);
        i < weaponComponent::COUNT;
        i = weaponComponent::WEAP_COMPS_ENUMS(static_cast<int>(i) + 1)
        )
      {
        if (curWeapon.getWeapComp(i) != nullptr)
        {
          curWeapon.getWeapComp(i)->weaponComponent::debug();
        }
        else
        {
          //TODO: Replace 'i' with a enumToString translation
          std::cout << "*This WeapSys is missing WeapComp type of: " << i << std::endl;
        }
      }
    }
    else
    {
      std::cout << "UNKNOWN COMMAND" << std::endl;
    }
  }

  std::cout << "QUIT" << std::endl;

  return 0;
}
