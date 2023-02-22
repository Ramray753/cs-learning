 #include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


int only_digits(string s_key){
    int ans = 0;
    int multiplier = 1;
    int l = strlen(s_key);
    for (int i = l - 1; i >= 0; i--){
        if (! isdigit(s_key[i])){
            return 0;
        }
        ans += multiplier * (s_key[i] - 48);
        multiplier *= 10;
    }
    return ans;
}

int main(int argc, string argv[])
{
    if (argc != 2){
        printf("Usage: ./caesar key\n");
        return 1;
    }
    string s_key = argv[1];
    int key = only_digits(s_key);
    if (! key){
        printf("Usage: ./caesar key\n");
        return 1;
    }
    string plain = get_string("plaintext:  ");
    printf("ciphertext: ");
    for (int i = 0, l = strlen(plain); i < l; i++){
        if (isalpha(plain[i])){
            if (islower(plain[i])){
                printf("%c", (plain[i] - 96 + key) % 26 + 96);
            } else {
                printf("%c", (plain[i] - 64 + key) % 26 + 64);
            }
        } else {
            printf("%c", plain[i]);
        }
    }
    printf("\n");

}