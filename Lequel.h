/*
 * Lequel?
 *
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Language identification based on trigrams.
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

void identifyLanguage(const Text &text, Languages &languages, std::string* languagesMatched);

#endif
