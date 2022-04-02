/**
 * @file LanguageManager_test.cpp
 * @author Albertina Galan, Alejandro Nahuel Heir
 * @brief Tests del administrador de lenguajes
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <string>

#include "Lequel.h"
#include "Text.h"
#include "LanguageManager.h"

using namespace std;

void print(string s)
{
    cout << s;
}

int fail()
{
    cout << "FAIL" << endl;
    return 1;
}

int pass()
{
    cout << "PASS" << endl;
    return 0;
}

int main()
{
    Text text;

    TrigramProfile profile1;
    TrigramProfile profile2;
    vector<string> keys1, keys2;
    vector<float> values1, values2;

    profile1["aaa"] = 10.0f;
    profile2["aaa"] = 5.0f;
    profile1["bbb"] = 20.0f;
    profile2["bbb"] = 50.0f;
    profile1["ccc"] = 10.0f;
    profile2["ddd"] = 10.0f;
    for (auto value : profile1)
    {
        keys1.push_back(value.first);
        values1.push_back(value.second);
    }
    for (auto value : profile2)
    {
        keys2.push_back(value.first);
        values2.push_back(value.second);
    }

    print("compareTrigramByFreq() works when first freq is greater than second freq... ");
    if (!compareTrigramByFreq({keys1[0], to_string(values1[0])}, {keys2[0], to_string(values2[0])}))
        return fail();
    pass();

    print("compareTrigramByFreq() works when second freq is greater than first freq... ");
    if (compareTrigramByFreq({keys1[1], to_string(values1[1])}, {keys2[1], to_string(values2[1])}))
        return fail();
    pass();

    print("compareTrigramByFreq() works when both first and second freq are the same... ");
    if (compareTrigramByFreq({keys1[1], to_string(values1[1])}, {keys2[1], to_string(values2[1])}))
        return fail();
    pass();

    map<string, string> languageCodeNames;
    languageCodeNames["aaa"] = "abcde";
    string languageIdentifier1 = "aaa,abcde";
    string languageIdentifier2 = "aaa,abcdd";
    string languageIdentifier3 = "bbb,abcde";
    string languageIdentifier4 = "bbb,abcdd";
    string code, name;
    string result;

    printf("verifyNewLanguage() works when new language does not exist... ");
    if (!verifyNewLanguage(languageCodeNames, languageIdentifier4, code, name, result))
        return fail();
    pass();

    printf("verifyNewLanguage() gets language code correctly... ");
    if (code != "bbb")
        return fail();
    pass();

    printf("verifyNewLanguage() gets language name correctly... ");
    if (name != "abcdd")
        return fail();
    pass();

    printf("verifyNewLanguage() works when new language already exists... ");
    if (verifyNewLanguage(languageCodeNames, languageIdentifier1, code, name, result))
        return fail();
    pass();

    printf("verifyNewLanguage() works when only new language code exists... ");
    if (verifyNewLanguage(languageCodeNames, languageIdentifier2, code, name, result))
        return fail();
    pass();

    printf("verifyNewLanguage() works when only new language name exists... ");
    if (verifyNewLanguage(languageCodeNames, languageIdentifier3, code, name, result))
        return fail();
    pass();

    return 0;
}
