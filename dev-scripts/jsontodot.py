import json

global objs
objs = dict()
global rels
rels = list()
#tree = json.loads('{"name" : "foo", "value" : {"bar":["baz", null, 1.0, 2]}, "test" : {"__mem_address__":"0xAAA"}, "test2" : {"__mem_address__":"0xAAA"}}')
tree = json.loads(open ("build/tmp.txt", "r").read())
global cnt
cnt = 0
classColors = dict()

print "digraph G {"
print ' overlap=prism'
print 'colorsheme=paired12'
#print 'weight = 0.2'
#print 'size="1000,1000"'
print 'nodestep=300'
print 'fontname = "Bitstream Vera Sans"'
print 'fontsize = 8'

print 'node ['
print 'fontname = "Bitstream Vera Sans"'
print 'fontsize = 8'
print 'shape = "record"'
print ']'

print 'edge ['
print 'fontname = "Bitstream Vera Sans"'
print 'fontsize = 8'
#print 'len = 10'
print ']'

def getName():
	global cnt
	name = 'obj_%d' % (cnt)
	cnt = cnt + 1
	return name

def getObjColor(obj):
	global classColors
	if obj.has_key('__class_name__'):
		cl = obj['__class_name__']
		if classColors.has_key(cl) == False:
			classColors[cl] = 'style=filled , fillcolor = "/set312/%d"' % (len(classColors)+1)
		return classColors[cl]
	else:
		return ''

def addObject(parent,obj):
	global cnt
	if obj.has_key('__mem_address__'):
		name = 'addr_'+str(obj['__mem_address__'])
	else:
		name = getName()
	if (parent != '__mem_objects__'):
		rels.append(parent + ' -> ' + name)
	if objs.has_key(name):
		return name
	objs[name] = name
	if obj.has_key('__class_name__'):
		tmp = "\t" + name + ' [ label = "{' + obj['__class_name__'] + ' : ' + name + '|'	
	else:
		tmp = "\t" + name + ' [ label = "{' + name + '|'
	for key, value in obj.items():
		if isinstance(value,dict):
			res = addObject(name,value)
			tmp = tmp + '+' + key + ' : ' + res + '\l'
		elif key == '__mem_address__':
			tmp = tmp
		elif key == '__mem_objects__':
			parseJson('__mem_objects__',value)
		elif key == '__mem_links__':
			for link in value:
				if link['to'] != 'NULL' and link['to'] != None:
					rels.append('addr_'+link['from'] + ' -> ' + 'addr_' + link['to'])
		elif key == '__class_name__':
			tmp = tmp
		elif isinstance(value,list):
			tmpName = name + '_' + key
			print tmpName + '[ label = "LIST ' + key + '", shape = "diamond" ]'
			rels.append(name + ' -> ' + tmpName)
			parseJson(tmpName,value)
		else:
			tmp = tmp + '+' + str(key) + ' : ' + str(value) + '\l'
	tmp = tmp + '}" ' + getObjColor(obj) + ' ] '
	print tmp
	return name

def simpleObj(parent,obj):
	name = getName()
	objs[name] = name
	rels.append(parent + ' -> ' + name)	
	print name + '[ shape = ellipse ,label = "' + str(obj) + '" ]'
	return name

def parseJson(parent,value):
	if isinstance(value,list):
		for item in value:
			parseJson(parent,item)
	elif isinstance(value,dict):
		#print "ok object"
		addObject(parent,value);
	else:
		simpleObj(parent,value);

parseJson('root',tree)

for item in rels:
	print "\t" + item

print "}"
