from asyncio import threads
import sys
import time
import subprocess
import threading
import argparse

inp = sys.argv
player1_wins, player2_wins, games_ran, total_time, player1_games_played, player2_games_played, player1_out_of_chips, player2_out_of_chips, player1_timed_out, player2_timed_out = 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0

parser = argparse.ArgumentParser(description='Benchmark 2 bots versus each other\n')
parser.add_argument('game_path', type=str, help='Path of game')
parser.add_argument('player_1_path', type=str, help='Path of player 1')
parser.add_argument('player_2_path', type=str, help='Path of player 2')
parser.add_argument('--games', type=int, help='Amount of games to run', default=10)
parser.add_argument('--threads', type=int, help='Amount of threads to run', default=1)
args = parser.parse_args()

def run_battle():
	global args, player1_wins, player2_wins, total_time, games_ran, player1_games_played, player2_games_played, player1_out_of_chips, player2_out_of_chips, player1_timed_out, player2_timed_out
	turn = 0
	for x in range(0, int(args.games / args.threads)):
		start = time.time()
		if turn % 2 == 0:
			program = subprocess.run([args.game_path, args.player_1_path, args.player_2_path], capture_output=True, text=True)
			if program.stderr == "Player 1 is out of chips\n":
				player1_out_of_chips += 1
			elif program.stderr == "Player 2 is out of chips\n":
				player2_out_of_chips += 1
			elif program.stderr == "Player 1 timed out\n":
				player1_timed_out += 1
			elif program.stderr == "Player 2 timed out\n":
				player2_timed_out += 1
			if program.stdout == "Player 1 wins\n":
				player1_wins += 1
			elif program.stdout == "Player 2 wins\n":
				player2_wins += 1
			player1_games_played += 1
		else:
			program = subprocess.run([args.game_path, args.player_2_path, args.player_1_path], capture_output=True, text=True)
			if program.stderr == "Player 1 is out of chips\n":
				player2_out_of_chips += 1
			elif program.stderr == "Player 2 is out of chips\n":
				player1_out_of_chips += 1
			elif program.stderr == "Player 1 timed out\n":
				player2_timed_out += 1
			elif program.stderr == "Player 2 timed out\n":
				player1_timed_out += 1
			if program.stdout == "Player 1 wins\n":
				player2_wins += 1
			elif program.stdout == "Player 2 wins\n":
				player1_wins += 1
			player2_games_played += 1
		end = time.time()
		total_time += end - start
		games_ran += 1
		turn += 1

if __name__ == "__main__":
	threads = [None] * args.threads
	for x in range(len(threads)):
		threads[x] = threading.Thread(target=run_battle)
		threads[x].start()
	for x in range(len(threads)):
		threads[x].join()
	print('\nRan '+ str(games_ran) + ' games on ' + str(args.threads) + ' thread(s)\n')
	print("Player 1 wins: " + str(player1_wins) + " - Player 2 wins: " + str(player2_wins) + "\n")
	print("Games played:	Player 1: " + str(player1_games_played) + " - Player 2: " + str(player2_games_played))
	print("Out of chips:	Player 1: " + str(player1_out_of_chips) + " - Player 2: " + str(player2_out_of_chips))
	print("Timed out:	Player 1: " + str(player1_timed_out) + " - Player 2: " + str(player2_timed_out))
	print("Total time: " + str(total_time))
	total_time /= games_ran
	print("Avarage time per run: " + str(total_time))