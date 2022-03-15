from multiprocessing import Process
import sys
import time
import subprocess
import threading
import argparse

inp = sys.argv
player1_wins = 0
player2_wins = 0
games_ran = 0
total_time = 0

# parser = argparse.ArgumentParser(description='Benchmark 2 bots versus each other\n')
# parser.add_argument('Game path', type=str, help='Path of game ')
# parser.add_argument('Player 1 path', type=str, help='Path of player 1 ')
# parser.add_argument('Player 2 path', type=str, help='Path of player 2 ')
# parser.add_argument('Amount to run', type=int, help='Amount of games to run ')
# parser.add_argument('Amount of threads', type=int, help='Amount of threads to run ')
# inp = parser.parse_args()

def run_battle():
	global inp
	global player1_wins
	global player2_wins
	global total_time
	global games_ran
	print('entered run_battle\n')
	print(inp)
	turn = 0
	games = int(inp[4]) / int(inp[5])
	for x in range(0, int(games)):
		start = time.time()
		if turn % 2 == 0:
			program = subprocess.run([inp[1], inp[2], inp[3]], capture_output=True, text=True)
			if program.stdout == "Player 1 wins\n":
				print('player 1 wins')
				player1_wins += 1
			elif program.stdout == "Player 2 wins\n":
				print('player 2 wins')
				player2_wins += 1
		else:
			program = subprocess.run([inp[1], inp[3], inp[2]], capture_output=True, text=True)
			if program.stdout == "Player 1 wins\n":
				print('player 1 wins')
				player2_wins += 1
			elif program.stdout == "Player 2 wins\n":
				print('player 2 wins')
				player1_wins += 1
		end = time.time()
		total_time += end - start
		games_ran += 1
		turn += 1

if __name__ == "__main__":
	Pros = []
	for x in range(0, int(inp[5])):
		print ("Thread Started")
		p = Process(target=run_battle)
		Pros.append(p)
		p.start()

	for t in Pros:
		t.join()
	print('Ran '+ str(games_ran) + ' games on ' + inp[5] + ' threads\n')
	print("Player_1 wins: " + str(player1_wins))
	print("Player_2 wins: " + str(player2_wins))
	print("Avarage time per run: " + str(total_time) + "\n")