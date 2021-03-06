/**
 * @file LanguageManager.cpp
 * @author Marc S. Ressl, Albertina Galan, Alejandro Nahuel Heir
 * @brief Administrador de lenguajes, para agregarlos y detectar existentes.
 * @version 0.1
 * @date 2022-03-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <iostream>

#include "LanguageManager.h"

using namespace std;

const string LANGUAGECODE_NAMES_FILE = "../resources/languagecode_names_es.csv";
const string TRIGRAMS_PATH = "../resources/trigrams/";

/**
 * @brief Loads trigram data.
 *
 * @param text: the corpus of the language to add
 * @param languageCodeNames: Map of language code vs. language name (in user's locale).
 * @param trigramProfiles: The trigram profiles.
 * @return: whether the function succeeded.
 */
bool loadLanguagesData(map<string, string> &languageCodeNames, Languages &languages)
{
    // Reads available language codes
    cout << "Reading language codes..." << endl;

    CSVData languageCodesCSVData;
    if (!readCSV(LANGUAGECODE_NAMES_FILE, languageCodesCSVData))
        return false;

    // Reads trigram profile for each language code
    for (auto fields : languageCodesCSVData)
    {
        if (fields.size() != 2)
            continue;

        string languageCode = fields[0];
        string languageName = fields[1];

        languageCodeNames[languageCode] = languageName;

        cout << "Reading trigram profile for language code \"" << languageCode << "\"..." << endl;

        CSVData languageCSVData;
        if (!readCSV(TRIGRAMS_PATH + languageCode + ".csv", languageCSVData))
            return false;

        languages.push_back(Language());
        Language &language = languages.back();

        language.languageCode = languageCode;

        for (auto fields : languageCSVData)
        {
            if (fields.size() != 2)
                continue;

            string trigram = fields[0];
            float frequency = (float)stoi(fields[1]);

            language.trigramProfile[trigram] = frequency;
        }

        normalizeTrigramProfile(language.trigramProfile);
    }

    return true;
}

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
void addLanguage(const Text &text, map<string, string> &languageCodeNames,
                 Languages &languages, const string &languageCode,
                 const string &languageName, string &result)
{
    // Se crea el trigramProfile del texto nuevo, y se escribe su .csv
    TrigramProfile trigramProfileHandler = buildTrigramProfile(text);

    CSVData csvDataHandler;
    for (auto profile : trigramProfileHandler)
    {
        csvDataHandler.push_front({profile.first, to_string((int)profile.second)});
    }

    csvDataHandler.sort(compareTrigramByFreq);
    writeCSV(TRIGRAMS_PATH + languageCode + ".csv", csvDataHandler);

    // Se agrega al mapa
    languageCodeNames[languageCode] = languageName;

    // Se escribe el nuevo csv con el nuevo idoma, orden??ndolo alfab??ticamente
    csvDataHandler.clear();
    for (auto profile : languageCodeNames)
    {
        csvDataHandler.push_front({profile.first, profile.second});
    }

    csvDataHandler.sort();
    writeCSV(LANGUAGECODE_NAMES_FILE, csvDataHandler);

    // Se actualiza la lista de idiomas que usa main
    Language newLanguage;
    newLanguage.languageCode = languageCode;
    newLanguage.trigramProfile = trigramProfileHandler;
    languages.push_back(newLanguage);

    result = "Se agreg??: " + languageName;
}

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
bool verifyNewLanguage(const map<string, string> &languageCodeNames,
                       const string &languageIdentifier, string &languageCode,
                       string &languageName, string &output)
{
    bool error = false;

    if (languageIdentifier[3] == ',')
    {

        languageCode = languageIdentifier.substr(0, 3);
        auto iteratorCode = languageCodeNames.find(languageCode);

        if (iteratorCode == languageCodeNames.end())
        {
            languageName = languageIdentifier.substr(4);

            for (auto it = languageCodeNames.begin(); it != languageCodeNames.end(); ++it)
            {
                if (it->second == languageName)
                {
                    output = "Ya existe ese idioma: " + languageName;
                    error = true;
                    break;
                }
            }
        }
        else
        {
            output = "Ya existe un idioma con ese codigo: " + languageCode;
            error = true;
        }
    }
    else
    {
        output = "Formato de archivo incorrecto: " + languageIdentifier;
        error = true;
    }
    return !error;
}

/**
 * @brief Criteria of comparison to order the CSV list, with trigramas and frecuency
 *
 * @param first Vector of strings with {trigam, frecuency}
 * @param second Vector of strings with {trigam, frecuency}
 * @return true, first has a higher frequency than second
 * @return false, first has a lower frequency than second
 */
bool compareTrigramByFreq(const vector<string> &first, const vector<string> &second)
{
    return stof(first[1]) > stof(second[1]);
}
