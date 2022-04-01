/**
 * @file Lequel.cpp
 * @author Marc S. Ressl, Albertina Galan, Alejandro Nahuel Heir
 * @brief Language identification based on trigrams.
 * @version 0.1
 * @date 2022-03-29
 *
 * @copyright Copyright (c) 2022
 *
 * More info:
 * https://towardsdatascience.com/understanding-cosine-similarity-and-its-application-fd42f585296a
 *
 */

#include <cmath>
#include <codecvt>
#include <locale>
#include <iostream>
#include <algorithm>

#include "Lequel.h"

using namespace std;

/**
 * @brief Builds a trigram profile for a given text.
 *
 * @param text A list of lines (Text).
 * @return TrigramProfile
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

/**
 * @brief Normalizes a trigram profile.
 *
 * @param trigramProfile
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

/**
 * @brief Calculates the cosine similarity between a text trigram profile
 * and a language trigram profile.
 *
 * @param textProfile Text trigram profile.
 * @param languageProfile Language trigram profile.
 * @return float, the cosine similarity score.
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

/**
 * @brief Identifies the language of a text.
 *
 * @param text A list of lines (Text).
 * @param languages
 * @return string, the language code of the most likely language.
 */
void identifyLanguage(const Text &text, Languages &languages, string *languagesMatched)
{
    float bestCosineSimilarity = 0.0f;
    float bestCosineSimilarities[3] = {0.0f, 0.0f, 0.0f};

    for (auto language : languages)
    {
        TrigramProfile testTrigramProfile = buildTrigramProfile(text);

        float actualCosineSimilarity = getCosineSimilarity(testTrigramProfile,
                                                           language.trigramProfile);

        if (actualCosineSimilarity > bestCosineSimilarities[0])
        {
            bestCosineSimilarities[2] = bestCosineSimilarities[1];
            languagesMatched[2] = languagesMatched[1];
            bestCosineSimilarities[1] = bestCosineSimilarities[0];
            languagesMatched[1] = languagesMatched[0];
            bestCosineSimilarities[0] = actualCosineSimilarity;
            languagesMatched[0] = language.languageCode;
        }
        else if (actualCosineSimilarity >= bestCosineSimilarities[1])
        {
            bestCosineSimilarities[2] = bestCosineSimilarities[1];
            languagesMatched[2] = languagesMatched[1];
            bestCosineSimilarities[1] = actualCosineSimilarity;
            languagesMatched[1] = language.languageCode;
        }
        else if (actualCosineSimilarity >= bestCosineSimilarities[2])
        {
            bestCosineSimilarities[2] = actualCosineSimilarity;
            languagesMatched[2] = language.languageCode;
        }
    }
}
