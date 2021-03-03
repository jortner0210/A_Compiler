/*
	The purpose of the Parser is to match a stream of catorized words
	against the rules that specify syntax for the language. To determine
	if the input stream is a sentence in the source language.

	- i.e. 
		Grammatical Rules:
		 1) Sentence -> Subject verb Object endmark
		 2) Subject  -> noun
		 3) Subject  -> Modifier noun
		 4) Object   -> noun
		 5) Object   -> Modifier noun
		 6) Modifier -> adjective

		Sentence: "Compilers are engineered objects."
		 
		 Start  - Sentence
		 Rule 1 - Subject verb Object endmark
		 Rule 2 - noun verb Object endmark
		 Rule 5 - noun verb Modifier noun endmark
		 Rule 6 - noun verb adjective noun endmark
*/