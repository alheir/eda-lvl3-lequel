/**
 * @file Lequel.h
 * @author Marc S. Ressl, Albertina Galan, Alejandro Nahuel Heir
 * @brief Language identification based on trigrams.
 * @version 0.1
 * @date 2022-03-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _LEQUEL_H
#define _LEQUEL_H

#include <list>
#include <map>
#include <string>

#include "Text.h"

// TrigramProfile: Map of trigram -> frequency
typedef std::map<std::string, float> TrigramProfile;

// TrigramList: list of trigrams
typedef std::list<std::string> TrigramList;

class Language
{
public:
    std::string languageCode;
    TrigramProfile trigramProfile;
};

typedef std::list<Language> Languages;

// Functions
/**
 * @brief Builds a trigram profile for a given text.
 *
 * @param text: A list of lines (Text).
 * @return TrigramProfile
 */
TrigramProfile buildTrigramProfile(const Text &text);

/**
 * @brief Normalizes a trigram profile.
 *
 * @param trigramProfile
 */
void normalizeTrigramProfile(TrigramProfile &trigramProfile);

/**
 * @brief Calculates the cosine similarity between a text trigram profile
 * and a language trigram profile.
 *
 * @param textProfile: Text trigram profile.
 * @param languageProfile: Language trigram profile.
 * @return float: the cosine similarity score.
 */
float getCosineSimilarity(TrigramProfile &textProfile, TrigramProfile &languageProfile);

/**
 * @brief Identifies the language of a text.
 *
 * @param text: A list of lines (Text).
 * @param languages: the trigram profiles of all languages
 * @param languagesMatched: an array of strings that will contain the top 3 languages matched
 */
void identifyLanguage(const Text &text, Languages &languages, std::string *languagesMatched);

#endif
