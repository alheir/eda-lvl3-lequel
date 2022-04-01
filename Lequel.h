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
TrigramProfile buildTrigramProfile(const Text &text);

void normalizeTrigramProfile(TrigramProfile &trigramProfile);

float getCosineSimilarity(TrigramProfile &textProfile, TrigramProfile &languageProfile);

void identifyLanguage(const Text &text, Languages &languages, std::string *languagesMatched);

#endif
