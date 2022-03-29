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
    wstring_convert<std::codecvt_utf8_utf16<wchar_t> > converter;

    // Your code goes here...

    // Tip: converts UTF-8 string to wstring
    // wstring unicodeString = converter.from_bytes(textLine);

    // Tip: convert wstring to UTF-8 string
    // string trigram = converter.to_bytes(unicodeTrigram);

    return TrigramProfile(); // Replace...
}

/*
 * Normalizes a trigram profile.
 *
 * Parameters:
 *  trigramProfile -    Trigram profile.
 */
void normalizeTrigramProfile(TrigramProfile &trigramProfile)
{
    // Your code goes here...

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
    // Your code goes here...

    return 0; // Replace...
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
    // Your code goes here...

    return ""; // Replace...
}
