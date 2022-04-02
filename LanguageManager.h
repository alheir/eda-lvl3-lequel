/**
 * @file LanguageManager.h
 * @author Marc S. Ressl, Albertina Galan, Alejandro Nahuel Heir
 * @brief Administrator of languajes, ta add and detect existence.
 * @version 0.1
 * @date 2022-03-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <map>
#include <vector>

#include "Lequel.h"
#include "Text.h"
#include "CSVData.h"

/**
 * @brief Loads trigram data.
 *
 * @param text: the corpus of the language to add
 * @param languageCodeNames: Map of language code vs. language name (in user's locale).
 * @param trigramProfiles: The trigram profiles.
 * @return: whether the function succeeded.
 */
bool loadLanguagesData(std::map<std::string, std::string> &languageCodeNames,
                       Languages &languages);

/**
 * @brief Allows the addition of a new language to the list of languages the program interprets
 *
 * @param text: the corpus of the language to add
 * @param languageCodeNames: the list of all current languages
 * @param languages: the trigram profiles of all languages
 * @param languageCode: the code for the new language
 * @param languageName: the name of the new language
 * @param result: whether the language was added or there was an error
 */
void addLanguage(const Text &text, std::map<std::string, std::string> &languageCodeNames,
                 Languages &languages, const std::string &languageCode,
                 const std::string &languageName, std::string &result);

/**
 * @brief Verifies if the language to add already exists or if there is an error in text format
 *
 * @param languageCodeNames: the list of all current languages
 * @param languageIdentifier: the code and name of the new language. Should be first line of text
 * @param languageCode: the code for the new language
 * @param languageName: the name of the new language
 * @param output: the string with the result: if its able to add the language or not
 * @return true Posible agregarlo
 * @return false Imposible agregarlo
 */
bool verifyNewLanguage(const std::map<std::string, std::string> &languageCodeNames,
                       const std::string &languageIdentifier, std::string &languageCode,
                       std::string &languageName, std::string &output);

/**
 * @brief Criteria of comparison to order the CSV list, with trigramas and frecuency
 *
 * @param first Vector of strings with {trigam, frecuency}
 * @param second Vector of strings with {trigam, frecuency}
 * @return true, first has a higher frequency than second
 * @return false, first has a lower frequency than second
 */
bool compareTrigramByFreq(const std::vector<std::string> &first,
                          const std::vector<std::string> &second);
