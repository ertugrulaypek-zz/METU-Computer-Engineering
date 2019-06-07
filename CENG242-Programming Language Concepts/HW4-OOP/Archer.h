#ifndef HW4_ARCHER_H
#define HW4_ARCHER_H

#include"Player.h"

class Archer : public Player{
  /**
   * Attack damage 50
   * Heal power 0
   * Max HP 200
   * Goal Priorities -> {ATTACK}
   * Class abbreviation -> "ar" or "AR"
   * Not able to move at all.
   * Can attack to a range of 2 squares directly up, down, left or right, from
   * its coordinate.
   *
   */
   public:
   Archer(uint, int, int, Team);
   virtual int getAttackDamage() const;
   virtual int getHealPower() const;
   virtual int getMaxHP() const;
   virtual std::vector<Goal> getGoalPriorityList();
   virtual const std::string getClassAbbreviation() const;
   virtual std::vector<Coordinate> getAttackableCoordinates();
   virtual std::vector<Coordinate> getMoveableCoordinates();
   virtual std::vector<Coordinate> getHealableCoordinates();
};

#endif
