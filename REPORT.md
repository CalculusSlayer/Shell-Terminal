# ECS 150 Shell Terminal Report
## By Nayeel Imtiaz and Ky Ngo

We used several data structures in
this project such as Linked Lists
and various custom objects. Examples
of custom objects we created include
the Process object and the StringArray
object.

The Process object is a pointer to a
ProcessObj structure which has fields
such as program (char\*), left\_args
(LinkedList), file\_name (LinkedList),
and FO\_type (enum File Operator).

The StringArray is a pointer to a
StringArrayObj which two fields: arr (char\*\*)
and length (int). It is basically the same as
a char\*\* object, except it contains an
easily to refer length attribute. This makes
deallocating memory for char\*\* easier as the 
length is known.
