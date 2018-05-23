#ifndef SERIES_H
#define SERIES_H

#include <stdbool.h>
#include "mtm_ex3.h"

/** The Series struct */
typedef struct series_t* Series;

/** Series element data type */
typedef void* SeriesElement;

/** Create a Series and return it, return NULL on error */
Series createSeries(const char* name, int episodesNum, Genre genre, int* ages, int episodeDuration);

/** Type of function for freeing a series */
void freeSeries(SeriesElement series);

/**
* This function should return:
* 		0 if they're equal(meaning they share the same name)
* 		A positive integer if the first series genre is bigger than the second
* 		series genre or if they are equal and the first series name is bigger
* 		than the second series name
* 		A negative integer if the first series genre is smaller than the second
* 		series genre or if they are equal and the first series name is smaller
* 		than the second series name
* If one of the parameters is invalid the 0 will be returned
*/
int compareSeries(SeriesElement series1, SeriesElement series2);

/**
* This function should return:
* 		0 if they're equal(meaning they share the same name)
* 		A positive integer if the first series rank is smaller than the second
* 		series rank or if they are equal and the first series name is bigger
* 		than the second series name
* 		A negative integer if the first series genre is bigger than the second
* 		series genre or if they are equal and the first series name is smaller
* 		than the second series name
* If one of the parameters is invalid the 0 will be returned
*/
int compareSeriesByRanking(SeriesElement series1, SeriesElement series2);

/** Return the series name, Will return NULL on error */
SeriesElement getName(SeriesElement series);

/** Return the genre value of the series, assert(series != NULL) will be used */
Genre getGenreValue(Series series);

/** This function returns the string value of the genre, Will return NULL on error */
char* getGenreText(Series series);

/** This function return the episode duration of the series, 0 on error */
int getEpisodeDuration(Series series);

/** Get the series rank, or 0 on error */
int getSeriesRank(Series series);

/**
 * This function should return:
 * 		true if seriesName is not empty and does not contain any non english,
 * 		decimal characters
 * 		false otherwise
 */
bool isValidSeriesName(const char* seriesName);

/**
 * This function should return:
 * 		true if the episodesNum > 0
 * 		false otherwise
 */
bool isValidEpisodesNum(int episodesNum);

/**
 * This function should return:
 * 		true if the episodeDuration > 0
 * 		false otherwise
 */
bool isValidEpisodeDuration(int episodeDuration);

/**
 * Return true if the series can be watched by a user of the age "age"
 * false on error or otherwise
 */
bool isAgeInRange(Series series, int age);

/**
 * Set the series rank to the parameter rank
 */
void setSeriesRank(Series series, int rank);

#endif
