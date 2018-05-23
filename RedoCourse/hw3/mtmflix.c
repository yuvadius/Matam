#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "mtmflix.h"
#include "set.h"
#include "user.h"
#include "series.h"
#include "util.h"

struct mtmFlix_t {
	Set users;
	Set series;
};

MtmFlix mtmFlixCreate() {
	MtmFlix mtmflix = malloc(sizeof(*mtmflix));
	if(mtmflix == NULL) {
		return NULL;
	}
	mtmflix->users = setCreate(copyPointer, freeUser, compareUsers);
	if(mtmflix->users == NULL) {
		free(mtmflix);
		return NULL;
	}
	mtmflix->series = setCreate(copyPointer, freeSeries, compareSeries);
	if(mtmflix->series == NULL) {
		setDestroy(mtmflix->users);
		free(mtmflix);
		return NULL;
	}
	return mtmflix;
}

void mtmFlixDestroy(MtmFlix mtmflix) {
	if(mtmflix != NULL) {
		setDestroy(mtmflix->series);
		setDestroy(mtmflix->users);
		free(mtmflix);
	}
}

MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix, const char* username, int age) {
	if(mtmflix == NULL || username == NULL) {
		return MTMFLIX_NULL_ARGUMENT;
	}
	if(isValidUserName(username) == false) {
		return MTMFLIX_ILLEGAL_USERNAME;
	}
	User user = createUser(username, age);
	if(user == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	if(setIsIn(mtmflix->users, user) == true) {
		freeUser(user);
		return MTMFLIX_USERNAME_ALREADY_USED;
	}
	if(isValidAge(age) == false) {
		freeUser(user);
		return MTMFLIX_ILLEGAL_AGE;
	}
	if(setAdd(mtmflix->users, user) == SET_OUT_OF_MEMORY) {
		freeUser(user);
		return MTMFLIX_OUT_OF_MEMORY;
	}
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveUser(MtmFlix mtmflix, const char* username) {
	if(mtmflix == NULL || username == NULL) {
		return MTMFLIX_NULL_ARGUMENT;
	}
	User user = createUser(username, 0);
	if(user == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	SET_FOREACH(User, userElement, mtmflix->users) {
		setRemove(getFriends(userElement), user);
	}
	if(setRemove(mtmflix->users, user) == SET_ITEM_DOES_NOT_EXIST) {
		freeUser(user);
		return MTMFLIX_USER_DOES_NOT_EXIST;
	}
	freeUser(user);
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixAddSeries(MtmFlix mtmflix, const char* name, int episodesNum, Genre genre, int* ages, int episodesDuration) {
	if(mtmflix == NULL || name == NULL) {
		return MTMFLIX_NULL_ARGUMENT;
	}
	if(isValidSeriesName(name) == false) {
		return MTMFLIX_ILLEGAL_SERIES_NAME;
	}
	Series series = createSeries(name, episodesNum, genre, ages, episodesDuration);
	if(series == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	if(setIsIn(mtmflix->series, series) == true) {
		freeSeries(series);
		return MTMFLIX_SERIES_ALREADY_EXISTS;
	}
	if(isValidEpisodesNum(episodesNum) == false) {
		freeSeries(series);
		return MTMFLIX_ILLEGAL_EPISODES_NUM;
	}
	if(isValidEpisodeDuration(episodesDuration) == false) {
		freeSeries(series);
		return MTMFLIX_ILLEGAL_EPISODES_DURATION;
	}
	if(setAdd(mtmflix->series, series) == SET_OUT_OF_MEMORY) {
		freeSeries(series);
		return MTMFLIX_OUT_OF_MEMORY;
	}
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveSeries(MtmFlix mtmflix, const char* name) {
	if(mtmflix == NULL || name == NULL) {
		return MTMFLIX_NULL_ARGUMENT;
	}
	Series series = createSeries(name, 0, 0, NULL, 0);
	if(series == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	SET_FOREACH(User, userElement, mtmflix->users) {
		setRemove(getFavoriteSeries(userElement), series);
	}
	if(setRemove(mtmflix->series, series) == SET_ITEM_DOES_NOT_EXIST) {
		freeSeries(series);
		return MTMFLIX_SERIES_DOES_NOT_EXIST;
	}
	freeSeries(series);
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixReportSeries(MtmFlix mtmflix, int seriesNum, FILE* outputStream) {
	if(mtmflix == NULL || outputStream == NULL)
		return MTMFLIX_NULL_ARGUMENT;
	if(setGetSize(mtmflix->series) <= 0)
		return MTMFLIX_NO_SERIES;
	List list = copySetToList(mtmflix->series, NULL);
	if(list == NULL)
		return MTMFLIX_OUT_OF_MEMORY;
	if(listSort(list, compareSeries) != LIST_SUCCESS) {
		listDestroy(list);
		return MTMFLIX_OUT_OF_MEMORY;
	}
	int genreCounter = seriesNum;
	Genre genre = getGenreValue(listGetFirst(list));
	LIST_FOREACH(Series, series, list) {
		if(genre != getGenreValue(series)) {
			genreCounter = seriesNum;
			genre = getGenreValue(series);
		}
		if(seriesNum == 0 || genreCounter != 0) {
			const char* name = mtmPrintSeries(getName(series), getGenreText(series));
			if(name == NULL) {
				listDestroy(list);
				return MTMFLIX_OUT_OF_MEMORY;
			}
			fputs(name, outputStream);
			--genreCounter;
		}
	}
	listDestroy(list);
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixReportUsers(MtmFlix mtmflix, FILE* outputStream) {
	if(mtmflix == NULL || outputStream == NULL)
		return MTMFLIX_NULL_ARGUMENT;
	if(setGetSize(mtmflix->users) <= 0)
		return MTMFLIX_NO_USERS;
	List list = copySetToList(mtmflix->users, NULL);
	if(list == NULL)
		return MTMFLIX_OUT_OF_MEMORY;
	if(listSort(list, compareUsers) != LIST_SUCCESS) {
		listDestroy(list);
		return MTMFLIX_OUT_OF_MEMORY;
	}
	LIST_FOREACH(User, user, list) {
		List userFriends = copySetToList(getFriends(user), getUsername);
		if(userFriends == NULL) {
			listDestroy(list);
			return MTMFLIX_OUT_OF_MEMORY;
		}
		List userFavoriteSeries = copySetToList(getFavoriteSeries(user), getName);
		if(userFavoriteSeries == NULL) {
			listDestroy(userFriends);
			listDestroy(list);
			return MTMFLIX_OUT_OF_MEMORY;
		}
		const char* name = mtmPrintUser(getUsername(user), getAge(user), userFriends, userFavoriteSeries);
		if(name == NULL) {
			listDestroy(userFavoriteSeries);
			listDestroy(userFriends);
			listDestroy(list);
			return MTMFLIX_OUT_OF_MEMORY;
		}
		fputs(name, outputStream);
		listDestroy(userFavoriteSeries);
		listDestroy(userFriends);
	}
	listDestroy(list);
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixSeriesJoin(MtmFlix mtmflix, const char* username, const char* seriesName) {
	if(mtmflix == NULL || username == NULL || seriesName == NULL) {
		return MTMFLIX_NULL_ARGUMENT;
	}
	User user = createUser(username, 0);
	if(user == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	User originalUser = getElement(mtmflix->users, user, compareUsers);
	freeUser(user);
	if(originalUser == NULL) {
		return MTMFLIX_USER_DOES_NOT_EXIST;
	}
	Series series = createSeries(seriesName, 0, 0, NULL, 0);
	if(series == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	Series originalSeries = getElement(mtmflix->series, series, compareSeries);
	freeSeries(series);
	if(originalSeries == NULL) {
		return MTMFLIX_SERIES_DOES_NOT_EXIST;
	}
	if(isAgeInRange(originalSeries, getAge(originalUser)) == false) {
		return MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE;
	}
	if(setAdd(getFavoriteSeries(originalUser), originalSeries) == SET_OUT_OF_MEMORY) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixSeriesLeave(MtmFlix mtmflix, const char* username, const char* seriesName) {
	if(mtmflix == NULL || username == NULL || seriesName == NULL) {
		return MTMFLIX_NULL_ARGUMENT;
	}
	User user = createUser(username, 0);
	if(user == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	User originalUser = getElement(mtmflix->users, user, compareUsers);
	freeUser(user);
	if(originalUser == NULL) {
		return MTMFLIX_USER_DOES_NOT_EXIST;
	}
	Series series = createSeries(seriesName, 0, 0, NULL, 0);
	if(series == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	Series originalSeries = getElement(mtmflix->series, series, compareSeries);
	freeSeries(series);
	if(originalSeries == NULL) {
		return MTMFLIX_SERIES_DOES_NOT_EXIST;
	}
	setRemove(getFavoriteSeries(originalUser), originalSeries);
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixAddFriend(MtmFlix mtmflix, const char* username1, const char* username2) {
	if(mtmflix == NULL || username1 == NULL || username2 == NULL) {
		return MTMFLIX_NULL_ARGUMENT;
	}
	User user1 = createUser(username1, 0);
	if(user1 == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	User originalUser1 = getElement(mtmflix->users, user1, compareUsers);
	freeUser(user1);
	if(originalUser1 == NULL) {
		return MTMFLIX_USER_DOES_NOT_EXIST;
	}
	User user2 = createUser(username2, 0);
	if(user2 == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	User originalUser2 = getElement(mtmflix->users, user2, compareUsers);
	freeUser(user2);
	if(originalUser2 == NULL) {
		return MTMFLIX_USER_DOES_NOT_EXIST;
	}
	if(strcmp(username1, username2) != 0) { //you cant friend yourself
		if(setAdd(getFriends(originalUser1), originalUser2) == SET_OUT_OF_MEMORY) {
			return MTMFLIX_OUT_OF_MEMORY;
		}
	}
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixRemoveFriend(MtmFlix mtmflix, const char* username1, const char* username2) {
	if(mtmflix == NULL || username1 == NULL || username2 == NULL) {
		return MTMFLIX_NULL_ARGUMENT;
	}
	User user1 = createUser(username1, 0);
	if(user1 == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	User originalUser1 = getElement(mtmflix->users, user1, compareUsers);
	freeUser(user1);
	if(originalUser1 == NULL) {
		return MTMFLIX_USER_DOES_NOT_EXIST;
	}
	User user2 = createUser(username2, 0);
	if(user2 == NULL) {
		return MTMFLIX_OUT_OF_MEMORY;
	}
	User originalUser2 = getElement(mtmflix->users, user2, compareUsers);
	freeUser(user2);
	if(originalUser2 == NULL) {
		return MTMFLIX_USER_DOES_NOT_EXIST;
	}
	setRemove(getFriends(originalUser1), originalUser2);
	return MTMFLIX_SUCCESS;
}

MtmFlixResult mtmFlixGetRecommendations(MtmFlix mtmflix, const char* username, int count, FILE* outputStream) {
	if(mtmflix == NULL || username == NULL || outputStream == NULL)
		return MTMFLIX_NULL_ARGUMENT;
	User user = createUser(username, 0);
	if(user == NULL)
		return MTMFLIX_OUT_OF_MEMORY;
	User originalUser = getElement(mtmflix->users, user, compareUsers);
	freeUser(user);
	if(originalUser == NULL)
		return MTMFLIX_USER_DOES_NOT_EXIST;
	if(count < 0)
		return MTMFLIX_ILLEGAL_NUMBER;
	List list = copySetToList(mtmflix->series, NULL);
	if(list == NULL)
		return MTMFLIX_OUT_OF_MEMORY;
	List filteredList = listFilter(list, isSeriesIntresting, originalUser);
	listDestroy(list);
	if(filteredList == NULL)
		return MTMFLIX_OUT_OF_MEMORY;
	LIST_FOREACH(Series, series, filteredList)
		setRank(series, originalUser); //Set the rank of the series as the user sees it
	if(listSort(filteredList, compareSeriesByRanking) == LIST_OUT_OF_MEMORY) {
		listDestroy(filteredList);
		return MTMFLIX_OUT_OF_MEMORY;
	}
	LIST_FOREACH(Series, series, filteredList) {
		if(getSeriesRank(series) != 0) {
			const char* name = mtmPrintSeries(getName(series), getGenreText(series));
			if(name == NULL) {
				listDestroy(filteredList);
				return MTMFLIX_OUT_OF_MEMORY;
			}
			fputs(name, outputStream);
			if(count == 1)
				break;
			if(count != 0)
				--count;
		}
	}
	listDestroy(filteredList);
	return MTMFLIX_SUCCESS;
}
