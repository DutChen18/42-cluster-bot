import sys
import time
import subprocess

inp = sys.argv
player1_wins = 0
player2_wins = 0
total_time = 0
turn = 0
for i in range(0, int(inp[4])):
	start = time.time()
	if turn % 2 == 0:
		program = subprocess.run([inp[1], inp[2], inp[3]], capture_output=True, text=True)
		if program.stdout == "Player 1 wins\n":
			player1_wins += 1
		elif program.stdout == "Player 2 wins\n":
			player2_wins += 1
	else:
		program = subprocess.run([inp[1], inp[3], inp[2]], capture_output=True, text=True)
		if program.stdout == "Player 1 wins\n":
			player2_wins += 1
		elif program.stdout == "Player 2 wins\n":
			player1_wins += 1
	end = time.time()
	total_time += end - start
	turn += 1
total_time /= int(inp[4])
print("player1 wins: " + str(player1_wins) + " player2 wins: " + str(player2_wins))
print("avarage time per run: " + str(total_time) + "\n")