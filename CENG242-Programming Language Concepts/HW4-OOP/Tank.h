#ifndef HW4_TANK_H
#define HW4_TANK_H

#include"Player.h"

class Tank : public Player{
  /**
   * Attack damage 25
   * Heal power 0
   * Max HP 1000
   * Goal Priorities -> {TO_ENEMY,ATTACK,CHEST} in decreasing order
   * Class abbreviation -> "ta" or "TA"
   * Can move to adjacent up, down, left or right square
   * Can attack to adjacent up, down, left or right square
   *
   */

   public:
   Tank(uint, int, int, Team);
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
