#ifndef MTM4_MOUNTAIN_H
#define MTM4_MOUNTAIN_H

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
     * An area of type mountain
     * Assume every name is unique.
     * Groups that become empty, should be removed from the area.
     */
    class Mountain : public Area{
    private:
    	std::string controlling_group; //the group that controls the mountain

    public:

    	/**
		 * Constructor
		 * @param name The name of the mountain
		 * @throws AreaInvalidArguments If the name is empty
		 */
		explicit Mountain(const std::string& name);

		/**
		 * Disable copy constructor
		 */
		Mountain(const Mountain&) = delete;

		/**
		 * Disable assignment operator
		 */
		Mountain &operator=(const Mountain &) = delete;

		/**
		 * Destructor
		 */
		~Mountain();

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
		void groupArrive(const string& group_name, const string& clan,
						 map<string, Clan>& clan_map) override;

		/**
		 * Get a group out of the mountain.
		 * If the group isn't the controlling groups then nothing happens
		 * If the group is the controlling group then the strongest group from
		 * the controlling group's clan becomes the new controlling group
		 * if there is no such group then the strongest group in the mountain
		 * becomes the controlling group
		 * @param group_name The name of the leaving group.
		 */
		void groupLeave(const std::string& group_name) override;
    };
} //namespace mtm

#endif //MTM4_MOUNTAIN_H
