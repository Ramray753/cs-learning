 #include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int isValidate(string s_key){
    char store[26];
    for (int i = 0; i < 26; i++){
        if(!isalpha(s_key[i]) || store[tolower(s_key[i]) - 97] != 0){
            return 0;
        }
        store[tolower(s_key[i]) - 97] = 1;
    }
    return 1;
}

int main(int argc, string argv[])
{
    if (argc != 2){
        printf("Usage: ./substitution key\n");
        return 1;
    }
    string s_key = argv[1];
    if (strlen(s_key) != 26 || !isValidate(s_key)){
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    string plain = get_string("plaintext:  ");
    printf("ciphertext: ");
    for (int i = 0, l = strlen(plain); i < l; i++){
        if (isalpha(plain[i])){
            if (islower(plain[i])){
                printf("%c", tolower(s_key[plain[i] - 97]));
            } else {
                printf("%c", toupper(s_key[plain[i] - 65]));
            }
        } else {
            printf("%c", plain[i]);
        }
    }
    printf("\n");

}