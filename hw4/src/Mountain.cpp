#include <string>
#include "Area.h"
#include "Mountain.h"
#include <algorithm>

namespace mtm {

	/**
	 * Constructor
	 * @param name The name of the plain
	 * @throws AreaInvalidArguments If the name is empty
	 */
	Mountain::Mountain(const std::string& name) : Area(name) {
		controlling_group = ""; //no controlling group
	}

	/**
	 * Destructor
	 */
	Mountain::~Mountain() {

	}

	/**
	 * Get a group into the mountain.
	 * if there is no controlling group(i.e. controlling_group = "") then
	 * the group will become the controlling group
	 * if there is already a controlling group then
	 * if the group and the controlling group are both from the same clan
	 * then the stronger group becomes the controlling group
	 * if the group and the controlling group are from different clans then
	 * the two groups fight and the winner becomes the controlling group
	 * @param group_name The name of the group that get into the mountain.
	 * @param clan The name of the clan that the group belongs to.
	 * @param clan_map The map of clans that contains the clan of the group
	 */
	void Mountain::groupArrive(const string& group_name, const string& clan,
					 	 	   map<string, Clan>& clan_map) {
		Area::groupArrive(group_name, clan, clan_map); //call base function
		Clan group_clan = clan_map.find(clan)->second; //get the clan
		GroupPointer group = group_clan.getGroup(group_name);//get group in clan
		if(controlling_group == "") {
			controlling_group = group_name;
		}
		else if(group_clan.doesContain(controlling_group)) {
			if(*group > *(group_clan.getGroup(controlling_group))) {
				controlling_group = group_name;
			}
		}
		else {
			GroupPointer controll_group;
			for(map<string, Clan>::const_iterator it = clan_map.begin();
				it != clan_map.end(); ++it) {
				if(it->second.doesContain(controlling_group)) {
					controll_group = it->second.getGroup(controlling_group);
					break;
				}
			}
			if(group->fight(*controll_group) == WON) {
				controlling_group = group_name;
			}
		}
	}

	/**
	 * Get a group out of the mountain.
	 * If the group isn't the controlling groups then nothing happens
	 * If the group is the controlling group then the strongest group from
	 * the controlling group's clan becomes the new controlling group
	 * if there is no such group then the strongest group in the mountain
	 * becomes the controlling group
	 * @param group_name The name of the leaving group.
	 */
	void Mountain::groupLeave(const std::string& group_name) {
		Area::groupLeave(group_name);
		if(group_name != controlling_group) {
			GroupPointer strongest_in_clan = getStrongestInClan(group_name);
			if(strongest_in_clan != nullptr) {
				controlling_group = strongest_in_clan->getName();
			}
			else {
				GroupPointer strongest_group = getStrongestGroup(group_name);
				if(strongest_group != nullptr) {
					controlling_group =getStrongestGroup(group_name)->getName();
				}
				else {
					controlling_group = "";
				}
			}
		}
	}
}
