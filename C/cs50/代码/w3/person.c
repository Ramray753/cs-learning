#include <stdio.h>
#include <cs50.h>
#include <string.h>

typedef struct{
  string name;
  int number;
}
person;

void printPerson(person p){
    printf("Name: %s, ID: %d\n", p.name, p.number);
}

int main(void){
    person people[2];
    people[0].name = "Ray";
    people[0].number = 12345;
    people[1].name = "Allen";
    people[1].number = 54321;
    person other = {"James", 98765};
    printPerson(people[0]);
    printPerson(people[1]);
    printPerson(other);
}
