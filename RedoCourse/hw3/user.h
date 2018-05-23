#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include "set.h"
#include "mtm_ex3.h"
#include "series.h"

/** The User struct */
typedef struct user_t* User;

/** User element data type */
typedef void* UserElement;

/** Create a User and return it, return NULL on error */
User createUser(const char* username, int age);

/** Get the username of the user, return NULL on error */
UserElement getUsername(UserElement user);

/** Get the age of the user, return -1 on error */
int getAge(User user);

/** Get the friends of the user, return NULL on error */
Set getFriends(User user);

/** Get the favorite series of the user, return NULL on error */
Set getFavoriteSeries(User user);

/** Type of function for freeing a user */
void freeUser(UserElement user);

/**
* This function should return:
* 		A positive integer if the first user name is bigger than the second
* 		A negative integer if the first user name is smaller than the second
* 		0 if they're equal;
* If one of the parameters is invalid the 0 will be returned
*/
int compareUsers(UserElement user1, UserElement user2);

/**
 * This function should return:
 * 		true if username is not empty and does not contain any non english,
 * 		decimal characters
 * 		false otherwise
 */
bool isValidUserName(const char* username);

/**
 * This function should return:
 * 		true if the age is between MTM_MIN_AGE and MTM_MAX_AGE
 * 		false otherwise
 */
bool isValidAge(int age);

/**
 * This function should return:
 * 		the number of series with the genre "genre" that the user liked
 * 		if there was an error return 0
 */
int getNumberOfLikedGenre(User user, Genre genre);

/** Get the average episode duration of the users liked series, 0 on error */
float averageEpisodeDuration(User user);

/** Get the amount of friends of "user" that like the "series", 0 on error */
int getAmountOfFriendsThatLikeSeries(User user, Series series);

/**Return true if series in range and if series is not already liked by the user
 * Return false otherwise
 */
bool isSeriesIntresting(SeriesElement series, UserElement user);

/** Sets the rank of the series as seen by the "user" */
void setRank(Series series, User user);

#endif
