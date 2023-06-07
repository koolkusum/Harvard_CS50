SELECT title FROM movies WHERE year = 2008;

SELECT birth FROM people WHERE name = "Emma Stone";

SELECT title FROM movies WHERE year >= 2018 ORDER BY title;

SELECT COUNT (movie_id) FROM ratings WHERE rating = 10.0;

SELECT title, year FROM movies WHERE title LIKE 'Harry Potter%' ORDER BY year;

SELECT avg(rating) FROM ratings WHERE movie_id IN (SELECT id FROM movies WHERE year = 2012);

SELECT movies.title, ratings.rating FROM movies JOIN ratings ON movies.id = ratings.movie_id WHERE year = 2010 ORDER BY rating DESC, title ASC;

SELECT name FROM people JOIN stars ON person_id = people.id JOIN movies ON movies.id = movie_id WHERE movies.title = "Toy Story";

SELECT DISTINCT name FROM people JOIN stars ON person_id = people.id JOIN movies ON movies.id = movie_id WHERE movies.year IN (SELECT year FROM movies WHERE year = 2004) ORDER BY birth; 

SELECT name FROM people WHERE people.id IN(SELECT DISTINCT(people.id) FROM people JOIN directors ON directors.person_id = people.id JOIN movies ON movies.id = directors.movie_id JOIN ratings ON ratings.movie_id = movies.id WHERE rating >= 9.0);

SELECT title FROM movies WHERE movies.id IN (SELECT movies.id FROM movies JOIN stars ON stars.movie_id = movies.id JOIN people ON people.id = stars.person_id JOIN ratings ON ratings.movie_id = movies.id WHERE people.name = "Chadwick Boseman" )ORDER BY (SELECT rating FROM ratings WHERE ratings.movie_id = movies.id) DESC LIMIT 5;

SELECT title FROM movies WHERE movies.id IN (SELECT movies.id FROM movies JOIN stars ON stars.movie_id = movies.id JOIN people ON people.id = stars.person_id WHERE people.name = 'Helena Bonham Carter' AND movies.id IN (SELECT movies.id FROM movies JOIN stars ON stars.movie_id = movies.id JOIN people ON people.id = stars.person_id WHERE people.name = 'Johnny Depp'));

SELECT DISTINCT name FROM people JOIN stars ON people.id = stars.person_id JOIN movies ON movies.id = stars.movie_id WHERE movies.id IN(SELECT movies.id FROM movies JOIN stars ON movies.id = stars.movie_id JOIN people ON stars.person_id = people.id WHERE name = 'Kevin Bacon' AND birth = 1958)AND name != 'Kevin Bacon';