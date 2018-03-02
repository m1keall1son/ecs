# Database Framework 

The database uses django (v1.10) to manage the database models and 
migrations. 

## Requirements 

* python 2.7 
* django 1.10
* rest_framework for django

Once you have python installed, you can run:

```
python get-pip.py
pip install -r requirements.txt
```

to make sure that all necessary modules are installed.

## Configuration 

Make sure you have the directory `C:\MidnightCommercial\databases\` 
created on your machine before you start. (Or try it without and let me 
know if it automatically creates it!) If you'd like to put the database 
into a different directory, update the `DATABASES` object in 
`t2_database\t2_database\settings.py` to a new directory.

## Usage
The project (the entire t2 database) is grouped into "applications", 
which are the reusable modules. The T2 database project has two 
applications: the logging application (`log`) and the Store API 
application (`store_api`). 

If you need to make changes to the database schema (the model in 
Django), edit `models.py` in the respective folder (`logs\` for 
log-related schema, `store_api` for store-related schema). Once you've 
edited the schema, type 

``` 
python ./manage.py makemigrations [schema name] --empty
```
then
``` 
python ./manage.py makemigrations [schema name]
```

This is equivalent to a git commit; internally django prepares to make 
all of the changes and logs the changes so you can migrate backwards 
later if necessary. 

To go ahead and make the migrations (analogous to pushing your commit), 
type: 

``` 
python ./manage.py migrate 
``` 

And then your database will be updated! 

To view the private administrative interface, you'll need to create an admin user:

```
python ./manage.py createsuperuser
```

then run the test server:

```
python ./manage.py runserver
```

if you encounter the error `Segmentation fault`, close your terminal, wait 10 min and try again.

While the server is running you can access the private interface at `http://127.0.0.1:8000/admin`.

## Database Population
Once you've followed the above steps, you should a working database that is empty.

From the private administrative interface you should be able to add or edit items one by one; however, 
this can get old quickly, so I've written a custom command to populate the database from a corpus of sample
data.

By running the command

```
python ./manage.py populate_store
```

you will randomly populate the database with:

* 3 categories -- fruits, vegetables, and breads
* each with one subcategory,
* containing 3 clouds,
* with a random grouping of between 12 and 25 randomly selected products

The test server must also always be running for the C++ API to access the server.

## Troubleshooting
If you run into issues with the test database not migrating properly, the easiest current way to nuke it and start over is:

* delete `C:\MidnightCommercial\databases\`
* delete `t2-IW\research\t2_database\store_api\migrations`
* run:
** `python manage.py makemigrations store_api --empty`
** `python manage.py makemigrations store_api`
** `python manage migrate`
** `python manage populate_store`