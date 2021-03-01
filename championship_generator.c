#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG 0

#define BUFFER_SIZE 1024
#define REST_NAME "REST"
#define REST_LEN 4


struct __node {
	char* player;
	struct __node** opponents_list;
	int color;
};

int gcd(int a, int b);

int main(void){

	printf("Number of players: ");
	int players_count = -1;

	while (scanf("%d", &players_count) == 0 || players_count <= 0){

		fprintf(stderr, "ERROR: Insert a positive integer greather than 0\n");
		printf("Number of players: ");
		
		scanf("%*[^\n]");
		scanf("%*c");

	}

	scanf("%*[^\n]");
	scanf("%*c");

	//Add 1 slot for "rest" day if number of players is uneven.
	char** players_list = malloc( sizeof(char**) * (players_count + players_count % 2) );

	char* name_buffer = malloc( sizeof(char) * BUFFER_SIZE);
	memset(name_buffer, '\0', sizeof(char) * BUFFER_SIZE);

	for (int i = 0; i < players_count; ++i){

		printf("Name of player #%d: ", i + 1);

		scanf("%s", name_buffer);
		scanf("%*[^\n]");
		scanf("%*c");

		size_t name_len = strnlen(name_buffer, BUFFER_SIZE);
		players_list[i] = malloc( sizeof(char*) * (name_len+1));
		memset(players_list[i], '\0', sizeof(char*) * (name_len+1));
		strncpy(players_list[i], name_buffer, name_len);

	}
	free(name_buffer);

	if (players_count % 2) {
		players_list[players_count] = malloc( sizeof(char*) * (REST_LEN+1));
		memset(players_list[players_count], '\0', sizeof(char*) * (REST_LEN+1));
		strncpy(players_list[players_count], REST_NAME, REST_LEN);	
		++players_count;
	}

	//Shuffling players list using Fisher and Yates method
	//	https://en.wikipedia.org/wiki/Fisher–Yates_shuffle#The_modern_algorithm
	srand(time(NULL));
	for (int i = players_count - 1; i >= 1; --i){
		int random_index = rand() % (i+1);
		char* exchange_temp = players_list[i];
		players_list[i] = players_list[random_index];
		players_list[random_index] = exchange_temp;
	}

	//We will use round robin to schedule the championship.
	//	The rotation factor must be coprime with (n-1).
	//	We chose a random rotation from the eligible ones.

	//Only a maximum of n-2 rotations can be possible
	//	(-1 because first position is fixed, -1 because rotating
	//	 n-1 positions would result in no change of positions).
	int* eligible_rotations = malloc( sizeof(int) * (players_count - 2) );
	eligible_rotations[0] = 1;
	int eligible_rotations_count = 1;

	for (int i = 2; i <= players_count - 2; ++i){
		if ( gcd(i, players_count-1) == 1 ){
			eligible_rotations[eligible_rotations_count++] = i;
		}
	}

	int random_index = rand() % eligible_rotations_count;
	int rotation_factor = eligible_rotations[random_index];
	free(eligible_rotations);

	puts("-----------------\n");
	
	int starting_first_player_index = 1;
	int starting_second_player_index = players_count-1;
	for (int days_count = 0; days_count < (players_count-1); ++days_count){

		int first_player_index = starting_first_player_index;
		int second_player_index = starting_second_player_index;

		printf("DAY #%d\n", days_count + 1);

		char* resting_player = NULL;

		for (int player_index = 0; player_index < players_count/2; ++player_index){

			char* first_player;
			if (player_index == 0){
			
				first_player = players_list[0];
			
			} else {
				
				first_player = players_list[first_player_index];
				++first_player_index;
				if (first_player_index == players_count){
					//Not 0 because first position is fixed
					first_player_index = 1;
				}

			}

			char* second_player = players_list[second_player_index];
			--second_player_index;
			if (second_player_index == 0){
				second_player_index = players_count - 1;
			}

			if ( strncmp(first_player, REST_NAME, REST_LEN) == 0 ){
				resting_player = second_player;
				continue;
			}

			if ( strncmp(second_player, REST_NAME, REST_LEN) == 0 ){
				resting_player = first_player;
				continue;
			}

			printf("\t%s:%s\n", first_player, second_player);

		}

		if (resting_player) printf("\t%s is resting.\n\n", resting_player);

		starting_first_player_index -= rotation_factor;
		//If starting_first_player_index <= 0 it means we crossed
		//	the fixed player, so we have to decrease one more.
		if (starting_first_player_index <= 0 ){
			starting_first_player_index -= 1;
			starting_first_player_index += players_count;
		}
		
		starting_second_player_index -= rotation_factor;
		//Again, if starting_second_player_index <= 0 it means we crossed
		//	the fixed player, so we have to decrease one more.
		if (starting_second_player_index <= 0 ){
			starting_second_player_index -= 1;
			starting_second_player_index += players_count;
		}

	}


	if (DEBUG) puts("-----------------");
	for (int i = 0; i < players_count; ++i){
		if (DEBUG) printf("Name of player #%d: %s\n", i + 1, players_list[i]);
		free(players_list[i]);
	}
	free(players_list);

}

int gcd(int a, int b){

	while ( b != 0 ){
		
		a %= b;

		a ^= b;
		b ^= a;
		a ^= b;

	
	}

	return a;

}