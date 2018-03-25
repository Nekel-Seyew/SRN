import pt_rand

def reproduce(a,b, mutate_func, numkids = 10, mutate_chance=0.001):
	kids = []
	agenes = a.get_genes()
	bgenes = b.get_genes()
	#half-and-half -> 2
	a1 = agenes[:len(agenes)/2]
	a2 = agenes[len(agenes)/2:]
	b1 = bgenes[:len(bgenes)/2]
	b2 = bgenes[len(bgenes)/2:]
	kids.append(organism(a1+b2))
	kids.append(organism(b1+a2))
	#interleave -> 2
	kida = []
	kidb = []
	for i in range(len(agenes)):
		if i % 2 == 0:
			a.append(agenes[i])
			b.append(agenes[i])
		else:
			a.append(bgenes[i])
			b.append(agenes[i])
	kids.append(organism(kida))
	kids.append(organism(kidb))
	#if we have too many, mutate and return shuffled selection
	if len(kids) > numkids:
		for i in range(len(kids)):
			k = kids[i]
			if pt_rand.genrand()%(1/mutate_chance) == 1:
				k = k.mutate(mutate_func, mutate_chance)
			kids[i] = k
		return pt_rand.shuffle(kids)[:numkids]
	#if we don't have enough yet, start randomly breeding
	numneeded = numkids - 4
	for i in range(numneeded):
		kid = []
		for k in range(len(agenes)):
			choice = pt_rand.genrand()%2
			if choice == 0:
				kid.append(agenes[k])
			else:
				kid.append(bgenes[k])
		kids.append(organism(kid))
	for i in range(len(kids)):
		k = kids[i]
		if pt_rand.genrand()%(1/mutate_chance) == 1:
			k = k.mutate(mutate_func, mutate_chance)
		kids[i] = k
	return kids


class gene(object):
	def __init__(self,data):
		self._data = data
	def get_data(self):
		return self._data

class organism(object):
	def __init__(self,genes):
		self._genes = list(genes)
	def get_genes(self):
		return self._genes
	def add_gene(self,gene,pos=-1):
		self._genes.insert(pos,gene)
		return self
	def mutate(self,mutate_func, chance):
		for i in range(len(self._genes)):
			g = self._genes[i]
			if pt_rand.genrand()%(1/chance) == 1:
				g = mutate_func(g)
			self._genes[i] = g
		return self

class genetic_system(object):
	def __init__(self, fitness_func, mutate_func, reproduction=reproduce, generation):
		self._fitness_func = fitness_func
		self._mutate_func = mutate_func
		self._reproduction = reproduction
		self._generation = generation
	def iterate(self, times, numkids = 10, gensize = 100, mutate_chance=0.001):
		for i in range(times):
			allchild = []
			for x in self._generation:
				for y in self._generation:
					if x == y:
						continue
					children = self._reproduction(x,y, self._mutate_func, fitness_cutoff, numkids, mutate_chance)
					allchild = allchild + children
			allchild.sort(key=lambda x: self._fitness_func(c),reverse=True)
			retkids = allchild[:gensize]
			self._generation = retkids
	def get_generation(self):
		return self._generation
	def breed(self, numkids=10, gensize=100, mutate_chance=0.001):
		allchild = []
		for x in self._generation:
			for y in self._generation:
				if x == y:
					continue
				children = self._reproduction(x,y, self._mutate_func, fitness_cutoff, numkids, mutate_chance)
				allchild = allchild + children
		retkids = pt_rand.shuffle(allchild)[:gensize]
		self._generation = retkids
	def reap(self, cutoff):
		keep = []
		for k in self._generation:
			if cutoff <= self._fitness_func(k):
				keep.append(k)
		self._generation = keep
