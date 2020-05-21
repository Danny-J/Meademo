//I am the middle interface between the outside world (external requests) to the weapon, and the weapon to the outside world.
//Think of me as a container that holds all the existing weapon components on this weapon
//I do not know how any weapon components interact with each other, I just understand that each weapon component, when activated/readied,
  //has info on what other weapon components should in turn be activated/readied themselves.
  //This in turn activates/readies other components whom have their own info of weapon components to activate/ready, and so on.