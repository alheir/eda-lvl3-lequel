/*
 * Lequel?
 *
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Language identification based on trigrams.
 */

#include <iostream>
#include <map>
#include <string>
#include <algorithm>

#include "raylib.h"

#include "CSVData.h"
#include "Lequel.h"

using namespace std;

const string LANGUAGECODE_NAMES_FILE = "resources/languagecode_names_es.csv";
const string TRIGRAMS_PATH = "resources/trigrams/";

/**
 * @brief Criterio de comparación para ordenar lista CSV, con trigramas y frecuencias
 * 
 * @param first Vector de strings con {trigama, frecuencia}
 * @param second Vector de strings con {trigama, frecuencia}
 * @return true, first tiene mayor frecuencia que second
 * @return false, first tiene menor frecuencia que second
 */
bool compareTrigramByFreq(const vector<string> &first, const vector<string> &second)
{
    return stof(first[1]) > stof(second[1]);
}

//TODO: ¿hacer que addLanguaje reciba nombre de archivo a agregar + nombre del idioma?

//TODO: El .csv tiene que quedar en TRIGRAMS_PATH, y con el nombre del idioma (cat).

//TODO: También, por cada idioma nuevo, hay que agregarlo al languagecode_names_es.csv ("cat","Catalán")
//TODO: ¿ordenarlo alfabéticamente? (actualmentel, el 'cat' quedó abajo)

//TODO: revisar si quedaron txts o csvs dando vueltas por el directorio del proyecto
void addLanguage()
{
    Text textHandler;
    getTextFromFile("resources/sampleLanguageText.txt", textHandler);

    TrigramProfile trigramProfileHandler = buildTrigramProfile(textHandler);

    CSVData csvDataHandler;

    for (auto profile : trigramProfileHandler)
    {
        csvDataHandler.push_front({profile.first, to_string((int)profile.second)});
    }

    csvDataHandler.sort(compareTrigramByFreq);
    writeCSV(TRIGRAMS_PATH + "sampleLanguageCSV.csv", csvDataHandler);

}

/*
 * Loads trigram data.
 *
 * Parameters:
 *  languageCodeNames -     Map of language code vs. language name (in user's locale).
 *  trigramProfiles -       The trigram profiles.
 *
 * Returns: whether the function succeeded.
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

//TODO: optimizar la funcion que devuelve los 3 idiomas mas probables
int main(int, char *[])
{
    map<string, string> languageCodeNames;
    Languages languages;

    if (!loadLanguagesData(languageCodeNames, languages))
    {
        cout << "Could not load trigram data." << endl;
        return 1;
    }

    //TODO: ver cómo implementar el llamado a addLanguage. ¿
    //addLanguage();
    //return 0;

    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Lequel?");

    SetTargetFPS(60);

    string languageCodes [3] = {"---", "---", "---"};

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_V) &&
            (IsKeyDown(KEY_LEFT_CONTROL) ||
             IsKeyDown(KEY_RIGHT_CONTROL) ||
             IsKeyDown(KEY_LEFT_SUPER) ||
             IsKeyDown(KEY_RIGHT_SUPER)))
        {
            const char *clipboard = GetClipboardText();

            Text text;
            getText(clipboard, text);

            identifyLanguage(text, languages, languageCodes);
        }

        if (IsFileDropped())
        {
            int count;
            char **droppedFiles = {0};
            droppedFiles = GetDroppedFiles(&count);

            Text text;
            getTextFromFile(droppedFiles[0], text);

            identifyLanguage(text, languages, languageCodes);

            ClearDroppedFiles();
        }

        BeginDrawing();

        ClearBackground(BEIGE);

        DrawText("Lequel?", 80, 80, 128, BROWN);
        DrawText("Copia y pega con Ctrl+V, o arrastra un archivo...", 80, 220, 24, BROWN);

        string languageStrings[3];
        for (int i = 0; i < 3; i++)
        {
            if (languageCodes[i] != "---")
            {
                if (languageCodeNames.find(languageCodes[i]) != languageCodeNames.end())
                    languageStrings[i] = languageCodeNames[languageCodes[i]];
                else
                    languageStrings[i] = "Desconocido";
            }
        }
        
        for (int i = 0; i < 3; i++)
        {
            int languageStringWidth = MeasureText(languageStrings[0].c_str(), 48);
            DrawText(languageStrings[i].c_str(), (screenWidth - languageStringWidth) / 2,
                         315 + 125 * i / (i+1) , 48 / (i + 1), BROWN);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
