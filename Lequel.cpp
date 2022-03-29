/*
 * Lequel?
 *
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Language identification based on trigrams.
 *
 * More info: https://towardsdatascience.com/understanding-cosine-similarity-and-its-application-fd42f585296a
 */

#include <cmath>
#include <codecvt>
#include <locale>
#include <iostream>

#include "Lequel.h"

using namespace std;

/*
 * Builds a trigram profile for a given text.
 *
 * Parameters:
 *  text -      A list of lines (Text).
 *
 * Returns: the trigram profile.
 */
TrigramProfile buildTrigramProfile(const Text &text)
{
    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    TrigramProfile profile;

    for (auto x : text)
    {
        if (x.length() < 3)
            continue;

        wstring unicodeString = converter.from_bytes(x);

        for (int i = 0; i < unicodeString.length() - 2; i++)
        {
            wstring trigram = unicodeString.substr(i, 3);
            profile[converter.to_bytes(trigram)] += 1;
        }
    }

    return profile;
}

/*
 * Normalizes a trigram profile.
 *
 * Parameters:
 *  trigramProfile -    Trigram profile.
 */
void normalizeTrigramProfile(TrigramProfile &trigramProfile)
{
    float accumulator = 0.0f;

    for (auto value : trigramProfile)
    {
        accumulator += value.second * value.second;
    }

    accumulator = sqrt(accumulator);

    for (auto &value : trigramProfile)
    {
        value.second /= accumulator;
    }

    return;
}

/*
 * Calculates the cosine similarity between a text trigram profile
 * and a language trigram profile.
 *
 * Parameters:
 *  textProfile -       Text trigram profile.
 *  languageProfile -   Language trigram profile.
 *
 * Returns: the cosine similarity score.
 */
float getCosineSimilarity(TrigramProfile &textProfile, TrigramProfile &languageProfile)
{
    float accumulator = 0.0f;
    for (auto textTrigram : textProfile)
    {
        auto iterator = languageProfile.find(textTrigram.first);

        if (iterator != languageProfile.end())
        {
            accumulator += iterator->second * textTrigram.second;
        }
    }

    return accumulator;
}

/*
 * Identifies the language of a text.
 *
 * Parameters:
 *  text -      A list of lines (Text).
 *
 * Returns: The language code of the most likely language.
 */
string identifyLanguage(const Text &text, Languages &languages)
{
    string languageMatched;
    float bestCosineSimilarity = 0.0f;

    for (auto language : languages)
    {
        TrigramProfile testTrigramProfile = buildTrigramProfile(text);

        float actualCosineSimilarity = getCosineSimilarity(testTrigramProfile,
                                                           language.trigramProfile);
        
        if(actualCosineSimilarity > bestCosineSimilarity)
        {
            bestCosineSimilarity = actualCosineSimilarity;
            languageMatched = language.languageCode;
        }
    }

    return languageMatched;
}
