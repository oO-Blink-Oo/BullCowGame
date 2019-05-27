#pragma once

#include "FBullCowGame.h"
#include <map>

// to make syntax Unreal friendly
#define Tmap std::map

FBullCowGame::FBullCowGame() { Reset(); } // default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const
{
    Tmap<int32, int32> WordLengthToMaxTries{{3, 4}, {4, 7}, {5, 10}, {6, 15}, {7, 20}};
    return WordLengthToMaxTries[MyHiddenWord.length()];
}

bool FBullCowGame::IsIsogram(FString Word) const
{
    // treat 0 and 1 letter words as isograms
    if (Word.length() <= 1)
    {
        return true;
    }

    Tmap<char, bool> LetterSeen; // setup our map
    for (auto Letter : Word)     // for all letters of the word
    {
        Letter = tolower(Letter); // handle mixed case
        if (LetterSeen[Letter])   // if this is true meaning this was seen already
        {
            return false; // we do NOT have an isogram
        }
        else
        {
            LetterSeen[Letter] = true;
        }
    }

    return true; // for example in cases where /0 is entered
}

bool FBullCowGame::IsLowercase(FString Word) const
{
    for (auto Letter : Word)
    {
        if (!islower(Letter))
        {
            return false; // everything was not in lowercase
        }
    }
    return true;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
    if (!IsIsogram(Guess)) //if the guess isn't an isogram
    {
        return EGuessStatus::Not_Isogram;
    }
    else if (!IsLowercase(Guess)) // if the guess isn't all lowercase
    {
        return EGuessStatus::Not_Lowercase;
    }
    else if (GetHiddenWordLength() != Guess.length()) // if the guess length is wrong
    {
        return EGuessStatus::Wrong_Length;
    }
    else
    {
        return EGuessStatus::OK;
    }
}

void FBullCowGame::Reset()
{
    const FString HIDDEN_WORD = "ant"; // this MUST be an isogram
    MyHiddenWord = HIDDEN_WORD;
    MyCurrentTry = 1;
    bGameIsWon = false;

    return;
}

//recieves a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
    MyCurrentTry++;
    FBullCowCount BullCowCount;
    int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

    // loop through all letters in the hidden word
    for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
    {
        // compare letters against the guess
        for (int32 GChar = 0; GChar < WordLength; GChar++)
        {
            // if they match then
            if (MyHiddenWord[MHWChar] == Guess[GChar])
            {
                if (MHWChar == GChar)
                {
                    BullCowCount.Bulls++;
                }
                else
                {
                    BullCowCount.Cows++;
                }
            }
        }
    }
    if (BullCowCount.Bulls == WordLength)
    {
        bGameIsWon = true;
    }
    else
    {
        bGameIsWon = false;
    }

    return BullCowCount;
}