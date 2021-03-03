import sys
import random

ID_CHAR_LIST = [
	'a',  'b',  'c',  'd',  'e',  'f',	
	'g',  'h',  'i',  'j',  'k',  'l',	
	'm',  'n',  'o',  'p',  'q',  'r',	
	's',  't',  'u',  'v',  'w',  'x',	
	'y',  'z', 		
	'A',  'B',  'C',  'D',  'E',  'F',	
	'G',  'H',  'I',  'J',  'K',  'L',	
	'M',  'N',  'O',  'P',  'Q',  'R',	
	'S',  'T',  'U',  'V',  'W',  'X',	
	'Y',  'Z',
	'_',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
]

OPERATOR_LIST = [
	'<', '<<', '<=',
	'>', '>>', '>=',
	'+', '++', '+=',
	'-', '--', '-=',
	'~', '^', 
	'=', '==', 
	'!', '!=', 
	'*', '*=', 
	'/', '/=', 
	'%', '%=',
	'&', '&&',
	'|', '||'
]

PUNCTUATION_LIST = [
	'(', ')', ';', ':', '[', ']', '{', '}', '.', ','
]

TOKEN_TYPE_LIST = [
	"integer", "float", "identifier", "operator", "punctuation"
]

SEPARATOR_LIST = [
	' ', '\n', '\t'
]

if __name__ == "__main__":

	if len(sys.argv) < 2:
		print("Provide file name, and token count")
	else:
		file_name   = sys.argv[1]
		token_count = int(sys.argv[2])
		
		out_file = open(file_name, "w")

		for i in range(token_count):
			tok_type  = random.choice(TOKEN_TYPE_LIST)
			separator = random.choice(SEPARATOR_LIST)
			
			if tok_type == "identifier":
				build_id = ""
				char_count = random.randint(1, 12)
				for _ in range(char_count):
					build_id += random.choice(ID_CHAR_LIST)
				
				out_file.write("{}{}".format(build_id, separator))

			elif tok_type == "operator":
				op = random.choice(OPERATOR_LIST)
				out_file.write("{}{}".format(op, separator))
			
			elif tok_type == "float":
				number   = random.uniform(0, 100)
				positive = random.randint(0, 1)
				if positive == 1:
					number *= -1
				#print("FLOAT: {}, POSITIVE: {}".format(number, positive))
				out_file.write("{}{}".format(number, separator)
)
			elif tok_type == "integer":	
				number   = random.randint(0, 100)
				positive = random.randint(0, 1)
				if positive == 1:
					number *= -1
				#print("INT: {}, POSITIVE: {}".format(number, positive))
				out_file.write("{}{}".format(number, separator))

			else: # punctuation
				punc = random.choice(PUNCTUATION_LIST)
				out_file.write("{}{}".format(punc, separator))
		
		out_file.close()
