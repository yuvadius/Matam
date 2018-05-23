#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "series.h"
#include "set.h"

struct series_t {
	char* name;
	int episodesNum;
	Genre genre;
	int* ages;
	int episodeDuration;
	int rank;
};

Series createSeries(const char* name, int episodesNum, Genre genre, int* ages, int episodeDuration) {
	Series series = malloc(sizeof(*series));
	if(series == NULL)
		return NULL;
	series->name = malloc(sizeof(*series->name) * (strlen(name) + 1));
	if(series->name == NULL) {
		free(series);
		return NULL;
	}
	strcpy(series->name, name);
	series->episodesNum = episodesNum;
	series->genre = genre;
	if(ages == NULL)
		series->ages = NULL;
	else {
		series->ages = malloc(sizeof(*series->ages) * 2);
		if(series->ages == NULL) {
			free(series->name);
			free(series);
			return NULL;
		}
		series->ages[0] = (ages[0] < MTM_MIN_AGE) ? MTM_MIN_AGE : ages[0];
		series->ages[1] = (ages[1] > MTM_MAX_AGE) ? MTM_MAX_AGE : ages[1];
	}
	series->episodeDuration = episodeDuration;
	return series;
}

void freeSeries(SeriesElement series) {
	if(((Series)series) != NULL) {
		free(((Series)series)->ages);
		free(((Series)series)->name);
		free(((Series)series));
	}
}

int compareSeries(SeriesElement series1, SeriesElement series2) {
	if(((Series)series1) == NULL || ((Series)series2) == NULL) {
		return 0;
	}
	if(strcmp(((Series)series1)->name, ((Series)series2)->name) == 0) {
		return 0;
	}
	int genreRelation = strcmp(getGenreText(((Series)series1)), getGenreText(((Series)series2)));
	if(genreRelation != 0) {
		return genreRelation;
	}
	int nameRelation = strcmp(((Series)series1)->name, ((Series)series2)->name);
	return nameRelation;
}

int compareSeriesByRanking(SeriesElement series1, SeriesElement series2) {
	if(((Series)series1) == NULL || ((Series)series2) == NULL) {
		return 0;
	}
	if(strcmp(((Series)series1)->name, ((Series)series2)->name) == 0) {
		return 0;
	}
	int rankRelation = ((Series)series2)->rank - ((Series)series1)->rank;
	if(rankRelation != 0) {
		return rankRelation;
	}
	int nameRelation = strcmp(((Series)series1)->name, ((Series)series2)->name);
	return nameRelation;
}

SeriesElement getName(SeriesElement series) {
	if(series == NULL) {
		return NULL;
	}
	return ((Series)series)->name;
}

Genre getGenreValue(Series series) {
	assert(series != NULL);
	return series->genre;
}

char* getGenreText(Series series) {
	if(series == NULL) {
		return NULL;
	}
	switch(series->genre) {
		case SCIENCE_FICTION:
			return "SCIENCE_FICTION";
		case DRAMA:
			return "DRAMA";
		case COMEDY:
			return "COMEDY";
		case CRIME:
			return "CRIME";
		case MYSTERY:
			return "MYSTERY";
		case DOCUMENTARY:
			return "DOCUMENTARY";
		case ROMANCE:
			return "ROMANCE";
		case HORROR:
			return "HORROR";
		default:
			return NULL;
	}
}

int getEpisodeDuration(Series series) {
	if(series == NULL) {
		return 0;
	}
	return series->episodeDuration;
}

bool isValidSeriesName(const char* seriesName) {
	if(seriesName == NULL || strlen(seriesName) <= 0) {
		return false;
	}
	int length = strlen(seriesName);
	for(int i = 0; i < length; ++i) {
		if(!isalpha(*seriesName) && !isdigit(*seriesName)) {
			return false;
		}
		++seriesName;
	}
	return true;
}

bool isValidEpisodesNum(int episodesNum) {
	return (episodesNum > 0);
}

bool isValidEpisodeDuration(int episodeDuration) {
	return (episodeDuration > 0);
}

bool isAgeInRange(Series series, int age) {
	if(series == NULL) {
		return false;
	}
	if(series->ages == NULL) { //for all ages
		return true;
	}
	if(age >= series->ages[0] && age <= series->ages[1]) {
		return true;
	}
	else {
		return false;
	}
}

void setSeriesRank(Series series, int rank) {
	if(series != NULL) {
		series->rank = rank;
	}
}

int getSeriesRank(Series series) {
	if(series == NULL) {
		return 0;
	}
	return series->rank;
}
