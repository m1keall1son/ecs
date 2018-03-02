from __future__ import unicode_literals

from django.db import models

# Create your models here.

MODEL_TYPE_CHOICES = (
(0, 'Category'),
(1, 'SubCategory'),
(2, 'Cloud'),
(3, 'Product'),
(4, 'Allergen'),
(5, 'Global Parameter')
)


class Category(models.Model):
	type = models.SmallIntegerField(choices=MODEL_TYPE_CHOICES, default=0)
	uuid = models.CharField(max_length=32, primary_key=True)
	name = models.CharField(max_length=256)
	description = models.TextField(blank=True)
	promotion = models.TextField(blank=True)
	far_away = models.CharField(max_length=256)
	background = models.CharField(max_length=256)
	icon = models.CharField(max_length=256)
	inventory = models.ManyToManyField('SubCategory')
	etag = models.CharField(max_length=256)
	
	def __str__(self):
		return self.name

		
class SubCategory(models.Model):
	type = models.SmallIntegerField(choices=MODEL_TYPE_CHOICES, default=1)
	uuid = models.CharField(max_length=32, primary_key=True)
	name = models.CharField(max_length=256)
	description = models.TextField(blank=True)
	priority = models.PositiveIntegerField()
	promotion = models.TextField(blank=True)
	icon = models.CharField(max_length=256)
	clouds = models.ManyToManyField('Cloud')
	etag = models.CharField(max_length=256)
	
	def __str__(self):
		return self.name

		
class Cloud(models.Model):
	type = models.SmallIntegerField(choices=MODEL_TYPE_CHOICES, default=2)
	uuid = models.CharField(max_length=32, primary_key=True)
	name = models.CharField(max_length=256)
	description = models.TextField(blank=True)
	promotion = models.TextField(blank=True)
	icon = models.CharField(max_length=256)
	products = models.ManyToManyField('Product')
	etag = models.CharField(max_length=256)

	def __str__(self):
		return self.name


class Product(models.Model):
	DISPLAY_SIZE_CHOICES = (
        (0, 'STANDARD'),
        (1, 'FEATURED'),
        (2, 'HIGHLIGHT')
	)

	type = models.SmallIntegerField(choices=MODEL_TYPE_CHOICES, default=3)
	name = models.CharField(max_length=256)
	brand = models.CharField(max_length=256, blank=True)
	description = models.TextField(blank=True)
	promotion = models.TextField(blank=True)
	barcode = models.CharField(max_length=256, blank=True)
	sku = models.CharField(max_length=256, primary_key=True)
	price = models.DecimalField(max_digits=7, decimal_places=2)
	in_stock = models.BooleanField()    
	display_size = models.SmallIntegerField(choices=DISPLAY_SIZE_CHOICES, default=0)
	sale_start_time = models.DateTimeField()
	sale_end_time = models.DateTimeField()
	sale_price = models.DecimalField(max_digits=7, decimal_places=2)
	sale_description = models.TextField(blank=True)
	allergens = models.ManyToManyField('Allergen')
	image = models.CharField(max_length=256)
	thumbnail = models.CharField(max_length=256)
	etag = models.CharField(max_length=256)

	def __str__(self):
		return self.name
				
class Allergen(models.Model):
	type = models.SmallIntegerField(choices=MODEL_TYPE_CHOICES, default=4)
	name = models.CharField(max_length=256, primary_key=True)
	description = models.TextField(blank=True)
	
	def __str__(self):
		return self.name
		
class GlobalParameter(models.Model):
	TYPE_CHOICES = (
	(0, 'INT'),
	(1, 'FLOAT'),
	(2, 'STRING')
	)

	type = models.SmallIntegerField(choices=MODEL_TYPE_CHOICES, default=5)
	key = models.CharField(max_length=256, primary_key=True)
	value = models.CharField(max_length=256)
	datatype = models.SmallIntegerField(choices=TYPE_CHOICES, default=0)
	
	def __str__(self):
		return self.key