#include <cs50.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>

int count_letters(char* text){
    int ans = 0;
    for(int i = 0, l = strlen(text); i < l; i++){
        if (isalpha(text[i])){
            ans++;
        }
    }
    return ans;
}

int count_words(char* text){
    int ans = 1;
    for(int i = 0, l = strlen(text); i < l; i++){
        if (isspace(text[i])){
            ans++;
        }
    }
    return ans;
}

int count_sentences(char* text){
    int ans = 0;
    for(int i = 0, l = strlen(text); i < l; i++){
        if (text[i] == '.' || text[i] == '!' || text[i] == '?'){
            ans++;
        }
    }
    return ans;
}

int main(void)
{
    char* text = get_string("Text: ");
    int letter_c = count_letters(text);
    int word_c = count_words(text);
    int sen_c = count_sentences(text);
    float result = 0.0588 * letter_c / word_c * 100  - 0.296 * sen_c / word_c * 100 - 15.8;
    int grade = (int) round(result);
    if (grade <= 0){
        printf("Before Grade 1\n");
    }
    else if (grade >= 16){
        printf("Grade 16+\n");
    }
    else {
        printf("Grade %i\n", grade);
    }
}