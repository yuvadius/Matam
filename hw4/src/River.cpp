#include <string>
#include "Area.h"
#include "River.h"
#include <algorithm>

namespace mtm {

	/**
	 * Constructor
	 * @param name The name of the river
	 * @throws AreaInvalidArguments If the name is empty
	 */
	River::River(const std::string& name) : Area(name) {

	}

	/**
	 * Destructor
	 */
	River::~River() {

	}

	/**
	 * Get a group into the river area
	 * This function calls the base class first and then does checks if the
	 * group should trade
	 * the group trades with the strongest group that is from the groups
	 * clan or from the clans friends
	 * @param group_name The name of the group that get into the river.
	 * @param clan The name of the clan that the group belongs to.
	 * @param clan_map The map of clans that contains the clan of the group
	 */
	void River::groupArrive(const string& group_name, const string& clan,
							map<string, Clan>& clan_map) {
		Area::groupArrive(group_name, clan, clan_map); //call base function
		Clan group_clan = clan_map.find(clan)->second; //get the clan of group
		GroupPointer group = group_clan.getGroup(group_name);//get group in clan
		std::vector<string> filters; //the group names that wont be traded with
		filters.push_back(group_name); //group can't trade with itself
		GroupPointer strongest = getStrongestGroupFriend(filters, group_clan);
		//loop while there is a group to trade with and while a trade failed
		while(strongest != nullptr && group->trade(*strongest) == false) {
			filters.push_back(strongest->getName()); //strongest failed trade
			strongest = getStrongestGroupFriend(filters, group_clan);
		}
	}
}
