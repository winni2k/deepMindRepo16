# now visualize the output
from mpl_toolkits.axes_grid.axislines import SubplotZero
import matplotlib.pyplot as plt
import numpy as np


# convert y to reward
def toReward(val):
	"""Convert wins to reward"""
	if val == 2:
		return -1
	return val

def readLog(f):
	"""Read log from file descriptor and return values to plot"""
	# get rid of header
	f.readline()
	x = []
	y = []
	for line in f:
		sline = line.split('\t')
		x.append(int(sline[0]))
		y.append(int(sline[1].rstrip('\n')))

	return [x, y]

def averageOverN(n, y):

	# average data over n episodes
	y2 = [toReward(v) for v in y]
	i = 0;
	total = 0;
	y3 = []
	for val in y2:
		total += val
		i += 1;
		if i == n:
			y3.append(total/float(n))
			total = 0
			i = 0

	return y3

# run command for training up a pair of tictactoe players
from subprocess import call
call(["src/ttt","--log","ttt.sarsa.log","--learn-algorithm","SARSA"])
call(["src/ttt","--log","ttt.qlearn.log","--learn-algorithm","QLearn"])

# load log data produced by ttt
sarsa = readLog(open('ttt.sarsa.log'))
qlearn = readLog(open('ttt.qlearn.log'))

# set up figure plotting
fig = plt.figure(1)
ax = SubplotZero(fig, 111)
fig.add_subplot(ax)

for direction in ["xzero", "yzero"]:
	ax.axis[direction].set_axisline_style("-|>")
	ax.axis[direction].set_visible(True)

ax.axis["xzero"].set_label("Episodes")
ax.axis["yzero"].set_label("Reward")
	
for direction in ["left", "right", "bottom", "top"]:
	ax.axis[direction].set_visible(False)

n=5000
ax.plot(sarsa[0][0::n], averageOverN(n, sarsa[1]), label="Reward of SARSA learning, averaged over {0} episodes".format(n))
ax.plot(qlearn[0][0::n], averageOverN(n, qlearn[1]), label="Reward of Q-learning, averaged over {0} episodes".format(n)) 
ax.legend()
plt.ylim([0,1])

# show result on screen
plt.show()



