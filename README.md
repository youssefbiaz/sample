# Youssef Biaz Sample Code

- **Name**: Youssef Biaz
- **Email**: biaz@usc.edu

This sample code is an early stage of a project I worked on for my
data structures class. I implemented my own basic Netflix
application. The functionality available in this version is creating
users, logging in, and searching for movies based on titles or
keywords (the only two films in the data file for now are City Lights
and The Princess Bride, but you can add your own films with keywords
to the movie data file and it will work).

In later stages, I added more functionality such as searching by
actor, the ability to rate movies from 1 to 5, a recommendation system
based on users' similarities in ratings, and an option to play the
Kevin Bacon Game. I also implemented the entire application using
Qt. Contact me for access to the final project.

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
