#ifndef MTM4_PLAIN_H
#define MTM4_PLAIN_H

#include <string>
#include <map>
#include "Clan.h"
#include "Group.h"
#include <vector>
#include <memory>

using std::string;
using std::shared_ptr;
using std::map;

namespace mtm{
    
    /**
     * An abstract call of an area in the world.
     * Assume every name is unique.
     * Groups that become empty, should be removed from the area.
     */
    class Plain : public Area{
    private:

    	/**
		 * Get the divided group name
		 * the divided name will be ("group_name" + "_" + number)
		 * the number is bigger then 1 the divided name will be a different name
		 * from all the group names in clan_map
		 * @param group_name The name of the group that get into the area
		 * @param clan_map The map of clans that contains the clan of the group
		 */
    	const std::string getGroupDivideName(const string& group_name,
    										 map<string, Clan>& clan_map);

    public:
        
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
        void groupArrive(const string& group_name, const string& clan,
                                 map<string, Clan>& clan_map);
    };
} //namespace mtm

#endif //MTM4_AREA_H
