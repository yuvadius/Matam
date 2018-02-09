#include <math.h>
#include <stdlib.h> /* abs */
#include <algorithm>
#include "Group.h"
#include "exceptions.h"

namespace mtm{

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
	Group::Group(const std::string& name, const std::string& clan, int children,
				 int adults, int tools, int food, int morale) : name(name),
				 clan(clan), children(children), adults(adults), tools(tools),
				 food(food), morale(morale) {
		if(name == "" || children < 0 || adults < 0 || tools < 0 || food < 0 ||
		   morale < 0 || morale > 100 || (children == 0 && adults == 0)) {
			throw GroupInvalidArgs();
		}
	}

	/**
	 * Constructor.
	 * - The group will not belong to any clan.
	 * - The group will have TOOLS_PER_ADULT tools for every adult.
	 * - The group will have FOOD_PER_ADULT food for every adult,
	 *      and FOOD_PER_CHILD food for every child
	 * - The group will have DEFAULT_MORALE morale.
	 * @param name The group's name. Can't be empty.
	 * @param children The amount of children in the group. Must be
	 *  non-negative.
	 * @param adults Bigger number means more tools. Must be non-negative.
	 * @throws GroupInvalidArgs If at least one of the arguments is
	 *  invalid, or both adults and children are 0.
	 */
	Group::Group(const std::string& name, int children, int adults) :
				 name(name), clan(""), children(children), adults(adults) {
		if(name == "" || children < 0 || adults < 0 ||
		  (children == 0 && adults == 0)) {
			throw GroupInvalidArgs();
		}
		this->tools = adults * TOOLS_PER_ADULT;
		this->food = (adults * FOOD_PER_ADULT) + (children * FOOD_PER_CHILD);
		this->morale = DEFAULT_MORALE;
	}

	/**
	 * Copy constructor
	 * @param other The group to copy
	 */
	Group::Group(const Group& other) : name(other.name), clan(other.clan),
				 children(other.children), adults(other.adults),
				 tools(other.tools), food(other.food), morale(other.morale) {
	}

	/** Destructor
	 */
	Group::~Group() {
	}

	/**
	 * Get the name of the group
	 * @return The name of the group
	 */
	const std::string& Group::getName() const {
		return name;
	}

	/**
	 * Get the amount of people (children + adults) in the group.
	 * @return  Amount of people in the group
	 */
	int Group::getSize() const {
		return (children + adults);
	}

	/**
	 * @return The name of the clan that the group belongs to.
	 */
	const std::string& Group::getClan() const {
		return clan;
	}

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
	void Group::changeMorale(int change, bool increase, bool shouldFloor) {
		if(change < 0) { //if the change isn't valid
			return;
		}
		double morale_change = (change / 100.0) * morale;
		if(shouldFloor == true) {
			morale_change = floor(morale_change);
		}
		else {
			morale_change = ceil(morale_change);
		}
		if(increase == true) {
			morale += (int)morale_change;
		}
		else {
			morale -= (int)morale_change;
		}
		if(morale > MAX_MORALE) {
			morale = MAX_MORALE;
		}
		else if(morale < MIN_MORALE) {
			morale = MIN_MORALE;
		}
	}

	/**
	 * Change the clan of the group.
	 * If the group had a different clan before, reduce morale by MORALE_CHANGE%
	 * If the group's clan was the same as the given one, do nothing.
	 * Otherwise, increase morale by MORALE_CHANGE% (floor in both cases)
	 *
	 * floor: 0.5 -> 0, 1.5 -> 1 etc...
	 * @param clan The name of the new clan that the groups will belong to.
	 */
	void Group::changeClan(const std::string& clan) {
		if(this->clan == clan) { //if the group isn't changing clans
			return;
		}
		this->clan = clan;
		if(this->clan != "") { //if the group is leaving its clan
			changeMorale(MORALE_CHANGE, false, true);
		}
		else {
			changeMorale(MORALE_CHANGE, true, true);
		}
	}

	/**
	 * Get the power of the group
	 * the power of the group is defined as:
	 * (POWER_PER_ADULT*adults + POWER_PER_CHILD*children) *
	 * (10*POWER_PER_TOOL + POWER_PER_FOOD*food) * (morale/100)
	 */
	double Group::getPower() const {
		return (POWER_PER_ADULT * adults + POWER_PER_CHILD * children) *
				(POWER_PER_TOOL * tools + POWER_PER_FOOD * food) *
				(morale / 100.0);
	}

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
	bool Group::operator<(const Group& rhs) const {
		if(getPower() < rhs.getPower()) {
			return true;
		}
		else if(getPower() == rhs.getPower() && name < rhs.name) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * Compare two groups. according to power, and name.
	 * @param rhs
	 * @return true, if power(this) > power(rhs), or if they're equal and
	 *  this group's name is bigger (lexicographic) than rhs's name.
	 *  false otherwise.
	 */
	bool Group::operator>(const Group& rhs) const {
		if(getPower() > rhs.getPower()) {
			return true;
		}
		else if(getPower() == rhs.getPower() && name > rhs.name) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * Compare two groups. according to power, and name.
	 * @param rhs
	 * @return true, if power(this) < power(rhs), or if they're equal and
	 *  this group's name is not bigger (lexicographic) than rhs's name.
	 *  false otherwise.
	 */
	bool Group::operator<=(const Group& rhs) const {
		if(getPower() < rhs.getPower()) {
			return true;
		}
		else if(getPower() == rhs.getPower() && name <= rhs.name) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * Compare two groups. according to power, and name.
	 * @param rhs
	 * @return true, if power(this) > power(rhs), or if they're equal and
	 *  this group's name is not smaller (lexicographic) than rhs's name.
	 *  false otherwise.
	 */
	bool Group::operator>=(const Group& rhs) const {
		if(getPower() > rhs.getPower()) {
			return true;
		}
		else if(getPower() == rhs.getPower() && name >= rhs.name) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * Compare two groups. according to power, and name.
	 * @param rhs
	 * @return true, if power(this) == power(rhs), and this group's name is
	 * equal to rhs's name.
	 * false otherwise.
	 */
	bool Group::operator==(const Group& rhs) const {
		if(*this > rhs || rhs > *this) {
			return false;
		}
		else {
			return true;
		}
}

	/**
	 * Compare two groups. according to power, and name.
	 * @param rhs
	 * @return true, if power(this) != power(rhs), and this group's name
	 * isn't equal to rhs's name.
	 * false otherwise.
	 */
	bool Group::operator!=(const Group& rhs) const {
		if(*this > rhs || rhs > *this) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * Empty the current group
	 * the name and clan will be set to ""
	 * and all other fields will be set to 0
	 */
	void Group::empty() {
		name = "";
		clan = "";
		children = 0;
		adults = 0;
		tools = 0;
		food = 0;
		morale = 0;
	}

	/**
	 * Try to unite two groups, this group, and given group.
	 * Will succeed only if the two groups are not the same group,
	 * belong to the same clan, both have at least 70(MIN_UNION_MORALE) morale,
	 * and they don't surpass together a given amount of people.
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
	bool Group::unite(Group& other, int max_amount) {
		if(*this == other) { //if both groups are the same
			return false;
		}
		else if(clan != other.clan) {
			return false;
		}
		else if(morale < MIN_UNION_MORALE || other.morale < MIN_UNION_MORALE) {
			return false;
		}
		else if((getSize() + other.getSize()) > max_amount) {
			return false;
		}
		else if(clan == "" || other.clan == "") {
			return false;
		}
		else {
			if(other.getPower() > getPower()) {
				name = other.name;
			}
			morale = floor((getSize() * morale + other.getSize() * other.morale)
							/ ((double)(getSize() + other.getSize())));
			children += other.children;
			adults += other.adults;
			tools += other.tools;
			food += other.food;
			other.empty();
			return true;
		}
	}

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
	Group Group::divide(const std::string& name) {
		if(name == "") {
			throw GroupInvalidArgs();
		}
		if(children <= 1 && adults <= 1) { //if the group can't be divided
			throw GroupCantDivide();
		}
		Group new_group(name, clan, floor(children / 2.0), floor(adults / 2.0),
						floor(tools / 2.0), floor(food / 2.0), morale);
		children -= new_group.children;
		adults -= new_group.adults;
		tools -= new_group.tools;
		food -= new_group.food;
		return new_group;
	}

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
	void Group::win(const Group& opponent) {
		adults -= floor(adults * (ADULTS_LOST_AFTER_WIN / 100.0));
		tools -= floor(tools * (TOOLS_LOST_AFTER_WIN / 100.0));
		int food_of_loser_after_loss = ceil(opponent.food *
											(FOOD_LOST_AFTER_LOSS / 100.0));
		food += floor(food_of_loser_after_loss * (FOOD_WON_AFTER_WIN / 100.0));
		changeMorale(MORALE_GAIN_AFTER_WIN, true, false);
	}

	/**
	 * Effects the groups fields after the group has lost a battle
	 * Fights effects on the loser group:
	 * - Losing third of each of the children and adult population (ceiling)
	 * - Losing Half of the tools, and half of the food (ceiling)
	 * - Morale decreases by 20%. (ceiling)
	 * For example: if a group that had 5 adults and 3 tools lost and 50
	 * morale, it will have 3 adults, 1 tool, and 40 morale after the fight.
	 */
	void Group::lose() {
		//the change in percent in the children in a group after the group lost
		static const double CHILDREN_LOST_AFTER_LOSS = 100.0 * (1.0 / 3.0);
		//the change in percent in the adults in a group after the group lost
		static const double ADULTS_LOST_AFTER_LOSS = 100.0 * (1.0 / 3.0);

		children -= ceil(children * (CHILDREN_LOST_AFTER_LOSS / 100.0));
		adults -= ceil(adults * (ADULTS_LOST_AFTER_LOSS / 100.0));
		tools -= ceil(tools * (TOOLS_LOST_AFTER_LOSS / 100.0));
		food -= ceil(food * (FOOD_LOST_AFTER_LOSS / 100.0));
		changeMorale(MORALE_LOST_AFTER_LOSS, false, false);
	}

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
	FIGHT_RESULT Group::fight(Group& opponent) {
		if(*this == opponent) {
			throw GroupCantFightWithItself();
		}
		if(getSize() == 0 || opponent.getSize() == 0) {
			throw GroupCantFightEmptyGroup();
		}
		if(getPower() == opponent.getPower()) {
			return DRAW;
		}
		else if(getPower() > opponent.getPower()) {
			win(opponent);
			opponent.lose();
			return WON;
		}
		else {
			opponent.win(*this);
			lose();
			return LOST;
		}
	}

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
	void Group::exchange(Group& other, int trade) {
		if(tools > food) {
			tools -= trade;
			food += trade;
		}
		else {
			tools += trade;
			food -= trade;
		}
		if(other.tools > other.food) {
			other.tools -= trade;
			other.food += trade;
		}
		else {
			other.tools += trade;
			other.food -= trade;
		}
	}

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
	bool Group::trade(Group& other) {
		if(*this == other) {
			throw GroupCantTradeWithItself();
		}
		if(food == tools || other.food == other.tools) {
			return false;
		}
		else if(food > tools && other.food > other.tools) {
			return false;
		}
		else if(food < tools && other.food < other.tools) {
			return false;
		}
		else {
			int group_offer = ceil(abs(tools - food) / 2.0);
			int other_group_offer = ceil(abs(other.tools - other.food) / 2.0);
			int trade = ceil((group_offer + other_group_offer) / 2.0);
			//if group is trading more than what it has
			if(trade > std::max(tools, food)) {
				trade = std::max(tools, food);
			}
			//if group is trading more than what it has
			if(trade > std::max(other.tools, other.food)) {
				trade = std::max(other.tools, other.food);
			}
			exchange(other, trade);
			return true;
		}
	}

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
	std::ostream& operator<<(std::ostream& os, const Group& group) {
		if(group.name != "") {
			os << "Group's name: " << group.name << std::endl;
			os << "Group's clan: " << group.clan << std::endl;
			os << "Group's children: " << group.children << std::endl;
			os << "Group's adults: " << group.adults << std::endl;
			os << "Group's tools: " << group.tools << std::endl;
			os << "Group's food: " << group.food << std::endl;
			os << "Group's morale: " << group.morale << std::endl;
		}
		else {
			os << "Group's name: " << std::endl;
			os << "Group's clan: " << std::endl;
			os << "Group's children: 0" << std::endl;
			os << "Group's adults: 0" << std::endl;
			os << "Group's tools: 0" << std::endl;
			os << "Group's food: 0" << std::endl;
			os << "Group's morale: 0" << std::endl;
		}
		return os;
	}
}
