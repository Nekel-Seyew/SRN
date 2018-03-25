
import threading

_pt_thread_lock = threading.Lock()

_seed = 570956270388484096L

def seed_rand(i):
	global _pt_thread_lock
	global _seed
	_pt_thread_lock.acquire(True)
	_seed = i
	_pt_thread_lock.release()

def genrand():
	global _pt_thread_lock
	global _seed
	_pt_thread_lock.acquire(True)
	m = 9223372036854775808L
	a = 6364136223846793005L
	c = 1442695040888963407L
	_seed = (a*_seed + c) % m
	k = _seed
	_pt_thread_lock.release()
	return k

def get_seed():
	global _pt_thread_lock
	global _seed
	_pt_thread_lock.acquire(True)
	a = "%li"%_seed
	_pt_thread_lock.release()
	return a

def shuffle(inlist):
	workable = list(inlist)
	retlist = []
	for k in range(len(inlist)):
		index = genrand()%len(workable)
		retlist.append(workable[index])
		del workable[index]
	return retlist
def choice(inlist):
	return inlist[genrand()%len(inlist)]

def sample(inlist,num):
	workable = list(inlist)
	retlist = []
	for k in range(num):
		index = genrand()%len(workable)
		retlist.append(workable[index])
		del workable[index]
	return retlist

