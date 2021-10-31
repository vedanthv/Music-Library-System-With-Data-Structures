//for standard ip/op
#include <stdio.h> 
//for dynamic memory allocation with malloc
#include <stdlib.h>
//for string built in functions
#include <string.h> 
//allows boolean to be used as a datatype
#include <stdbool.h> 
//package handles all character functions
#include <ctype.h> 
//handles all math functions
#include <math.h> 
//handles all timing functions. We are using only sleep in the project
#include<time.h>
//gives access to many macros in Windows OS. Here we are using Sleep(sometime_in_ms) to put the computer to sleep incase of inactivity
#include <windows.h> 

#define MAX_SONG_POOL 120 //defines that maximum no of songs that can be added to out list is 120
#define MAX_STRING_SIZE 75 //name of the song must be within 75 characters

// Data Structure for an individual song

typedef struct Song *songptr; // define a pointer which stores the address of the current song node
typedef struct Song // struct to define all the characteristics of the song
{
    char title[MAX_STRING_SIZE];
    char album[MAX_STRING_SIZE];
    char uri[MAX_STRING_SIZE];
    char id[MAX_STRING_SIZE];
    short int year;
    double duration;
} Song;

songptr song_pool[MAX_SONG_POOL]; //an array to store all the songs. Existing ones as well as new songs added here. The limit is 120 songs

int pool_insert_index = 0; // starting index of the song list is stored in pool_insert_index and the value is 0(first position of array signifying the first song)

// Lets create new struct for playlist
typedef struct PlaylistNode *node_ptr; // a pointer to store and keep traversing through the playlist of songs
typedef struct PlaylistNode // this is a double ended linked list. We have the song as data along with the address of the previous node(prev_song) and the next node(next_song)
{
    songptr song;
    node_ptr prev_song;
    node_ptr next_song;
} PlaylistNode;

// header_node is currently null because at this point of time no playlist is there. Also now_playling is null because no song is playing since no playlist is there
node_ptr header_node = NULL, now_playing = NULL;

//Function to check if the song pool is empty
bool is_pool_empty()
{
    return song_pool[0] == NULL;
}

//Function to check if playlist exists
bool does_playlist_exist()
{
    return !(header_node == NULL);
}

// Function to play next song
void play_next_song()
{
    //condition to check if playlist exists
    if (does_playlist_exist() && now_playing != NULL) 
    {
        //if the song isn't the last song ie, next address of current node is not null
        if (now_playing->next_song != NULL)
            now_playing = now_playing->next_song; //go to the next node(song)
        else
            printf("REACHED END OF PLAYLIST"); //if song is last song in playlist print this
    }
    else
    {
        printf("NO SONG ADDED TO PLAYLIST"); //if no playlist and no song is being played then print this
    }
}

void play_prev_song()
{
    // chk playlist exists and currently song is playing
    if (does_playlist_exist() && now_playing != NULL)
    {
        if (now_playing->prev_song != NULL) //if the song isn't the first in the playlist
            now_playing = now_playing->prev_song; // go to the previous node(song) of the playlist
        else
            printf("REACHED START OF PLAYLIST");
    }
    else
    {
        printf("NO SONG ADDED TO PLAYLIST");
    }
}

//Function to show the song details
void show_song_details()
{
    //if no song is playing or the song has no properties(title,duration etc)
    if (now_playing == NULL || now_playing->song == NULL)
    {
        printf("                       CREATE A PLAYLIST FIRST\n");
    }
    else
    {
        printf("\n----------------------------------------------------------------------\n");
        printf("                          DETAILED OVERVIEW\n");
        printf("----------------------------------------------------------------------\n");
        printf("\t   TITLE    |   %s\n", now_playing->song->title); // since now_playing is pointer pointing to address of the current song, we can get the member functions(title, duration etc...) of the song 
        printf("\t   ALBUM    |   %s\n", now_playing->song->album);
        printf("\t   YEAR     |   %d\n", now_playing->song->year);
        printf("\t  DURATION  |   %0.2lf minutes\n", now_playing->song->duration);
        printf("\tSPOTIFY URI |   %s\n", now_playing->song->uri);
        printf("    Paste the above URL in your browser to play the song on Spotify\n");
        printf("----------------------------------------------------------------------\n");
    }
    // make a char variable called leave
    char leave[MAX_STRING_SIZE];
    printf("<<<<< Enter any input to return to MAIN MENU\n");
    if (scanf("%s", leave)) //if user types anything then clear screen and return to show_songs_details page
    {
        system("cls");
        return;
    }
}

// Main Menu Of Application
void main_menu()
{
    printf("\n----------------------------------------------------------------------\n");
    printf("                              MAIN MENU\n");
    printf("----------------------------------------------------------------------\n");

    // if no song playing or no no playlist or last song reached
    if (now_playing == NULL || now_playing->song == NULL)
    {
        printf("                          NO TRACK SELECTED\n");
    }
    // display the current song playing and its duration
    else
    {
        printf("    NOW    |   %s\n", now_playing->song->title);
        printf("  PLAYING  |   %0.2lf minutes\n", now_playing->song->duration);
    }

    // Wide Variety of Options given to the user
    printf("----------------------------------------------------------------------\n");
    printf("   #   |  Action   \n");
    printf("----------------------------------------------------------------------\n");
    printf("   1   |  Create a new song\n");
    printf("   2   |  Display all available songs\n");
    if (does_playlist_exist())
        printf("   3   |  Delete playlist\n");
    else
        printf("   3   |  Create a new playlist\n");
    printf("   4   |  Add a song to the playlist\n");
    printf("   5   |  Show playlist\n");
    printf("   6   |  Play previous track\n");
    printf("   7   |  Play next track\n");
    // if the now_playing pointer doesnt point to null we can fetch more details about the song
    if (now_playing != NULL && now_playing->song != NULL)
        printf("   8   |  Show more information about the song playing\n");
    printf("  -1   |  Exit music player\n");
    printf("----------------------------------------------------------------------\n");
    printf("                        Enter your choice below\n");
}

// Functions to show all the songs from the song_pool array
void show_all_songs_raw()
{
    // loop runs from 0th index to MAX_SONG_POOL - 1 th index
    for (int i = 0; i < MAX_SONG_POOL && song_pool[i] != NULL; i++)
        // print title , album, duration etc...
        printf("%-1d %-3s %-3s %-3d %0.2lfmin\n", (i + 1), song_pool[i]->title, song_pool[i]->album, song_pool[i]->year, song_pool[i]->duration);
}

// Function to show the playlist
void show_playlist()
{
    if (does_playlist_exist())
    {
        printf("\n----------------------------------------------------------------------\n");
        printf("                          YOUR PLAYLIST\n");
        printf("----------------------------------------------------------------------\n");
        printf(" Title                                            | Duration\n");
        printf("----------------------------------------------------------------------\n");
        node_ptr current = header_node; // create a node current and assign it to header_node
        // until the current node doesn't become null or the playlist ends
        while (current != NULL && current->song != NULL) 
        {
            // next will point to the next song
            node_ptr next = current->next_song;
            // print all the song details that current points to
            printf(" %-48.48s | %2.2lf min\n", current->song->title, current->song->duration);
            // move current forward
            current = next;
        }

        printf("----------------------------------------------------------------------");
    }
    else
    {
        printf("\nYou haven't created a playlist yet.\n");
    }
}

// Function to Dislpay Songs Pagewise 
void pagewise_song_display(int step)
{
    printf("\n----------------------------------------------------------------------\n");
    printf("                             SONGS LIST\n");
    printf("----------------------------------------------------------------------\n");
    printf("   #   |  Title                                            | Duration\n");
    printf("----------------------------------------------------------------------\n");
    // We have to display 10 songs in one page so the starting counter will be step - 10

    for (int i = step - 10; i < step && song_pool[i] != NULL; i++)
    {
        printf("  %2d   |  %-48.48s | %2.2lf min\n", (i + 1), song_pool[i]->title, song_pool[i]->duration); // print song title and duration
    }
    printf("----------------------------------------------------------------------\n");
    printf("[Enter -2 to go to prev page] | Page %d | [Enter -1 to go to next page]\n", ((int)step / 10)); // display the page number. So if there are only 10 songs 10/10 is 1 so page no is 1. 
    printf("----------------------------------------------------------------------\n");
    printf(" <<<< Enter 0 to go back to main menu.\n");

    printf("----------------------------------------------------------------------\n");
    printf("                        Enter your choice below\n");
}

// Function to select songs
int song_selector()
{
    int song_number = -1; // currently no song
    int step = 10; //how many songs to display on one page

    //until song number is more than 0 or the song number exceeds the total songs in the playlist, keep playing
    while (song_number < 0 || song_number > pool_insert_index)
    {
        pagewise_song_display(step); // display all songs
        char input[MAX_STRING_SIZE];
        scanf("%s", input);

        // we are copying the value of input and storing it in &song_number. If this cannot be done,  then the input we have given is larger than the song_pool
        if (!sscanf(input, "%d", &song_number))
        {
            system("cls");
            printf("You seem to have entered an invalid input.\nReturning to main menu.");
            return 0;
        }

        //Function to basically go one page backwards in songs list
        if (song_number == -2)
        {
            if (step > 10)
            {
                step -= 10; // to go 10 songs back in song_pool
                system("cls");
            }
            else // if step < 10 then obviously we can't go back to previous page
            {
                system("cls");
                printf("\nThis itself is the first page");
            }
        }

        // FUnction that increases the step by 10 and goes to the next page of songs
        else if (song_number == -1)
        {
            if (step < ceil(pool_insert_index / 10.0) * 10) //important sourced from a blog
            {
                step += 10;
                system("cls");
            }
            else
            {
                system("cls");
                printf("\nThis is the last page");
            }
        }

        // If any other choice entered or the song_number exceeds the total songs
        else if (song_number < -2 || song_number > pool_insert_index)
        {
            system("cls");
            printf("\nENTER A VALID CHOICE\n");
        }
    }

    return song_number;
}

// Creating a node to store the song details
// Later on in user_song_input the new songs will be created by user using this struct
Song *createSong(const char *title, const char *album, const short int year, const double duration, const char *uri, const char *id)
{
    // strcpy copies the value of string 2 to string 1. We are copying the value of the value of title to temp -> title and similarly for the other properties
    Song *temp = malloc(sizeof(Song));
    strcpy(temp->title, title);
    strcpy(temp->album, album);
    strcpy(temp->uri, uri);
    strcpy(temp->id, id);
    temp->year = year;
    temp->duration = duration;
    pool_insert_index++; // move to the next index in the song_pool
    return temp;
}

//Allocates memory for a new playlist and prompts user to add a song to it
void create_playlist()
{
    int song_number = 0, i = 0;

    node_ptr temp = (node_ptr)malloc(sizeof(PlaylistNode));
    printf("\nPick the song you want to add to your new playlist\n");
    song_number = song_selector(); // pick the song to be added. We get the song_number returned...
    if (song_number > 0 && song_number <= 120)
    {
        temp->song = song_pool[song_number - 1]; // the details of the song is stored in the song pool. The song is created using the Create Song structure described previously
        temp->prev_song = NULL;
        temp->next_song = NULL;
        header_node = temp; // move the header node to the current newly added song
        now_playing = temp; // set now_playing to the newly added song so that it will play first when playlist is selected

        system("cls");
        printf("---\n%s has been added to your new playlist.\n---", song_pool[song_number - 1]->title);
    }
    else if (song_number == 0)
    {
        system("cls");
    }
    else
    {
        printf("\nThere was a problem while handling your request, try again.\n");
    }
}

//Prompts user to pick a song from the pool and add it to the existing playlist
void add_to_playlist()
{
    int song_number = 0;
    int step = 10;
    printf("\nPick the song you want to add\n");
    song_number = song_selector();
    // maximum value of pool insert index is MAX length of song_pool (120) - 1
    if (song_number > 0 && song_number <= pool_insert_index)
    {
        node_ptr new_node = (node_ptr)malloc(sizeof(PlaylistNode)); // create a new playlist
        node_ptr last = header_node; // will help later in traversal
        new_node->song = song_pool[song_number - 1]; //store the song details in the data part of the doubly linked list node
        new_node->next_song = NULL; // since this is the first song being created the address of next is NULL.

        /* 4. If the Linked List is empty basically song_pool has no song at all, then make the new node as head */
        if (header_node == NULL)
        {
            new_node->prev_song = NULL;
            // make both header_node and now_playing point to the new_node
            header_node = new_node;
            now_playing = new_node;
        }
        else
        {
            //Traverse till the last node
            while (last->next_song != NULL)
                last = last->next_song;
            // add new song
            last->next_song = new_node;
            // new song's previous node contains address of last
            new_node->prev_song = last;
        }
        system("cls");
        printf("%s has been added to your playlist.\n", song_pool[song_number - 1]->title);
    }
    else if (song_number == 0)
    {
        system("cls");
    }
    else
    {
        printf("\nThere was a problem while handling your request.\n");
    }
}

/** CSV Parser that maps a data set of upto songs onto the song pool
    Songs must be present in the root directory at rawdata.csv

    Sourced from Google : https://stackoverflow.com/questions/12911299/read-csv-file-in-c
*/
void readFromCSV()
{
    FILE *file = fopen("rawdata.csv", "r");
    char line[1000];
    int i = 0;

    if (file == NULL)
    {
        perror("******\nUnable to load songs from data source");
        printf("Please check if rawdata.csv exists at the root directory of the application.\n");
        printf("You can still enter songs manually.\n******\n");
        printf("Error Message");
    }
    else
    {
        char songtitle[MAX_STRING_SIZE];
        char releaseDate[MAX_STRING_SIZE];
        int durationms;
        double duration = 0.0;
        char id[MAX_STRING_SIZE];
        char album[MAX_STRING_SIZE];
        char uri[MAX_STRING_SIZE];
        int year;
        while (fgets(line, sizeof(line), file) && pool_insert_index < MAX_SONG_POOL)
        {
            char *token;
            token = strtok(line, ","); //tokenizes current line with delimiter ',' and returns the first substring
            if (token == NULL)
                continue;
            strcpy(songtitle, token);
            token = strtok(NULL, ","); // get the next token
            strncpy(releaseDate, token + 1, 4);
            sscanf(releaseDate, "%d", &year);
            token = strtok(NULL, ",");
            char temp[MAX_STRING_SIZE];
            strncpy(temp, token + 1, 6);
            sscanf(temp, "%d", &durationms);
            duration = durationms / 60000.0;
            token = strtok(NULL, ","); // get the next token
            strcpy(id, token);
            token = strtok(NULL, ","); // get the next token
            strcpy(album, token);
            token = strtok(NULL, ","); // get the next token
            strcpy(uri, token);
            token = strtok(NULL, ","); // get the next token
            //printf("Song read : %s  %s\n",songname,durationm);
            if (i == 0)
            {
                i++;
                continue;
            }
            song_pool[i - 1] = createSong(songtitle, album, year, duration, uri, id);
            i++;
        }
    }

    fclose(file);
}
