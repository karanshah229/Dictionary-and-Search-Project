#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 15
#define MAX_MEANING_LEN 150
#define MAX_SYNONYMS 2
#define HASH_TABLE_SIZE 31

//Stores the word per node in dictionary
struct word
{
    char word[MAX_WORD_LEN];
    char meaning[MAX_MEANING_LEN];
    struct word *synonyms[MAX_SYNONYMS];
};

//Stores pointers to previously searched words
struct stack
{
    struct word *prev_words[HASH_TABLE_SIZE];
    int top;
};

//Calculates hash key
int hash_index(char word[MAX_WORD_LEN]);

//Creates new word node and assigns word and its meaning
struct word *new_word(char word[MAX_WORD_LEN], char meaning[MAX_MEANING_LEN]);

//Insertion into hash table using linear probing
void insert(struct word *HT[HASH_TABLE_SIZE], char word[MAX_WORD_LEN], char meaning[MAX_MEANING_LEN]);

//Searches hash table for word using string input
struct word *search(struct word *HT[HASH_TABLE_SIZE], char word[MAX_WORD_LEN]);

//Links synonyms to pointer as a pointer to another word node
void add_synonym(struct word *HT[HASH_TABLE_SIZE], char word[MAX_WORD_LEN], char synonym1[MAX_WORD_LEN], char synonym2[MAX_WORD_LEN]);

//Displays the searched word, meaning and synonyms
void print(struct word *);

//Helper function to transform String to upper case
void toUpperCase(char s[]);

//Initialize the dictionary with default words
void init(struct word *HT[HASH_TABLE_SIZE]);

//Initialize the dictionary with words from a file
void init_file(struct word *HT[HASH_TABLE_SIZE], char file[MAX_MEANING_LEN]);

//Main function
int main(int argc, char **argv)
{
    struct word *HT[HASH_TABLE_SIZE];
    struct stack *stack = (struct stack *)malloc(sizeof(struct stack));
    stack->top = -1;
    int i, choice;
    char input[MAX_WORD_LEN];

    for (i = 0; i < HASH_TABLE_SIZE; i++)
        HT[i] = NULL;

    if(argc < 2)
		printf("Usage: %s <file1>..<file2>...<file> <keyword>\n", argv[0]);
	else
		for(int i = 1; i < argc; i++)
			init_file(HT, argv[i]);

    if(argc < 2) init(HT);

    while (1){
    	puts("");
        puts("1. Search");
        puts("2. Previous words");
        puts("3. Exit");
        puts("Enter choice code: ");
        scanf("%d", &choice);

        switch (choice){
		    case 1: printf("Enter a word to search: \n");
					scanf("%s", input);
					struct word *word = search(HT, input);
					if (word != NULL) {
						stack->top++;
						stack->prev_words[stack->top] = word;
						print(word);
					} else {
						printf("Word does not exist in this dictionary\n");
						stack->top++;
						struct word *other_word = new_word(input, "");
						stack->prev_words[stack->top] = other_word;
					}
					break;
		    case 2:	printf("Previously searched words: \n");
                    int temp = stack->top;
				    while (stack->top != -1){
				        printf("%s\n", stack->prev_words[stack->top]->word);
				        stack->top--;
				    }
				    stack->top = temp;
			        break;
		    case 3: exit(0);
        }
    }

    return 0;
}

void insert(struct word *HT[HASH_TABLE_SIZE], char word[MAX_WORD_LEN], char meaning[MAX_MEANING_LEN])
{
    int hash_index_t = hash_index(word);

    while (HT[hash_index_t] != NULL)
        hash_index_t = (hash_index_t + 1) % HASH_TABLE_SIZE;

    HT[hash_index_t] = new_word(word, meaning);
}

struct word *search(struct word *HT[HASH_TABLE_SIZE], char word[MAX_WORD_LEN])
{
    int hash_index_t = hash_index(word);

    while (HT[hash_index_t] != NULL && strcmp(HT[hash_index_t]->word, word))
        hash_index_t = (hash_index_t + 1) % HASH_TABLE_SIZE;

    if (HT[hash_index_t] == NULL)
        return NULL;
    else
        return HT[hash_index_t];
}

void add_synonym(struct word *HT[HASH_TABLE_SIZE], char word[MAX_WORD_LEN], char synonym1[MAX_WORD_LEN], char synonym2[MAX_WORD_LEN])
{
    struct word *word_t = search(HT, word);
    struct word *synonym1_location = search(HT, synonym1);
    struct word *synonym2_location = search(HT, synonym2);
    word_t->synonyms[0] = synonym1_location;
    word_t->synonyms[1] = synonym2_location;
}

int hash_index(char word[MAX_WORD_LEN])
{
    int hash_index = 0, i;
    for (i = 0; i < strlen(word); i++)
        hash_index += (int)word[i];

    hash_index = hash_index % HASH_TABLE_SIZE;

    return hash_index;
}

void print(struct word *word)
{
    printf("%s", word->word);
    if(word->meaning != '\0')
        printf(" : %s\n", word->meaning);
	if(word->synonyms[0] != '\0')
		printf("Synonyms: %s | %s\n", word->synonyms[0]->word, word->synonyms[1]->word);
	else
		puts("No Synonyms");
}

struct word *new_word(char word[MAX_WORD_LEN], char meaning[MAX_MEANING_LEN])
{
    struct word *new_word = (struct word *)malloc(sizeof(struct word));

    strcpy(new_word->word, word);
    strcpy(new_word->meaning, meaning);
    new_word->synonyms[0] = NULL;

    return new_word;
}

void init(struct word *HT[HASH_TABLE_SIZE]){
	//Keep adding words here
	insert(HT, "mad", "Mentally disturbed. Deranged or insane");
	insert(HT, "absurd", "Utterly or obviously senseless, illogical or untrue. Laughably foolish or false");
	insert(HT, "academy", "A secondary or high school");
	insert(HT, "university", "An institution of learning of the highest level");
	insert(HT, "institute", "A society or organization for carrying on a particular work");
	insert(HT, "crazy", "Senseless, Impractical, totally unsound");
	insert(HT, "editor", "a person who is in charge of and determines the final content of a newspaper, magazine, or multi-author book");

	//Add synonyms like this: word, it's 1st synonym,
	//then 2nd synonym
	add_synonym(HT, "mad", "absurd", "crazy");
	add_synonym(HT, "absurd", "mad", "crazy");
	add_synonym(HT, "crazy", "absurd", "mad");
	add_synonym(HT, "academy", "institute", "university");
	add_synonym(HT, "institute", "academy", "university");
	add_synonym(HT, "university", "institute", "academy");

}

void init_file(struct word *HT[HASH_TABLE_SIZE], char filename[MAX_MEANING_LEN]){
	char path[] = "H:\\Coding\\C\\DSA\\DSA\\Dictionary_Project\\bin\\Debug\\";
	strcat(path, filename);
	puts(path);

	FILE *fp = fopen(path, "r");
	char word[MAX_MEANING_LEN];

	if (fp == NULL){
		printf("Error file missing\n");
		exit(-1);
	}
	while(!feof(fp)){
	    fscanf(fp,"%s",word);
	    insert(HT, word, "\0");
    }
}
