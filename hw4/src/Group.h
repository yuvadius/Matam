#ifndef MTM4_GROUP_HPP
#define MTM4_GROUP_HPP

#include <string>
#include <ostream>

namespace mtm{
    
    enum FIGHT_RESULT{
        WON, LOST, DRAW
    };
    
    /**
    * A Family group of hunter-gatherers.
    */
    class Group{

    private:
    	std::string name; //the name of the group
    	std::string clan; //the name of the clan that the group belongs to
    	int children; //the number of children in the group
    	int adults; //the number of adults in the group
    	int tools; //the number of tools in the group
    	int food; //the amount of food in the group
    	int morale; //the morale of group(whole number 0-100)

    	//the default number of tools each adult brings
    	static const int TOOLS_PER_ADULT = 4;
    	//the default amount of food each child brings
		static const int FOOD_PER_CHILD = 2;
    	//the default amount of food each adult brings
    	static const int FOOD_PER_ADULT = 3;
    	//the default morale of a group
    	static const int DEFAULT_MORALE = 70;
    	//the minimal morale that a group can have
    	static const int MIN_MORALE = 0;
    	//the maximum morale that a group can have
    	static const int MAX_MORALE = 100;
    	//the change in percent in the morale when a group changes clan
    	static const int MORALE_CHANGE = 10;
    	//the power that one adult gives to the group
    	static const int POWER_PER_ADULT = 10;
    	//the power that one child gives to the group
    	static const int POWER_PER_CHILD = 3;
    	//the power that one tool gives to the group
    	static const int POWER_PER_TOOL = 10;
    	//the power that one food gives to the group
    	static const int POWER_PER_FOOD = 1;
    	//the minimal morale that a group must have to perform a union
    	static const int MIN_UNION_MORALE = 70;
    	//the change in percent in the adults in a group after the group won
    	static const int ADULTS_LOST_AFTER_WIN = 25;
    	//the change in percent in the tools in a group after the group won
		static const int TOOLS_LOST_AFTER_WIN = 25;
		//the amount of food gained(in percent) from the loser group
		static const int FOOD_WON_AFTER_WIN = 50;
		//the morale increase(in percent) of a group after the group won
		static const int MORALE_GAIN_AFTER_WIN = 20;
		//the change in percent in the tools in a group after the group lost
		static const int TOOLS_LOST_AFTER_LOSS = 50;
		//the change in percent in the food in a group after the group lost
		static const int FOOD_LOST_AFTER_LOSS = 50;
		//the morale decrease(in percent) of a group after the group lost
		static const int MORALE_LOST_AFTER_LOSS = 20;

    	/**
		 * Increase the morale of the group by change percent(will floor the
		 * morale afterwards)
		 * @param change The change in percent to the group's morale(if change
		 * isn't a number in the range 0-100 then no change will occur)
		 * @param increase if true then the morale will rise and if false then
		 * the moral will fall
		 * @param shouldFloor if true then the morale will be floored, if false then
		 * ceiling will be used on the morale
		 */
    	void changeMorale(int change, bool increase, bool shouldFloor);

    	/**
		 * Get the power of the group
		 * the power of the group is defined as:
		 * (POWER_PER_ADULT*adults + POWER_PER_CHILD*children) *
		 * (10*POWER_PER_TOOL + POWER_PER_FOOD*food) * (morale/100)
		 */
		double getPower() const;

		/**
		 * Empty the current group
		 * the name and clan will be set to ""
		 * and all other fields will be set to 0
		 */
		void empty();

		/**
		 * Effects the groups fields after the group has won a battle
		 * Fights effects on the winner group:
		 * - Losing a quarter of the adults (floor) and no children.
		 * - Losing a quarter of the tools (floor).
		 * - gaining half of the food the loser group lost in the fight. (floor)
		 *      The gain is equal to ceiling(loser.food / 2) / 2, even if the
		 *      loser group lost all of it's people.
		 * - Morale increases by 20%. (ceiling)
		 * @param opponent The group that lost
		 */
		void win(const Group& opponent);

		/**
		 * Effects the groups fields after the group has lost a battle
		 * Fights effects on the loser group:
		 * - Losing third of each of the children and adult population (ceiling)
		 * - Losing Half of the tools, and half of the food (ceiling)
		 * - Morale decreases by 20%. (ceiling)
		 * For example: if a group that had 5 adults and 3 tools lost and 50
		 * morale, it will have 3 adults, 1 tool, and 40 morale after the fight.
		 */
		void lose();

		/**
		 * Makes a trade between both groups("this" group and "other" group)
		 * Each group will gain "trade" number of resources of the
		 * resource it has less(food or tools)
		 * Each group will lose "trade" number of resources of the
		 * resource it has more(food or tools)
		 * NOTE: this function assumes that for each group the amount of food does
		 * not equal the amount of tools, and this function assumes that trade is
		 * valid
		 * @param other Group to trade with
		 * @param trade The number of resources that will be traded
		 */
		void exchange(Group& other, int trade);

    public:
        /**
         * Constructor
         * @param name The group's name. Can't be empty.
         * @param clan The clan which the group belongs to.
         *  empty string means no clan.
         * @param children The amount of children in the group. Must be
         *  non-negative.
         * @param adults Bigger number means more tools. Must be non-negative.
         * @param tools Bigger number means more tools. Must be non-negative.
         * @param food Bigger number means more food. Must be non-negative.
         * @param morale Bigger number means higher morale.
         *  Must be between 0 to 100.
         * @throws GroupInvalidArgs If at least one of the arguments is
         *  invalid, or both adults and children are 0.
         */
        Group(const std::string& name, const std::string& clan, int children,
              int adults, int tools, int food, int morale);
        
        /**
         * Constructor.
         * - The group will not belong to any clan.
         * - The group will have 4 tools for every adult.
         * - The group will have 3 food for every adult,
         *      and 2 food for every child
         * - The group will have 70 morale.
         * @param name The group's name. Can't be empty.
         * @param children The amount of children in the group. Must be
         *  non-negative.
         * @param adults Bigger number means more tools. Must be non-negative.
         * @throws GroupInvalidArgs If at least one of the arguments is
         *  invalid, or both adults and children are 0.
         */
        Group(const std::string& name, int children, int adults);
        
        /**
         * Copy constructor
         * @param other The group to copy
         */
        Group(const Group& other);
        
        /** Destructor
         */
        ~Group();
        
        /**
         * Get the name of the group
         * @return The name of the group
         */
        const std::string& getName() const;
        
        /**
         * Get the amount of people (children + adults) in the group.
         * @return  Amount of people in the group
         */
        int getSize() const;
        
        /**
         * @return The name of the clan that the group belongs to.
         */
        const std::string& getClan() const;
        
        /**
         * Change the clan of the group.
         * If the group had a different clan before, reduce morale by 10%.
         * If the group's clan was the same as the given one, do nothing.
         * Otherwise, increase morale by 10%. (floor in both cases)
         *
         * floor: 0.5 -> 0, 1.5 -> 1 etc...
         * @param clan The name of the new clan that the groups will belong to.
         */
        void changeClan(const std::string& clan);
        
        /**
         * Compare two groups. according to power (defined here), and name.
         *
         * Power: ((10*number_of_adults + 3 * number_of_children) *
         * (10 * number_of_tools + number_of_food)*morale)/100 (floor)
         *
         * @param rhs
         * @return true, if power(this) < power(rhs), or if they're equal and
         *  this group's name is smaller (lexicographic) than rhs's name.
         *  false otherwise.
         */
        bool operator<(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) > power(rhs), or if they're equal and
         *  this group's name is bigger (lexicographic) than rhs's name.
         *  false otherwise.
         */
        bool operator>(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) < power(rhs), or if they're equal and
         *  this group's name is not bigger (lexicographic) than rhs's name.
         *  false otherwise.
         */
        bool operator<=(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) > power(rhs), or if they're equal and
         *  this group's name is not smaller (lexicographic) than rhs's name.
         *  false otherwise.
         */
        bool operator>=(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) == power(rhs), and this group's name is
         * equal to rhs's name.
         * false otherwise.
         */
        bool operator==(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) != power(rhs), and this group's name
         * isn't equal to rhs's name.
         * false otherwise.
         */
        bool operator!=(const Group& rhs) const;
        
        /**
         * Try to unite two groups, this group, and given group.
         * Will succeed only if the two groups are not the same group,
         * belong to the same clan, both have at least 70 morale, and they
         * don't surpass together a given amount of people.
         * Should the union succeed, this group will change according to:
         * - name = name of the more powerful group
         * - clan = old clan
         * - children = sum of the children in the two groups
         * - adults = sum of the adults in the two groups
         * - tools = sum of the tools in the two groups
         * - food = sum of the food in the two groups
         * - morale = average of the morales, counting the amount of people
         * (floor)
         *
         * The other group will become empty.
         * @param other The group to unite with.
         * @param max_amount If the two groups have more people combined then
         *  max_amount, then the union fails.
         * @return True if Unite was successful, false otherwise.
         */
        bool unite(Group& other, int max_amount);
        
        /**
         * Divide the group.
         *
         * After the division this group will have the same
         *  name, clan, morale. And ceiling of half of children , adults ,
         *  tools, food
         *
         * The other group will have a new name, same clan, same morale,
         *  and floor of half of children , adults, tools, food.
         *
         * Ceiling: 0.5 -> 1, 1.5 -> 2 etc...
         *
         * floor: 0.5 -> 0, 1.5 -> 1 etc...
         * @param name The name of the new group.
         * @return The new group, that divided from this group.
         * @throws GroupInvalidArgs If name is empty.
         * @throws GroupCantDivide If the divided group Would be empty (can
         * only happen of this group has no more than 1 child and 1 adult).
         */
        Group divide(const std::string& name);
        
        /**
         * This group fights an opponent group. The bigger group according to
         *  the relational operators is the winner. If both groups are equal,
         *  the groups will not fight.
         *
         * Fights effects on the loser group:
         * - Losing third of each of the children and adult population (ceiling)
         * - Losing Half of the tools, and half of the food (ceiling)
         * - Morale decreases by 20%. (ceiling)
         * For example: if a group that had 5 adults and 3 tools lost and 50
         * morale, it will have 3 adults, 1 tool, and 40 morale after the fight.
         *
         * Fights effects on the winner group:
         * - Losing a quarter of the adults (floor) and no children.
         * - Losing a quarter of the tools (floor).
         * - gaining half of the food the loser group lost in the fight. (floor)
         *      The gain is equal to ceiling(loser.food / 2) / 2, even if the
         *      loser group lost all of it's people.
         * - Morale increases by 20%. (ceiling)
         * @param opponent The group to fight with.
         * @return WON if this group won, LOST if this group lost, and DRAW
         * in case the 2 groups are equal.
         * @throws GroupCantFightWithItself if opponent is the same as this
         * @throws GroupCantFightEmptyGroup if this group, or the other group
         *  is empty (i.e no people).
         */
        FIGHT_RESULT fight(Group& opponent);
        
        /**
         * Try to do a trade between the two groups.
         * Each groups tries make its tools and food equal.
         * To do that, each group offers |group's tools - group's food| / 2
         * (ceiling) of the resource it has more of.
         * If the other group offers the other resource, a trade is made.
         * In the trade, each group gives the average of the offers [(offer1 +
         * offer2)/2 (ceiling)], from the resource it has more of. A group
         * can't give more tools or food than it has, and the two groups
         * always trade the same amount of resources.
         *
         * If one of the groups has the same amount of tools as amount of
         * food, or if both has more tools than food, or vice versa,
         * the trade would not be made.
         * @param other Group to trade with.
         * @return true if a trade was made, false otherwise.
         * @throws GroupCantTradeWithItself if other is the same as this
         */
        bool trade(Group& other);
        
        /**
         * Print the data of a given group. Output form:
         *      Group's name: [group's name]
         *      Group's clan: [group's clan]
         *      Group's children: [group's children]
         *      Group's adults: [group's adults]
         *      Group's tools: [group's tools]
         *      Group's food: [group's food]
         *      Group's morale: [group's morale]
         *
         * @param os the output stream
         * @param group The group to print
         * @return the output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const Group& group);
    };
} // namespace mtm

#endif //MTM4_GROUP_HPP
