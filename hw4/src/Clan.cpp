#include <math.h>
#include <stdlib.h> /* abs */
#include <algorithm>
#include "Clan.h"
#include "exceptions.h"

namespace mtm{
	/**
	 * Constructor
	 * @param name The name of the new clan, can't be empty
	 * @throws ClanEmptyName if name is empty.
	 */
	Clan::Clan(const std::string& name) : name(name) {
		if(name == "") {
			throw ClanEmptyName();
		}
	}

	/**
	 * Copy constructor.
	 */
	Clan::Clan(const Clan& other) : name(other.name), groups(other.groups),
									clans(other.clans) {
	}

	/**
	 * Destructor
	 */
	Clan::~Clan() {

	}

	/**
	 * Add a group (copy of it) to the clan
	 * @param group The group to add to the clan
	 * @throws ClanGroupIsEmpty if the group has no people (empty)
	 * @throws ClanGroupNameAlreadyTaken if there is already a group
	 * with the same name in the clan.
	 */
	void Clan::addGroup(const Group& group) {
		if(group.getSize() == 0) {
			throw ClanGroupIsEmpty();
		}
		for(MtmSet<GroupPointer>::iterator it = groups.begin();
			it != groups.end(); ++it) {
			if(group.getName() == (*it)->getName()) {
				throw ClanGroupNameAlreadyTaken();
			}
		}
		GroupPointer new_group(new Group(group));
		new_group->changeClan(name);
		groups.insert(new_group);
	}

	/**
	 * Get a group from the clan, with a given name
	 * @param group_name The name of the group the return
	 * @return A reference to the group with the given name
	 * @throws ClanGroupNotFound if there is no group in the clan with the
	 * given name.
	 */
	const GroupPointer& Clan::getGroup(const std::string& group_name) const {
		for(MtmSet<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			if(group_name == (*it)->getName()) {
				return *it;
			}
		}
		throw ClanGroupNotFound();
	}

	bool Clan::doesContain(const std::string& group_name) const {
		for(MtmSet<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			if(group_name == (*it)->getName()) {
				return true;
			}
		}
		return false;
	}

	/**
	 * The function returns the amount of people in the clan.
	 * A person belongs to the clan, if he belongs to a group, that
	 * belongs to the clan.
	 * @return The amount of people in the clan.
	 */
	int Clan::getSize() const {
		int counter = 0;
		for(MtmSet<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			counter += (*it)->getSize();
		}
		return counter;
	}

	/**
	 * Make two clans unite, to form a new clan, with a new name. All the
	 * groups of each clan will change their clan.
	 * The friends of the new clan are all the friends of either one of
	 * the old 2 clans.
	 * The new clan will replace this clan, and the other clan (received
	 * by an argument) will become empty (i.e no groups).
	 *
	 * If One of the exception is thrown, both clan shouldn't change.
	 * @param other The clan to unite with
	 * @param new_name The name of the new clan. If the name is of one of
	 *  the uniting clans (assume to clan A), that means the other clan,
	 *  joined clan A, and the groups of the clan A, don't change their
	 *  clan.
	 * @return A reference to the new clan (this).
	 * @throws ClanEmptyName new_name is empty.
	 * @throws ClanCantUnite If other is the same clan as this or if
	 *  there is a group in one of the clans, that has the same name as a
	 *  group in the other clan.
	 */
	Clan& Clan::unite(Clan& other, const std::string& new_name) {
		if(new_name == "") {
			throw ClanEmptyName();
		}
		if(name == other.name) {
			throw ClanCantUnite();
		}
		for(MtmSet<GroupPointer>::const_iterator it = groups.begin();
			it != groups.end(); ++it) {
			if(other.doesContain((*it)->getName())) {
				throw ClanCantUnite();
			}
		}
		if(name != new_name) {
			name = new_name;
			for(MtmSet<GroupPointer>::iterator it = groups.begin();
				it != groups.end(); ++it) {
				(*it)->changeClan(name);
			}
		}
		for(MtmSet<GroupPointer>::iterator it = other.groups.begin();
			it != other.groups.end(); ++it) {
			addGroup(*(*it));
		}
		clans.unite(other.clans);
		other.groups.clear();
		return *this;
	}

	/**
	 * Make this clan, and a given clan, friends. If the two clans already
	 * were friends, does nothing.
	 * Assume that every clan has a unique name. (In copied clans, you
	 * can assume one of the copies will not be used again).
	 * @param other The clan to become a friend with.
	 */
	void Clan::makeFriend(Clan& other) {
		if(isFriend(other) == false) {
			std::shared_ptr<Clan> other_copy(new Clan(other));
			clans.insert(other_copy);
			std::shared_ptr<Clan> this_copy(new Clan(*this));
			other.clans.insert(this_copy);
		}
	}

	/**
	 * Check if a given clan is a friend of this clan.
	 * Every clan is a friend of itself.
	 * @param other The clan to check if is a friend.
	 * @return true, if the given clan is a friend of this clan, false
	 *  otherwise.
	 */
	bool Clan::isFriend(const Clan& other) const {
		if(name == other.name) {
			return true;
		}
		for(MtmSet<std::shared_ptr<Clan>>::const_iterator it = clans.begin();
			it != clans.end(); ++it) {
			if((*it)->name == other.name) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Print The clan name, and it groups, sorted by groups comparison
	 * operators, from strongest to weakest. In the next Format:
	 *
	 * Clan's name: [name]
	 *     [1'st group name]
	 *     [2'nd group name]
	 *     [3'rd group name]
	 *             *
	 *             *
	 *             *
	 *
	 * @param os The output stream
	 * @param clan The clan to print
	 * @return A reference to the output stream
	 */
	std::ostream& operator<<(std::ostream& os, const Clan& clan) {
		std::list<Group> sorted_groups;
		for(MtmSet<GroupPointer>::const_iterator it = clan.groups.begin();
			it != clan.groups.end(); ++it) {
			sorted_groups.push_front((*(*it)));
		}
		//sort from strongest to weakest group
		sorted_groups.sort(std::greater<Group>());
		os << "Clan's name: " << clan.name << std::endl;
		os << "Clan's groups:" << std::endl;
		for(std::list<Group>::iterator it = sorted_groups.begin();
			it != sorted_groups.end(); ++it) {
			os << (*it).getName() << std::endl;
		}
		return os;
	}
}
