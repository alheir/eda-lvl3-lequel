/**
 * @file LanguageManager.h
 * @author Marc S. Ressl, Albertina Galan, Alejandro Nahuel Heir
 * @brief Administrador de lenguajes, para agregarlos y detectar existentes.
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

/*
 * Loads trigram data.
 *
 * Parameters:
 *  languageCodeNames -     Map of language code vs. language name (in user's locale).
 *  trigramProfiles -       The trigram profiles.
 *
 * Returns: whether the function succeeded.
 */
bool loadLanguagesData(std::map<std::string, std::string> &languageCodeNames,
                       Languages &languages);

/**
 * @brief
 *
 * @param text
 * @param languageCodeNames
 * @param languages
 * @param languageCode
 * @param languageName
 * @param result
 */
void addLanguage(const Text &text, std::map<std::string, std::string> &languageCodeNames,
                 Languages &languages, const std::string &languageCode,
                 const std::string &languageName, std::string &result);

/**
 * @brief
 *
 * @param languageCodeNames
 * @param languageIdentifier
 * @param languageCode
 * @param languageName
 * @param output
 * @return true Posible agregarlo
 * @return false Imposible agregarlo
 */
bool verifyNewLanguage(const std::map<std::string, std::string> &languageCodeNames,
                       const std::string &languageIdentifier, std::string &languageCode,
                       std::string &languageName, std::string &output);

/**
 * @brief Criterio de comparación para ordenar lista CSV, con trigramas y frecuencias
 *
 * @param first Vector de strings con {trigama, frecuencia}
 * @param second Vector de strings con {trigama, frecuencia}
 * @return true, first tiene mayor frecuencia que second
 * @return false, first tiene menor frecuencia que second
 */
bool compareTrigramByFreq(const std::vector<std::string> &first,
                          const std::vector<std::string> &second);
