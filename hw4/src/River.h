#ifndef MTM4_RIVER_H
#define MTM4_RIVER_H

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
     * An area of type river
     * Assume every name is unique.
     * Groups that become empty, should be removed from the area.
     */
    class River : public Area {

    public:

    	/**
		 * Constructor
		 * @param name The name of the river
		 * @throws AreaInvalidArguments If the name is empty
		 */
		explicit River(const std::string& name);

		/**
		 * Disable copy constructor
		 */
		River(const River&) = delete;

		/**
		 * Disable assignment operator
		 */
		River &operator=(const River &) = delete;

		/**
		 * Destructor
		 */
		~River();
        
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
        void groupArrive(const string& group_name, const string& clan,
                                 map<string, Clan>& clan_map) override;
    };
} //namespace mtm

#endif //MTM4_RIVER_H
