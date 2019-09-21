#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <iterator>
#include <time.h>

using namespace std;

int deck[52] = {1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13};

bool get_move(){
	int move;
	cout << "1 for yes and 2 for no: " << flush;
	while (!(cin >> move) || (move != 1 && move != 2)){
		cout << "Error: Please enter 1 or 2: ";
		cin.clear();
		cin.ignore(123,'\n');
	}
	cout << "\n";
	return move==1;
}

void shuffle(int num){
	int a, b, temp;
	for (int i=0; i<num; i++){
		a = rand()%52;
		b = rand()%52;
		temp = deck[a];
		deck[a] = deck[b];
		deck[b] = temp;
	}
	return;
}

int minimum(vector<int> arr){
	int score=0, card;
	bool first=true;
	for (int i=0; i<arr.size(); i++){
		card = arr[i];
		if (card>=10)
			score += 10;
		else
			score += card;
	}
	return score;
}

int maximum(vector<int> arr){
	bool first_ace = true;
	int card, score=0;
	for (int i=0; i<arr.size(); i++){
		card = arr[i];
		if (card==1){
			if (first_ace){
				first_ace = false;
				score += 11;
			}
			else
				score++;
		}
		else if (card >= 10)
			score += 10;
		else
			score += card;
	}
	if (score > 21)
		return minimum(arr);
	return score;
}

void display(vector<int> arr){
	int card, a, b;
	for (int i=0; i<arr.size(); i++){
		card = arr[i];
		if (card == 1)
			cout << "A ";
		else if (card>10){
			if (card==11) cout << "J ";
			else if (card==12) cout << "Q ";
			else cout << "K ";
		}
		else{
			cout << card << ' ';
		}
	}
	a = minimum(arr);
	b = maximum(arr);
	cout << " Total: " << a << ' ';
	if (b < 22 && b!=a) {
		cout << "or " << b;
	}
	cout << "\n";
	return;
}
int index;
int next_card(){
	int card;
	card = deck[index++];
	index = index % 52;
	return card;
}

void show_both(vector<int> arr1, vector<int> arr2){
	cout << "Computer: ";
	display(arr1);
	cout << "Player: ";
	display(arr2);
	cout << "\n";
	return;
}


int main()
{	
	int win=0, lost=0, draw=0;
	srand((unsigned)time(NULL));	// rng
	bool first, agent_stop, player_stop, over;
	int agent_score, player_score;
	while(1){
		cout << "Would you like a chance to draw first after initial two cards?\n";
		first = get_move();
		agent_stop = false;
		player_stop = false;
		over = false;
		vector<int> agent;
		vector<int> player;

		shuffle(52 + rand()%52);

		index = rand()%52;
		agent.push_back(next_card());
		agent.push_back(next_card());
		player.push_back(next_card());
		player.push_back(next_card());
		show_both(agent, player);

		if (first){
			cout << "More cards?\n";
			if (get_move()){
				player.push_back(next_card());
				show_both(agent, player);
				if (minimum(player) > 21){
					cout << "\nComputer Win!\n\n"; win ++;
					over = true;
				}
			}
			else {
				player_stop = true;
			}

		}

		while (!over){
			if ((maximum(agent) > maximum(player)) && player_stop==true){
				cout << "Computer Win!\n\n"; win ++;
				break;
			}
			if (!agent_stop){
				agent_score = maximum(agent);
				player_score = maximum(player);
				if (agent_score < player_score || agent_score<12 || (rand()%13 < 21 - agent_score)){
					agent.push_back(next_card());
					show_both(agent, player);
					if (minimum(agent) > 21){
						cout << "\nPlayer Win!\n\n"; lost ++;
						break;
					}
				}
				else {
					agent_stop = true;
				}
			}

			if (!player_stop){
				cout << "More cards?\n";
				if (get_move()){
					player.push_back(next_card());
					show_both(agent, player);
					if (minimum(player) > 21){
						cout << "\nComputer Win!\n\n"; win ++;
						break;
					}
				}
				else {	// player stops
					player_stop = true;
				}
			}

			if (agent_stop && player_stop){
				agent_score = maximum(agent);
				player_score = maximum(player);
				if (agent_score > player_score){ cout << "Computer Win!\n\n"; win ++;}
				else if (agent_score < player_score){ cout << "Player Win!\n\n"; lost ++;}
				else {cout << "Draw!\n\n"; draw ++;}
				break;
			}
			
		}
		// for (int i=0; i<52; i++){
		// 	cout << deck[i] << ' ';
		// }
		// cout << "\n";
		// for (int i=0; i<agent.size(); i++){
		// 	cout << agent[i] << ' ';
		// }
		// cout << "\n";
		// for (int i=0; i<player.size(); i++){
		// 	cout << player[i] << ' ';
		// }
		// cout << "\n";
		cout << "Would you like to play another game?\n";
		if (!get_move()) break;
	}
	cout << "\nWin: " << win << "  Lost: " << lost << "  Draw: " << draw; 
	return 0;
}

