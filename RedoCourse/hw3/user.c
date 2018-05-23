#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "user.h"
#include "util.h"

struct user_t {
	char* username;
	int age;
	Set friends;
	Set favoriteSeries;
};

User createUser(const char* username, int age) {
	User user = malloc(sizeof(*user));
	if(user == NULL)
		return NULL;
	user->age = age;
	user->username = malloc(sizeof(*user->username) * (strlen(username) + 1));
	if(user->username == NULL) {
		free(user);
		return NULL;
	}
	strcpy(user->username, username);
	user->friends = setCreate(copyPointer, freePointer, compareUsers);
	if(user->friends == NULL) {
		free(user->username);
		free(user);
		return NULL;
	}
	user->favoriteSeries = setCreate(copyPointer, freePointer, compareSeries);
	if(user->favoriteSeries == NULL) {
		setDestroy(user->friends);
		free(user->username);
		free(user);
		return NULL;
	}
	return user;
}

UserElement getUsername(UserElement user) {
	if(user == NULL) {
		return NULL;
	}
	return ((User)user)->username;
}

int getAge(User user) {
	if(user == NULL) {
		return -1;
	}
	return user->age;
}

Set getFriends(User user) {
	if(user == NULL) {
		return NULL;
	}
	return user->friends;
}

Set getFavoriteSeries(User user) {
	if(user == NULL) {
		return NULL;
	}
	return user->favoriteSeries;
}

void freeUser(UserElement user) {
	if(((User)user) != NULL) {
		setDestroy(((User)user)->favoriteSeries);
		setDestroy(((User)user)->friends);
		free(((User)user)->username);
		free(((User)user));
	}
}

int compareUsers(UserElement user1, UserElement user2) {
	assert((User)user1 != NULL && (User)user2 != NULL);
	return strcmp(((User)user1)->username, ((User)user2)->username);
}

bool isValidUserName(const char* username) {
	if(username == NULL || strlen(username) <= 0) {
		return false;
	}
	int length = strlen(username);
	for(int i = 0; i < length; ++i) {
		if(!isalpha(*username) && !isdigit(*username)) {
			return false;
		}
		++username;
	}
	return true;
}

bool isValidAge(int age) {
	return (age >= MTM_MIN_AGE && age <= MTM_MAX_AGE);
}

int getNumberOfLikedGenre(User user, Genre genre) {
	if(user == NULL) {
		return 0;
	}
	int counter = 0;
	SET_FOREACH(Series, series, user->favoriteSeries) {
		if(getGenreValue(series) == genre) {
			++counter;
		}
	}
	return counter;
}

float averageEpisodeDuration(User user) {
	if(user == NULL) {
		return 0;
	}
	float episodeCounter = 0;
	float episodeDurationCounter = 0;
	SET_FOREACH(Series, series, user->favoriteSeries) {
		episodeCounter += 1;
		episodeDurationCounter += getEpisodeDuration(series);
	}
	return (episodeDurationCounter / episodeCounter);
}

int getAmountOfFriendsThatLikeSeries(User user, Series series) {
	if(user == NULL || series == NULL) {
		return 0;
	}
	int counter = 0;
	SET_FOREACH(User, friend, user->friends) {
		if(setIsIn(friend->favoriteSeries, series) == true) {
			++counter;
		}
	}
	return counter;
}

bool isSeriesIntresting(SeriesElement series, UserElement user) {
	return (isAgeInRange((Series)series, (((User)user)->age)) &&
			!setIsIn((((User)user))->favoriteSeries, (Series)series));
}

void setRank(Series series, User user) {
	if(series != NULL && user != NULL) {
		float G = getNumberOfLikedGenre(user, getGenreValue(series));
		float L = averageEpisodeDuration(user);
		float CUR = getEpisodeDuration(series);
		float F = getAmountOfFriendsThatLikeSeries(user, series);
		setSeriesRank(series, (int)((G*F)/(1+fabs(CUR - L))));
	}
}
