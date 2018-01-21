#include <string>
#include "World.h"
#include "Area.h"
#include "Plain.h"
#include "Mountain.h"
#include "River.h"
#include <algorithm>

namespace mtm {

	/**
	 * Empty constructor
	 */
	World::World() {

	}

	/**
	 * Empty destructor
	 */
	World::~World() {

	}

	/**
	 * Add a new clan to the world.
	 * @param new_clan The name of the new clan
	 * @throws WorldInvalidArgument If new_clan is empty
	 * @throws WorldClanNameIsTaken If there is or was a clan with the
	 *  given name.
	 */
	void World::addClan(const string& new_clan) {
		if(new_clan == "") {
			throw WorldInvalidArgument();
		}
		if(clan_map.find(new_clan) == clan_map.end()) {
			throw WorldClanNameIsTaken();
		}
		clan_map.insert(std::pair<string, Clan>(new_clan, Clan(new_clan)));
	}

	/**
	 * Add a new area to the world.
	 * @param area_name The name of the area
	 * @param type The type of the area (PLAIN, MOUNTAIN, RIVER)
	 * @throws WorldInvalidArgument If area_name is empty
	 * @throws WorldAreaNameIsTaken If there is already an area with the
	 *  given name.
	 */
	void World::addArea(const string& area_name, AreaType type) {
		if(area_name == "") {
			throw WorldInvalidArgument();
		}
		if(areas.find(area_name) == areas.end()) {
			throw WorldAreaNameIsTaken();
		}
		switch(type) {
		case PLAIN:
			areas[area_name] = AreaPtr(new Plain(area_name));
			break;
		case MOUNTAIN:
			areas[area_name] = AreaPtr(new Mountain(area_name));
			break;
		case RIVER:
			areas[area_name] = AreaPtr(new River(area_name));
			break;
		}
	}

	/**
	 * Get the group with the name group_name in the world
	 * @param group_name the name of the group
	 * @return the group that has the name group_name, if it doesn't exist then
	 * return nullptr
	 */
	const GroupPointer World::getGroup(const string& group_name) {
		GroupPointer group = nullptr;
		for(map<string, Clan>::const_iterator it = clan_map.begin();
			it != clan_map.end(); ++it) {
			if(it->second.doesContain(group_name)) {
				return it->second.getGroup(group_name);
			}
		}
		return group;
	}

	/**
	 * Add a new group to the world, with given size, clan, and starting
	 *  area (the group "arrives" to the area).
	 *  The group should have 77 morale when entering the area.
	 * @param group_name The name of the new group
	 * @param clan_name The name of the clan of the group.
	 * @param num_children Number of children in the group.
	 * @param num_adults Number of the adults in the group.
	 * @param area_name The name of the area the group starts in (arrives
	 *  to it first)
	 * @throws WorldInvalidArgument If group_name is empty, or at least
	 * one of num_children and num_adults is negative, or both are 0.
	 * @throws WorldGroupNameIsTaken If there is already a group with the
	 *  given name in the world.
	 * @throws WorldClanNotFound If there is no clan with the given name
	 *  in the world.
	 * @throws WorldAreaNotFound If there is no area with the given name
	 *  in the world.
	 */
	void World::addGroup(const string& group_name, const string& clan_name,
				 int num_children, int num_adults, const string& area_name) {
		if(group_name == "" || num_children < 0 || num_adults < 0 ||
				(num_children + num_adults == 0)) {
			throw WorldInvalidArgument();
		}
		if(getGroup(group_name) != nullptr) {
			throw WorldGroupNameIsTaken();
		}
		if(clan_map.find(clan_name) == clan_map.end()) {
			throw WorldClanNotFound();
		}
		if(areas.find(area_name) == areas.end()) {
			throw WorldAreaNotFound();
		}
		clan_map[clan_name].addGroup(Group(group_name,num_children,num_adults));
		areas[area_name]->groupArrive(group_name, clan_name, clan_map);
	}

	/**
	 * Make that an area reachable from another area.
	 * (make 'to' reachable from 'from')
	 * @param from The name of the area that the other area will be
	 * reachable from.
	 * @param to The name of the area that should be reachable from the
	 * other area.
	 * @throws WorldAreaNotFound If at least one of the areas isn't in
	 *  the world.
	 */
	void World::makeReachable(const string& from, const string& to) {
		if(areas.find(from) == areas.end() || areas.find(to) == areas.end()) {
			throw WorldAreaNotFound();
		}
		if(areas.find(from)->second->isReachable(to) == false) {
			areas.find(from)->second->addReachableArea(to);
		}
	}

	/**
	 * Move a group to destination area.
	 * @param group_name The name of the group that should move
	 * @param destination The name of the area the group should move to.
	 * @throws WorldGroupNotFound If there is no group with the given
	 *  name in the world.
	 * @throws WorldAreaNotFound If there is no area with the given name
	 *  in the world.
	 * @throws WorldGroupAlreadyInArea If the group is already in the
	 *  destination area.
	 * @throws WorldAreaNotReachable If the destination area isn't
	 *  reachable from the area the group is currently in.
	 */
	void World::moveGroup(const string& group_name, const string& destination) {

	}
}
