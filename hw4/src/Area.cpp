#include "Area.h"
#include <algorithm>

namespace mtm {

	/**
	 * Constructor
	 * @param name The name of the area
	 * @throws AreaInvalidArguments If the name is empty
	 */
	Area::Area(const std::string& name) : name(name) {
		if(name == "") {
			throw AreaInvalidArguments();
		}
	}

	 /**
	 * Destructor
	 */
	Area::~Area() {
	}

	/**
	 * Add an area, that can be reachable from this area.
	 * Doesn't mean that this area is reachable from the area with the
	 * given name.
	 * @param area_name The name of the area that can be reachable from
	 *  this are.
	 */
	void Area::addReachableArea(const std::string& area_name) {
		reachable_areas.push_back(area_name);
	}

	/**
	 * Check if an area is reachable from this area.
	 * An area is always reachable from itself.
	 * @param area_name The name of the area to check if it's reachable.
	 * @return True if the area with the given name is reachable from
	 * this area, and false otherwise.
	 */
	bool Area::isReachable(const std::string& area_name) const {
		if(area_name == name) {
			return true;
		}
		else {
			return (std::find(reachable_areas.begin(), reachable_areas.end(),
							  area_name) != reachable_areas.end());
		}
	}

	/**
	 * Get a group into the area.
	 * @param group_name The name of the group that get into the area.
	 * @param clan The name of the clan that the group belongs to.
	 * @param clan_map The map of clans that contains the clan of the group
	 * @throws AreaClanNotFoundInMap If there is no clan with the given
	 * name in the map.
	 * @throws AreaGroupNotInClan If there is no group with the given name
	 * in the clan with the given name in the map.
	 * @throws AreaGroupAlreadyIn If group with same name already in the
	 *  area.
	 *
	 * Assert that the map does contain the clan that the group belongs to.
	 */
	void Area::groupArrive(const string& group_name, const string& clan,
							 map<string, Clan>& clan_map) {
		if(clan_map.count(clan) < 1) {
			throw AreaClanNotFoundInMap();
		}
		if(clan_map.find(clan)->second.doesContain(group_name) == false) {
			throw AreaGroupNotInClan();
		}
		for(std::vector<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			if((*it)->getName() == group_name) {
				throw AreaGroupAlreadyIn();
			}
		}
		groups.push_back(clan_map.find(clan)->second.getGroup(group_name));
	}

	/**
	 * Get a group out of the area.
	 * @param group_name The name of the leaving group.
	 * @throws AreaGroupNotFound If there is no group in the area with the
	 *  same name;
	 */
	void Area::groupLeave(const std::string& group_name) {
		for(std::vector<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			if((*it)->getName() == group_name) {
				groups.erase(it);
				return;
			}
		}
		throw AreaGroupNotFound();
	}

	/**
	 * Get a set of the names of all the groups in the area.
	 * @return A set that contains the names of all the groups in the area.
	 */
	MtmSet<std::string> Area::getGroupsNames() const {
		MtmSet<std::string> names;
		for(std::vector<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			names.insert((*it)->getName());
		}
		return names;
	}

	/**
	 * get the clan of the group stored in "groups"
	 * @param group_name The name of the group
	 * @return the name of the clan of the group if it exists,
	 * an empty string otherwise
	 */
	const string Area::getClan(const string& group_name) {
		for(std::vector<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			if((*it)->getName() == group_name) {
				return (*it)->getClan();
			}
		}
		return "";
	}

	/**
	 * get the strongest group in the clan of the group(not including the group)
	 * in the area
	 * @param clan The name of the clan
	 * @return the strongest group in "groups", if none exist then nullptr
	 */
	const GroupPointer Area::getStrongestInClan(const string& group) {
		const string clan_name = getClan(group);
		GroupPointer strongest = nullptr;
		for(std::vector<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			if((*it)->getName() != group && (*it)->getClan() == clan_name) {
				if(strongest == nullptr) {
					strongest = *it;
				}
				else if(strongest < *it) {
					strongest = *it;
				}
			}
		}
		return strongest;
	}

	/**
	 * get the strongest group in the area(not including "group")
	 * @return the strongest group, if none exist then nullptr
	 */
	const GroupPointer Area::getStrongestGroup(const string& group) {
		GroupPointer strongest = nullptr;
		for(std::vector<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			if((*it)->getName() != group) {
				if(strongest == nullptr) {
					strongest = *it;
				}
				else if(strongest < *it) {
					strongest = *it;
				}
			}
		}
		return strongest;
	}

		/**
		 * get the strongest group in the area(not including groups in
		 * filter_groups) that is in the clan or is in a clan that is
		 * friends with the clan
		 * @param filter_groups the groups that cannot be returned as the
		 * strongest group
		 * @param clan the clan that the strongest group belongs to or is
		 * friends with
		 * @return the strongest group, if none exist then nullptr
		 */
		const GroupPointer Area::getStrongestGroupFriend(
				const std::vector<string>& filter_groups, const Clan& clan) {
		GroupPointer strongest = nullptr;
		for(std::vector<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			//if group was not found
			if(std::find(filter_groups.begin(), filter_groups.end(),
						(*it)->getName()) == filter_groups.end()) {
				//if the groups' clans are friends
				if(clan.isFriend(Clan((*it)->getClan()))) {
					if(strongest == nullptr) {
						strongest = *it;
					}
					else if(strongest < *it) {
						strongest = *it;
					}
				}
			}
		}
		return strongest;

	}
}
