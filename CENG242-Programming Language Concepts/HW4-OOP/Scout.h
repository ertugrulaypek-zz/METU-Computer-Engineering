#ifndef HW4_SCOUT_H
#define HW4_SCOUT_H

#include"Player.h"

class Scout : public Player{
  /**
   * Attack damage 25
   * Heal power 0
   * Max HP 125
   * Goal Priorities -> {CHEST,TO_ALLY,ATTACK} in decreasing order
   * Class abbreviation -> "sc" or "SC"
   * Can move to all adjacent squares, including diagonals.
   * Can attack all adjacent squares, including diagonals.
   *
   */
   public:
   Scout(uint, int, int, Team);
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
