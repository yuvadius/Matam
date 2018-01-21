#include <string>
#include "Area.h"
#include "Plain.h"
#include <algorithm>

namespace mtm {

	/**
	 * Constructor
	 * @param name The name of the plain
	 * @throws AreaInvalidArguments If the name is empty
	 */
	Plain::Plain(const std::string& name) : Area(name) {

	}

	/**
	 * Destructor
	 */
	Plain::~Plain() {

	}

	/**
	 * Get the divided group name
	 * the divided name will be ("group_name" + "_" + number)
	 * the number is bigger then 1 the divided name will be a different name
	 * from all the group names in clan_map
	 * @param group_name The name of the group that get into the area
	 * @param clan_map The map of clans that contains the clan of the group
	 */
	const std::string Plain::getGroupDivideName(const string& group_name,
										 	    map<string, Clan>& clan_map) {
		bool group_exists = true;
		int i = 2;
		while(group_exists == true) {
			group_exists = false;
			std::string new_group_name = group_name + "_" + std::to_string(i);
			for(map<string, Clan>::const_iterator it = clan_map.begin();
				it != clan_map.end(); ++it) {
				if(it->second.doesContain(new_group_name)) {
					group_exists = true;
					break;
				}
			}
			if(group_exists == false) {
				break;
			}
			++i;
		}
		return group_name + "_" + std::to_string(i);
	}

	/**
	 * Get a group into the plain area
	 * This function call the base class first and then does checks if the
	 * group should unite or divide
	 * the group should unite with the strongest group in the clan if the
	 * group's size is less then GROUP_UNITE_THRESHOLD * group_clan.getSize()
	 * and if the united group's size is less then or equal to
	 * GROUP_UNITE_THRESHOLD * group_clan.getSize()
	 * the group should divide if the group has at least GROUP_DIVIDE_THRESHOLD
	 * members in the group and if the group's size is more then
	 * GROUP_UNITE_THRESHOLD * group_clan.getSize()
	 * the divided groups name will be set by "getGroupDivideName"
	 * @param group_name The name of the group that get into the area.
	 * @param clan The name of the clan that the group belongs to.
	 * @param clan_map The map of clans that contains the clan of the group
	 */
	void Plain::groupArrive(const string& group_name, const string& clan,
							map<string, Clan>& clan_map) {
		static const double GROUP_UNITE_THRESHOLD = (1.0 / 3.0);
		static const int GROUP_DIVIDE_THRESHOLD = 10;
		Area::groupArrive(group_name, clan, clan_map); //call base function
		Clan group_clan = clan_map.find(clan)->second; //get the clan
		GroupPointer group = group_clan.getGroup(group_name);//get group in clan
		if(group->getSize() < (GROUP_UNITE_THRESHOLD * group_clan.getSize())) {
			std::vector<GroupPointer>::iterator max =
							max_element(std::begin(groups), std::end(groups));
			group->unite(**max, GROUP_UNITE_THRESHOLD * group_clan.getSize());
			groups.erase(max);
		}
		else if(group->getSize() > GROUP_UNITE_THRESHOLD*group_clan.getSize()) {
			if(group->getSize() >= GROUP_DIVIDE_THRESHOLD) {
				std::string group2 = getGroupDivideName(group_name, clan_map);
				GroupPointer new_group(new Group(group->divide(group2)));
				groups.push_back(new_group); //add the divided group to the area
			}
		}
	}
}
