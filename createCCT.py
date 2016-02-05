import sys
class MyException(Exception):
	def __init__(self, value):
		self.value = value

class Edge:
	def __init__(self, src, dst):
		self.weight = 1
		self.caller = src
		self.callee = dst

class Node:
	def __init__(self, func_name):
		self.name = func_name
		self.parent_edge = None
		self.edges = []

	def __str__(self):
		str_ = ""
		for e in self.edges:
			str_ += str(e.callee)+', '
		if len(self.edges) != 0:
			str_ = ' ['+str_[0:-2]+']'

		if self.name == 'main':
			return self.name+str_
		else: 
			return self.name+'-'+str(self.parent_edge.weight)+str_

	def define_parent(self, parent_e):
		self.parent_edge = parent_e

	def add_child(self, child_name):
		for e in self.edges:
			if e.callee.name == child_name:
				e.weight += 1
				return e.callee

		child = Node(child_name)
		new_edge = Edge(self, child)
		child.define_parent(new_edge)
		self.edges.append(new_edge)
		return child


# sys.setrecursionlimit(100)
root = Node("main")
current = root #current top element in the stack

lines = [line.rstrip('\n') for line in open('trace-2.txt', 'r')]

for line in lines:
	if line.startswith('[Call begin] '):
		name = line[12:]
		current = current.add_child(name)

	elif line.startswith('[Call end] '):
		name = line[10:]
		if current != root:
			current = current.parent_edge.caller
		else: raise MyException("Invalid Trace Call!")

	else: pass #ignore other lines

print root #representing the whole tree

