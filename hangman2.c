#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define ROUNDS 3
#define CATEGORIES 4
#define MISTAKES 7

#define CHOICE_YES 'Y'
#define CHOICE_NO 'N'

const char *city[] = 
{
    "LONDON", "PARIS", "BERLIN", "MADRID", "ROME", "SANTIAGO",
    "LISBON", "VIENNA", "ATHENS", "DUBLIN", "OSLO", "MONTEVIDEO",
    "STOCKHOLM", "COPENHAGEN", "HELSINKI", "BRUSSELS", "AMSTERDAM",
    "WARSAW", "PRAGUE", "BUDAPEST", "BUCHAREST", "BELGRADE",
    "SOFIA", "MOSCOW", "ISTANBUL", "DUBAI", "TOKYO", "JERUSALEM",
    "BEIJING", "BANGKOK", "SEOUL", "MUMBAI", "JAKARTA", "BEIRUT",
    "SINGAPORE", "SHANGHAI", "MANILA", "HANOI", "RIYADH",
    "TEHRAN", "CAIRO", "LAGOS", "NAIROBI", "CASABLANCA", "KIEV",
    "ALGIERS", "ACCRA", "DAKAR", "TUNIS", "KAMPALA", "ZURICH",
    "ZAGREB", "TALLIN", "VILNIUS", "BERN", "BRATISLAVA"
};

const char *country[] = 
{
    "FRANCE", "GERMANY", "SPAIN", "ITALY", "PORTUGAL",
    "AUSTRIA", "GREECE", "IRELAND", "NORWAY", "SWEDEN",
    "DENMARK", "FINLAND", "BELGIUM", "NETHERLANDS", "POLAND",
    "CZECHIA", "HUNGARY", "ROMANIA", "SERBIA", "BULGARIA",
    "RUSSIA", "TURKEY", "JAPAN", "CHINA", "URUGUAY",
    "THAILAND", "SOUTHKOREA", "INDIA", "INDONESIA", "SINGAPORE",
    "PHILIPPINES", "VIETNAM", "SAUDIARABIA", "IRAN", "EGYPT",
    "NIGERIA", "KENYA", "MOROCCO", "ALGERIA", "GHANA", "UKRAINE",
    "ETHIOPIA", "SENEGAL", "TUNISIA", "UGANDA", "CROATIA",
    "ESTONIA", "LITHUANIA", "SWITZERLAND", "SLOVAKIA"
};

const char *car_brand[] = 
{
    "TOYOTA", "VOLKSWAGEN", "MERCEDES", "BMW", "AUDI",
    "FORD", "CHEVROLET", "HONDA", "HYUNDAI", "NISSAN",
    "KIA", "PEUGEOT", "RENAULT", "FIAT", "SKODA",
    "MAZDA", "SUBARU", "TESLA", "VOLVO", "PORSCHE",
    "JAGUAR", "LANDROVER", "LEXUS", "MITSUBISHI", "SUZUKI",
    "CHERY", "DACIA", "FERRARI", "LAMBORGHINI", "BUGATTI",
    "MASERATI", "BENTLEY", "GELLY", "BYD", "MCLAREN",
    "CADILLAC", "CHRYSLER", "DODGE", "JEEP", "GMC",
    "LINCOLN", "ACURA", "INFINITI", "GENESIS",
    "SEAT", "CITROEN", "OPEL", "SAAB", "LANCIA"
};

const char *animal[] = 
{
    "LION", "TIGER", "ELEPHANT", "GIRAFFE", "ZEBRA",
    "CHEETAH", "LEOPARD", "PANDA", "KANGAROO", "KOALA",
    "DOLPHIN", "SHARK", "WHALE", "OCTOPUS", "CROCODILE",
    "ALLIGATOR", "EAGLE", "FALCON", "OWL", "PENGUIN",
    "PEACOCK", "PARROT", "RAVEN", "WOLF", "FOX",
    "BEAR", "DEER", "RABBIT", "SQUIRREL", "HEDGEHOG",
    "HORSE", "DONKEY", "CAMEL", "GOAT", "SHEEP",
    "BUFFALO", "BISON", "CHIMPANZEE", "GORILLA", "ORANGUTAN",
    "HIPPOPOTAMUS", "RHINOCEROS", "ARMADILLO", "SLOTH", "ANTEATER",
    "JAGUAR", "HYENA", "OTTER", "BEAVER", "MONGOOSE"
};

const char **dictionar[] = {city, country, car_brand, animal};
const char *numele_categoriei[CATEGORIES] = 
{
    "CITY", "COUNTRY", "CAR BRAND", "ANIMAL"
};


//fac un sizeof pentru ca sa imi calculeze numerul de cuvinte din toate dictionarele
const int LITERE = sizeof(dictionar)/ sizeof(dictionar[0]);

typedef struct 
{
    char word[100];
    char guessed[100];
    int length;
    int found;
    int categorie;
} Runda;

typedef struct
{
    Runda igra[ROUNDS];
    int categoria;
    int mistakes;
    int score;
    int Game_Over;
    int categoriile[ROUNDS];
} Game;


void Hangman_game(Game *jocul) 
{
    srand(time(0));
    
    printf("-------->ALEGE CATEGORIA<--------\n");
    //alegem categoria cu care vrem sa jocum, ori vrem sa ghicim vreo tara sau altceva
    for (int i = 0; i < ROUNDS; i++) 
    {
        (*jocul).categoriile[i] = rand() % CATEGORIES;

        //verficam se ne asiguram ca aceeleasi categorii nu se vor repeta in urmatoarele runde
        for (int j = 0; j < i; j++) 
        {
            if ((*jocul).categoriile[i] == (*jocul).categoriile[j]) 
            {
                i--;
                break;
            }
        }
    }


    //o sa aleg 3 cuvinte din dictionar (csunt defapt 4) pentru fiecare runda
    // sunt 3 runde, si atunci random va apare un cuvant pentru fiecare runda
    for (int i = 0; i < ROUNDS; i++) 
    {
        int category = (*jocul).categoriile[i];
        int word_count = 0;
        
        // numar cuvintele
        while (dictionar[category][word_count] != NULL)
        {
            word_count++;
        }
        
        int rec = rand() % word_count;
        strcpy((*jocul).igra[i].word, dictionar[category][rec]);
        (*jocul).igra[i].length = strlen(dictionar[category][rec]);
        (*jocul).igra[i].found = 0;
        (*jocul).igra[i].categorie = category;

        //si la cautare cuvantului litera va fi ascunsa dupa semnul "_", la fel dupa semnul acela jucatorul poate sa numere cate litere contine cuvantul
        for (int j = 0; j < (*jocul).igra[i].length; j++) 
        {
            (*jocul).igra[i].guessed[j] = '_';
        }
        (*jocul).igra[i].guessed[(*jocul).igra[i].length] = '\0';
    }
    
    (*jocul).categoria = 0;
    (*jocul).mistakes = 0;
    (*jocul).score = 0;
    (*jocul).Game_Over = 0;
};

void Hangman(int mistakes) 
{
    //Pe ce spanzura
    printf(" _______\n");
    printf(" |     |\n");
    
    //cap
    if (mistakes >= 1)
        printf(" |     O\n");
    else
        printf(" |\n");


    //trunchi
    if (mistakes >= 2)
        printf(" |     |\n");
    else
        printf(" |\n");


    //bratele
    if (mistakes == 3)
        printf(" |     |\n");
    else if (mistakes == 4)
        printf(" |    /|\n");
    else if (mistakes >= 5)
        printf(" |    /|\\\n");
    else
        printf(" |\n");


    //picioarele
    if (mistakes == 6)
        printf(" |    /\n");
    else if (mistakes >= 7)
        printf(" |    / \\\n");
    else
        printf(" |\n");

    printf(" |\n");
    printf("_|_\n\n");
};

void Jocul_Design(Game *jocul) 
{
    system("cls");
    //Curatam ecranul ca sa nu fie totul adunat, apoi afisam noul desen al spanzuratului si literele ghicite pana acum. Dupa fiecare alegere, jocul arata ca nou"

    printf("------->  H A N G M A N  <--------\n");
    printf("-->  R U N D A: %d/%d\n", (*jocul).categoria + 1, ROUNDS);
    printf("-->  C A T E G O R I A: %s\n", numele_categoriei[(*jocul).categoriile[(*jocul).categoria]]);
    printf("-->  M I S T A K E S: %d/%d\n", (*jocul).mistakes, MISTAKES);
    printf("-->  S C O R U L: %d\n", (*jocul).score);
    Hangman((*jocul).mistakes);
    
    //aici afisam cuvantul care este in joc
    printf("C U V A N T U L  C O R E C T  E S T E: %s\n", (*jocul).igra[(*jocul).categoria].guessed);
};

int Ghiceste(Game *jocul, char caracter) 
{
    caracter = toupper(caracter);
    int found = 0;
    Runda *actual = &(*jocul).igra[(*jocul).categoria];


    //acuma fac o funtie care o sa verifice dar litera este in cuvant, unde se va afisa litera in loc de "_"
    for (int i = 0; i < (*actual).length; i++) 
    {
        if ((*actual).word[i] == caracter && (*actual).guessed[i] == '_') 
        {
            (*actual).guessed[i] = caracter;
            found = 1;
        }
    }

    if (!found) 
    {
        (*jocul).mistakes++;
        if ((*jocul).mistakes >= MISTAKES) 
        {
            (*jocul).Game_Over = 1;
        }
        return 0;
    }


    //acum verificam daca dupa ce am apasat o litera, litera se afla in acel cuvant, mai precis zis vedem daca cuvantul a fost ghicit sau nu
    if (strcmp((*actual).word, (*actual).guessed) == 0) 
    {
        (*jocul).score = (*jocul).score + ((MISTAKES - (*jocul).mistakes)) * 10;
        (*actual).found = 1;
        return 2;
    }
    return 1;
};

void Play_Again(Game *jocul) 
{
    char izbor;
    printf("\nP L A Y  A G A I N\n (Y/N): ");
    scanf(" %c", &izbor);
    izbor = toupper(izbor);

    if (izbor == CHOICE_YES) 
    {
        Hangman_game(jocul);
    } else 
    {
        exit(0);
    }
}

void HangmanAgain(Game *jocul) 
{
    while(1)
    {
        Jocul_Design(jocul);

        //Verfic daca e gata runda
        if ((*jocul).categoria >= ROUNDS) 
        {
            printf("\n--------> W I N N E R <--------\n");
            printf("Y O U R  S C O R E: %d\n", (*jocul).score);
            Play_Again(jocul);
            return;
        }

        if ((*jocul).Game_Over) 
        {
            Jocul_Design(jocul);
            printf("\n-------->  G A M E  O V E R  <--------\n");
            printf("\n-------->  Y O U  L O S T  <--------\n");
            printf("THE CORRECT WORD WAS: %s\n", (*jocul).igra[(*jocul).categoria].word);
            Play_Again(jocul);
            return;
        }

        printf("Enter your guess: ");
        char guess;
        scanf(" %c", &guess);
        while (getchar() != '\n'); 

        int result = Ghiceste(jocul, guess);
        
        if (result == 2) 
        {
            Jocul_Design(jocul);
            printf("\n ==  W E L L  D O N E  ==\n");
            printf("\n ==  B E G G I N E R S  L U C K ==\n");

            if ((*jocul).categoria == ROUNDS - 1) 
            {
                printf("\n -------->  W I N N E R  <--------\n");
                printf("Y O U R  F I N A L  S C O R E : %d \n", (*jocul).score);
                Play_Again(jocul);
                return;
            } else 
            {
                printf("--->  P R E S S  E N T E R  T O  C O N T I N U E\n");
                while (getchar() != '\n');
                (*jocul).categoria++;
                (*jocul).mistakes = 0;
            }
        }
    }
};




int main() 
{
    Game jocul;
    Hangman_game(&jocul);
    HangmanAgain(&jocul);
    return 0;
};