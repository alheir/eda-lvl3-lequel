/**
 * @file main.cpp
 * @author Marc S. Ressl, Albertina Galan, Alejandro Nahuel Heir
 * @brief Language identification based on trigrams.
 * @version 0.1
 * @date 2022-03-29
 *
 * @copyright Copyright (c) 2022
 *
 * @note Se provee en resources/corpus una lista de textos de idiomas para agregar.
 *       El idioma Catalán ya fue incorporado. Referirse al README de esa carpeta y al
 *       del directorio principal.
 */

#include <iostream>
#include <map>

#include "raylib.h"
#include "LanguageManager.h"

using namespace std;

enum OPERATION_MODES
{
    DETECTION,
    ADD_LANGUAGE
};

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

    string languageCodes[3] = {"---", "---", "---"};

    int lequelMode = DETECTION;
    string operationResult;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_D))
            lequelMode = DETECTION;
        else if (IsKeyPressed(KEY_A))
            lequelMode = ADD_LANGUAGE;

        switch (lequelMode)
        {
        case DETECTION:
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

            DrawText("Lequel?",
                     80, 80, 128, BROWN);
            DrawText("Copia y pega con Ctrl+V, o arrastra un archivo...",
                     80, 220, 24, BROWN);
            DrawText("... o presione A para agregar un idioma.",
                     80, 250, 24, BROWN);

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
                int languageStringWidth = MeasureText(languageStrings[i].c_str(), 48 / (i + 1));

                DrawText(languageStrings[i].c_str(), (screenWidth - languageStringWidth) / 2,
                         315 + 140 * i / (i + 1), 48 / (i + 1), BROWN);
            }
            EndDrawing();

            break;
        }

        case ADD_LANGUAGE:
        {
            if (IsFileDropped())
            {
                int count;
                char **droppedFiles = {0};
                droppedFiles = GetDroppedFiles(&count);

                Text text;
                getTextFromFile(droppedFiles[0], text);

                ClearDroppedFiles();

                string languageIdentifier = text.front();
                text.pop_front();

                string languageName, languageCode;

                if (verifyNewLanguage(languageCodeNames, languageIdentifier, languageCode,
                                      languageName, operationResult))
                {
                    addLanguage(text, languageCodeNames, languages, languageCode, languageName,
                                operationResult);
                }
            }

            BeginDrawing();

            ClearBackground(BEIGE);

            DrawText("Lequel?",
                     80, 80, 128, BROWN);
            DrawText("Arrastra un corpus en algún idioma para agregarlo:",
                     80, 220, 24, BROWN);
            DrawText("La primera linea debe tener una identificacion del idioma:",
                     80, 250, 24, BROWN);
            DrawText("[código del idioma (3caracteres)],[nombre del idioma]",
                     80, 280, 20, RED);
            DrawText("Para volver al modo de detección, presione D",
                     80, 310, 24, BROWN);

            DrawText(operationResult.data(),
                     80, 340, 24, BROWN);

            EndDrawing();

            break;
        }

        default:

            break;
        }
    }

    CloseWindow();

    return 0;
}
