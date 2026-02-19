Motivation Maker is a parser that will replace TAGs in the text by whichever word you desire, it is made in C and specifically made to be fed ODT files

Usage : ./Motivation_Maker *.odt

Requires : libzip-dev

What is an ODT file ?

It is the default format for Libre Office word
From a technical stand point it is a Archive containing a plethora of files.
source : https://opensource.com/article/22/8/odt-files

As we aim to simply modify the the content of the ODT file, we focus on the content.xml, the file originally parsed like a HTML page, not human readable.


How can we parse content.xml ?

First of we use strstr to find the first occurence of the TAG we need to replace before offseting the string pointer to after TAG before calling another strstr.
The man advantage of this technic is twofold:
- enables us to not cycle through every character in the text
- it returns a pointer to the beginning of the TAG within the text

These informations are packed in linked list called a list of edits

Although now we know where the TAGs are simply writing over them would naive as the desired words may not have the same size, in otherwords we need to create a new string from scrath with the right size.


How can we modify the content ?

As we now know where the all the TAGs are in the text we must, calculate the size of the new string, we simply sum of the difference between TAGs and replacer_strings
The idea to "modify" the content will be to copy the ORIGINAL STRING into the new one write the replacer_string whenever we come across a TAG.
But as we already have the size of the NEW STRING alloc'd, it is more efficient to use strncpy, and our list of edits

in other to do that we will have two thing:
- writing cursor ptr => simply put a pointer which will get offset according to the amount we wrote in the NEW STRING
- reading cursor ptr => a pointer which will be offset by the amount we read in the ORIGINAL STRING

The diffence between the reading cursor the edit's pointer to the beginning of the TAG will give the number of the char that needs to be written before reaching our TAG
after which the number of char written is added the to reading pointer + the length of TAG in char
As for our writing cursor, the number of the char that needs to be written before reaching our TAG is also added to it,
before we write our replacer string then offset the writing cursor by the length of the replacer string.

The technic has a flaw: what if an edit's pointer points to an address which that is lower than our reading cursor, meaning that the edit's address was overtook and that original string was copied over it.

The only thing we can do is to obvioulsy insert the edit, as the NEW STRING has the right size to accomodate the replacer string's length we don't have to worry about realloc's.
To insert our edits we can simply split the NEW STRING where the TAG is then stock the overflow minus the TAG in a Buffer string, after what we can write our edits and concatenate our NEW STRING and Buffer.

Motivation_Maker might not be the most efficient way to deal with an ODT file, this has for objective to be side project which hinges on memory management and allocation.

A good way to parse a text would also be the extensive use of sprintf.
