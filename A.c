#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//anggapan semua kata di trie unik
struct node{
    //untuk menyimpan karakter di node tersebut
    char value;
    //deskripsi dari kata yang berakhir di node tersebut
    char description[100];
    //menyimpan node-node anak jumlah 26 untuk setiap huruf di alphabet
    struct node *children[26];
    //menandakan akhir dari kata
    int endOfWord;
}; 

//biar rapi aja
struct node* createNode(char c);
void insert(char *word, char *description);
void printTrie(struct node *root, char *word, int level);
struct node *printStartWith(struct node *root, char *prefix);
struct node *searchWord(struct node *root, char *word);
int hasNoChildren(struct node *curr);
int removeWordHelper(struct node *curr, char *word, int depth, int len);
void removeWord(struct node *root, char *word);
int isEmpty(struct node *root);
void menu();

//root dari trie untuk menyimpan semua kata
struct node *root = NULL;
//index untuk angka ketika display
int countIndex;

int main(){
    //inisialisasi root node diisi spasi karena tidak bisa membawa karakter
    //entah kenapa gk bisa di atas
    root = createNode(' ');

    //data testing
    insert("macaco", "world");
    insert("macaroni", "hello");
    insert("macaroon", "okay");
    insert("machinable", "okay");
    insert("machine", "okay");
    insert("macromolecular", "okay");
    insert("macroscopic", "okay");
    insert("macronuclear", "okay");

    menu();
    return 0;

}

void menu(){
    //gbs ini
    // system("cls");
    system("clear");
    int choice;
    char word[100];
    char description[100];
    printf("1. Release a new slang word\n");
    printf("2. Search for a slang word\n");
    printf("3. View all slang words starting with a certain prefix word\n");
    printf("4. View all slang words\n");
    printf("5. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice){
        case 1:
            int countSpaces;
            while (1){
                printf("Input a new slang word [must be more than 1 characters and contains no spaces]: ");
                //ternyata salah di sini
                scanf(" %[^\n]", word); getchar();
                countSpaces = 0;
                for (int i = 0; word[i] != '\0'; i++){
                    if (word[i] == ' '){
                        countSpaces++;
                    }
                }
                //code bugging here when len of word is 2 so added special condtion
                //gtau ini gara2 otak ngebug atau emg c aja yg aneh
                // if (strlen(word) == 2 && countSpaces == 0) break;
                if(strlen(word) < 2 || countSpaces != 0){
                    continue;
                }else{
                    break;
                }
            }
            while (1){
                printf("Input new slang word description [must be more than 2 words]: ");
                scanf(" %[^\n]", description); getchar();
                countSpaces = 0;
                for (int i = 0; description[i] != '\0'; i++){
                    if (description[i] == ' '){
                        countSpaces++;
                    }
                }
                if (countSpaces < 2){
                    continue;
                }else{ 
                    break;
                }
            }
            insert(word, description);
            printf("Press enter to continue...");
            getchar();
            menu();
            break;
        case 2:
            while (1){
                printf("Input a slang word to be searched [must be more than 1 characters and contains no spaces]: ");
                //lupa ganti
                scanf(" %[^\n]", word); getchar();
                if (strlen(word) < 2){
                    continue;
                }else if(strchr(word, ' ') != NULL){
                    continue;
                }else{
                    break;
                }
            }
            struct node *curr = searchWord(root, word);
            if(curr != NULL){
                printf("\nSlang word: %s\n", word);
                printf("Description: %s\n", curr->description);
            }else {
                printf("\nThere is no word \"%s\" in the dictionary\n", word);
            }
            printf("\nPress enter to continue...");
            getchar();
            menu();
            break;
        case 3:
            char prefix[100];
            countIndex = 1;
            printf("Input a prefix to be searched: ");
            getchar();
            scanf("%s", prefix); getchar();
            printStartWith(root, prefix);
            printf("\nPress enter to continue...");
            getchar();
            menu();
            break;
        case 4:
            countIndex = 1;
            if(isEmpty(root)){
                printf("There is no slang word yet in the dictionary\n");
            }else{
                printf("List of all slang words in the dictionary:\n");
                printTrie(root, (char*)malloc(100), 0);
            }
            getchar();
            printf("\nPress enter to continue...");
            getchar();
            menu();
            break;
        case 5:
            printf("Thank you... Have a nice day :)\n");
            getchar();
            exit(1);
            break;
        //tambah delete function ah

        //input diluar 1-5 mengulang
        default:
            menu();
            break;
    }    
}

//fuunction yang mengalokasi memori untuk node baru jika dibutuhkan
struct node* createNode(char c){
    struct node *newnode = (struct node*)malloc(sizeof(struct node));
    newnode->value = c;
    for (int i = 0; i < 26; i++){
        newnode->children[i] = NULL;
    }
    //menandai bahwa belum ada kata yang berakhir di node tersebut
    newnode->endOfWord = 0;
    return newnode;
}

void insert(char *word, char *description){
    //inisialisasi root sebagai curr agar bisa dipindah tanpa kehilangan akar
    struct node *curr = root;
    for(int i = 0; word[i] != '\0'; i++){
        //ambil index yaitu huruf dikurang karakter a agar indeks bisa antara 0-25
        int index = word[i] - 'a';
        if(curr->children[index] == NULL){
            //buat node baru jika belum ada
            curr->children[index] = createNode(word[i]);
        }
        //pindah curr ke anak 
        curr = curr->children[index];
    }
    //update description jika kata sudah ada dan membuat description baru jika belum ada
    strcpy(curr->description, description);
    if (curr->endOfWord == 1){
        printf("\nSuccessfully updated a slang word\n");
        return;
    }
    //menandai adanya kata yang berakhir di node ini
    curr->endOfWord = 1;
    printf("\nSuccessfully released new slang word\n");
}

void printTrie(struct node *root, char *word, int level) {
    //jika sudah sampai di akhir kata, print kata tersebut
    if (root->endOfWord) {
        word[level] = '\0';
        printf("%d. %s\n", countIndex, word);
        countIndex++;
    }

    //mencoba mencari kata2 selanjutnya secara rekursif
    for (int i = 0; i < 26; i++) {
        if (root->children[i]) {
            word[level] = i + 'a';
            printTrie(root->children[i], word, level + 1);
        }
    }
}

struct node *printStartWith(struct node *root, char *prefix) {
    struct node *curr = root;
    char word[100]; 
    int level = 0;

    //untuk mencari node terakhir dari sebuah prefix
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        //apabila prefix yang dicari tidak ada di dalam trie
        if (curr->children[index] == NULL) {
            printf("\nThere is no prefix \"%s\" in the dictionary\n", prefix);
            return NULL;
        }
        curr = curr->children[index];
        word[level++] = prefix[i];
    }

    printf("\nWords that start with \"%s\":\n", prefix);
    //setelah menemukan node terakhir dari prefix, print semua kata yang berawalan dengan prefix tersebut
    printTrie(curr, word, level);
    return curr;
}

//return type adalah node agar dapat membaca deskrpsi di kata yang berakhir di node tersebut
struct node *searchWord(struct node *root, char *word) {
    struct node *curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        //tidak ada node dengan kata tersebut
        if (curr->children[index] == NULL) {
            // printf("Word not found\n");
            return 0;
        }
        curr = curr->children[index];
    }
    //ditemukan kata di node tersebut karena end of word true
    if (curr->endOfWord) {
        // printf("Word found\n");
        return curr;
    //tidak ditemukan kata di node tersebut
    } else {
        // printf("Word not found\n");
        return NULL;
    }
}

//untuk memperiksa apakah sebuah node memiliki anak atau tidak
//eh ini sama fungsi isEmpty sama ya
//yauda lah males ganti
int hasNoChildren(struct node *curr) {
    for (int i = 0; i < 26; i++) {
        if (curr->children[i] != NULL) {
            return 0;
        }
    }
    return 1;
}

int removeWordHelper(struct node *curr, char *word, int depth, int len) {
    //node kosong
    if (curr == NULL) {
        return 0; 
    }
    //sampai di node yang berisi kata yang akan dihapus
    if (depth == len) {
        if (curr->endOfWord) {
            curr->endOfWord = 0;
            if (hasNoChildren(curr)) {
                return 1;
            }
            return 0;
        }
    } else {
        int index = word[depth] - 'a';
        if (removeWordHelper(curr->children[index], word, depth + 1, len)) {
            //bebaskan node yang tidak memiliki anak dan bukan merupakan akhir dari suatu kata
            free(curr->children[index]);
            //set anak-anaknya ke null agar tidak terjadi kesalahan
            curr->children[index] = NULL;

            return (!curr->endOfWord && hasNoChildren(curr));
        }
    }
    return 0;
}

//main removal function
void removeWord(struct node *root, char *word) {
    int len = strlen(word);
    if (len > 0 && root != NULL) {
        removeWordHelper(root, word, 0, len);
    }
}

//untuk memperiksa apakah root kosong atau berisi
int isEmpty(struct node *root) {
    for (int i = 0; i < 26; i++) {
        //ada anak
        if (root->children[i] != NULL) {
            return 0;
        }
    }
    return 1;
}

