# Facebook App Sample Code

- **Name**: Youssef Biaz
- **Email**: biaz@usc.edu

This sample code is a project I'm currently working on for
class. We're creating our own basic Netflix. This is still only in its
first stages, so the only functionality available is creating users,
logging in, and searching for movies based on titles or keywords (the
only two films in the data file for now are City Lights and The
Princess Bride, but you can add your own films with keywords to the
movie data file and it will work.

***Compilation Instructions***: To compile, just run `make`. Then to
   run with the provided data file, just type the command `bin/main
   mdata.txt`


***Extra Notes***: 
- user ID's are case sensitive. 'Aaroncot' and
'aaroncot' are two different usernames.  
- keywords that are added are
case sensitive, but when searched they are not. For instance, 'comedy'
and 'COMEDY' would be added as two separate keywords, but when a user
searches for either both film matches would come up. The reason for
this is because I'd allow uniqueness to be determined by the data but
not by a searching user. If the movie data file wants those keywords
to be different so be it--but a user needs to be able to access all
keywords.
