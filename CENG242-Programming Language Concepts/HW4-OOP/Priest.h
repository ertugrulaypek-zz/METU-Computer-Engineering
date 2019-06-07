#ifndef HW4_PRIEST_H
#define HW4_PRIEST_H

#include"Player.h"

class Priest : public Player{
  /**
   * Attack damage 0
   * Heal power 50
   * Max HP 150
   * Goal Priorities -> {HEAL,TO_ALLY,CHEST} in decreasing order
   * Class abbreviation -> "pr" or "PR"
   * Can move to all adjacent squares, including diagonals.
   * Can heal all adjacent squares, including diagonals.
   *
   */
   public:
   Priest(uint, int, int, Team);
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
