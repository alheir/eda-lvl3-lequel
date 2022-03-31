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

/**
 * @brief Criterio de comparación para ordenar lista CSV, con codigo y nombres de lenguaje
 * 
 * @param first Vector de strings con {codigo, nombre}
 * @param second Vector de strings con {codigo, nombre}
 * @return true, first va antes alfabeticamnete que second
 * @return false, first va despues alfabeticamente que second
 */
int compareAlphabetically(const vector<string> &first, const vector<string> &second)
{
    return (first[0].compare(second[0]) <= 0) ? true : false;
}

//TODO: ¿hacer que addLanguaje reciba nombre de archivo a agregar + nombre del idioma?

//TODO: El .csv tiene que quedar en TRIGRAMS_PATH, y con el nombre del idioma (cat).

//TODO: También, por cada idioma nuevo, hay que agregarlo al languagecode_names_es.csv ("cat","Catalán")
//TODO: ¿ordenarlo alfabéticamente? (actualmentel, el 'cat' quedó abajo)

//TODO: revisar si quedaron txts o csvs dando vueltas por el directorio del proyecto
//TODO: revisar funcionamiento y que agregue a languagecode_names_es.csv
void addLanguage(const Text &text, string &languageIdentifier)
{
    TrigramProfile trigramProfileHandler = buildTrigramProfile(text);

    CSVData csvDataHandler;

    for (auto profile : trigramProfileHandler)
    {
        csvDataHandler.push_front({profile.first, to_string((int)profile.second)});
    }

    string languageCode = languageIdentifier.substr(1,3);

    csvDataHandler.sort(compareTrigramByFreq);
    writeCSV(TRIGRAMS_PATH + languageCode + ".csv", csvDataHandler);

}

void  modeAddLanguage(std::__1::map<std::__1::string, std::__1::string> &languageCodeNames)
{
    string result = " ";

    while(!IsKeyPressed(KEY_Q))
    {
        if (IsFileDropped())
        {
            bool error = false;

            int count;
            char **droppedFiles = {0};
            droppedFiles = GetDroppedFiles(&count);

            Text text;
            getTextFromFile(droppedFiles[0], text);

            string languageIdentifier = text.front();
            text.pop_front();

            string languageCode = languageIdentifier.substr(0,5);
            string languageName = languageIdentifier.substr(6);
            auto iteratorCode = languageCodeNames.find(languageCode);
            
            if((languageIdentifier[0] == '\"') 
                && (languageIdentifier[4] == '\"') 
                && (languageIdentifier[5] == ',')
                && (languageIdentifier[6] == '\"')
                && (languageIdentifier.back() == '\"') )
            {
                if(iteratorCode == languageCodeNames.end())
                {
                    for (auto it = languageCodeNames.begin(); it != languageCodeNames.end(); ++it)
                    {
                        if (it->second == languageName)
                        {
                            result = "Ya existe ese idioma: " + languageName;
                            error = true;
                            break;
                        }
                    }
                }
                else
                {
                    result = "Ya existe un idioma con ese codigo: " + languageCode ;
                     error = true;
                }
            }
            else
            {
                result = "Formato de archivo incorrecto " + languageIdentifier;
                error = true;
            }
             
            if (!error)
            {
                addLanguage(text, languageIdentifier);
                result = "Se agrego" + languageName;
                languageCodeNames[languageCode] = languageName;
                
                CSVData csvDataHandler;
                for (auto profile : languageCodeNames)
                {
                    string firstMinusComas = profile.first;
                    string secondMinusComas = profile.second;
                    firstMinusComas.erase(std::remove(firstMinusComas.begin(),
                                         firstMinusComas.end(), '\"'), firstMinusComas.end());
                    secondMinusComas.erase(std::remove(secondMinusComas.begin(),
                                         secondMinusComas.end(), '\"'), secondMinusComas.end());                                         
                    csvDataHandler.push_front({firstMinusComas, secondMinusComas});
                }
                csvDataHandler.sort(compareAlphabetically);
                writeCSV(LANGUAGECODE_NAMES_FILE, csvDataHandler);
            }

            ClearDroppedFiles();
        }

        BeginDrawing();

        ClearBackground(BEIGE);

        DrawText("Lequel?", 80, 80, 128, BROWN);
        DrawText("Arrastra un archivo:", 80, 220, 24, BROWN);
        DrawText("La primera linea debe tener una identificacion del idioma", 80, 250, 24, BROWN);
        DrawText(" \"[codigo de idioma (3 caracteres)]\",\"[nombre del idioma]\" ", 80, 280, 24, BROWN);
        DrawText("Para salir del modo insertar idioma, presione Q", 80, 310, 24, BROWN);
        if(result != " ")
        {
            DrawText(result.data(), 80, 340, 24, BROWN);
        }
        EndDrawing();
    }
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

    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Lequel?");

    SetTargetFPS(60);

    string languageCodes [3] = {"---", "---", "---"};


    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_A))
        {
            modeAddLanguage(languageCodeNames);
        }
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
        DrawText("... o presione A para agregar un idioma.", 80, 250, 24, BROWN);

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
            int languageStringWidth = MeasureText(languageStrings[i].c_str(), 48/ (i + 1));
            DrawText(languageStrings[i].c_str(), (screenWidth - languageStringWidth) / 2,
                         315 + 140 * i / (i+1) , 48 / (i + 1), BROWN);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
